// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <Arduino.h>
#include <TheThingsNetwork.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

void TheThingsNetwork::init(Stream& modemStream, Stream& debugStream, int sf, int fsb) {
  this->modemStream = &modemStream;
  this->debugStream = &debugStream;
  configure_channels(sf, fsb);
}

String TheThingsNetwork::readLine(int waitTime) {
  unsigned long start = millis();
  while (millis() < start + waitTime) {
    String line = modemStream->readStringUntil('\n');
    if (line.length() > 0) {
      return line.substring(0, line.length() - 1);
    }
  }
  return "";
}

bool TheThingsNetwork::waitForOK(int waitTime, String okMessage) {
  String line = readLine(waitTime);
  if (line == "") {
    debugPrintLn(F("Wait for OK time-out expired"));
    return false;
  }

  if (line != okMessage) {
    debugPrint(F("Response is not OK: "));
    debugPrintLn(line);
    return false;
  }

  return true;
}

String TheThingsNetwork::readValue(String cmd) {
  modemStream->println(cmd);
  return readLine();
}

bool TheThingsNetwork::sendCommand(String cmd, int waitTime) {
  debugPrint(F("Sending: "));
  debugPrintLn(cmd);

  modemStream->println(cmd);

  return waitForOK(waitTime);
}

bool TheThingsNetwork::sendCommand(String cmd, String value, int waitTime) {
  int l = value.length();
  byte buf[l];
  value.getBytes(buf, l);

  return sendCommand(cmd, buf, l, waitTime);
}

char btohexa_high(unsigned char b) {
  b >>= 4;
  return (b > 0x9u) ? b + 'A' - 10 : b + '0';
}

char btohexa_low(unsigned char b) {
  b &= 0x0F;
  return (b > 0x9u) ? b + 'A' - 10 : b + '0';
}

bool TheThingsNetwork::sendCommand(String cmd, const byte *buf, int length, int waitTime) {
  debugPrint(F("Sending: "));
  debugPrint(cmd);
  debugPrint(F(" with "));
  debugPrint(length);
  debugPrintLn(F(" bytes"));

  modemStream->print(cmd + " ");

  for (int i = 0; i < length; i++) {
    modemStream->print(btohexa_high(buf[i]));
    modemStream->print(btohexa_low(buf[i]));
  }
  modemStream->println();

  return waitForOK(waitTime);
}

void TheThingsNetwork::reset(bool adr) {
  #if !TTN_ADR_SUPPORTED
    adr = false;
  #endif

  modemStream->println(F("sys reset"));
  String version = readLine(3000);
  if (version == "") {
    debugPrintLn(F("Invalid version"));
    return;
  }
  
  model = version.substring(0, version.indexOf(' '));
  debugPrint(F("Version is "));
  debugPrint(version);
  debugPrint(F(", model is "));
  debugPrintLn(model);
  
  String devEui = readValue(F("sys get hweui"));
  String str = "";
  str.concat(F("mac set deveui "));
  str.concat(devEui);
  sendCommand(str);

  String devEui = readValue(F("sys get hweui"));
  String str = "";
  str.concat(F("mac set deveui "));
  str.concat(devEui);
  sendCommand(str);

  str = "";
  str.concat(F("mac set adr "));
  if(adr){
    str.concat(F("on"));
  } else {
    str.concat(F("off"));
  }
  sendCommand(str);
}

bool TheThingsNetwork::enableFsbChannels(int fsb) {
  int chLow = fsb > 0 ? (fsb - 1) * 8 : 0;
  int chHigh = fsb > 0 ? chLow + 7 : 71;
  int ch500 = fsb + 63;

  for (int i = 0; i < 72; i++){
    String str = "";
    str.concat(F("mac set ch status "));
    str.concat(i);
    if (i == ch500 || chLow <= i && i <= chHigh){
      str.concat(F(" on"));
    }
    else{
      str.concat(F(" off"));
    }
    sendCommand(str);
  }
  return true;
}

void TheThingsNetwork::onMessage(void (*cb)(const byte* payload, int length, int port)) {
  this->messageCallback = cb;
}

bool TheThingsNetwork::personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]) {
  reset();
  sendCommand(F("mac set devaddr"), devAddr, 4);
  sendCommand(F("mac set nwkskey"), nwkSKey, 16);
  sendCommand(F("mac set appskey"), appSKey, 16);
  return personalize();
}

bool TheThingsNetwork::personalize() {
  sendCommand(F("mac join abp"));
  String response = readLine();
  if (response != F("accepted")) {
    debugPrint(F("Personalize not accepted: "));
    debugPrintLn(response);
    return false;
  }

  debugPrint(F("Personalize accepted. Status: "));
  debugPrintLn(readValue(F("mac get status")));
  return true;
}

bool TheThingsNetwork::provision(const byte appEui[8], const byte appKey[16]) {
  sendCommand(F("mac set appeui"), appEui, 8);
  sendCommand(F("mac set appkey"), appKey, 16);
  return sendCommand(F("mac save"), 50000);
}

bool TheThingsNetwork::join(int retries, long int retryDelay) {
  String devEui = readValue(F("sys get hweui"));
  String str = "";
  str.concat(F("mac set deveui "));
  str.concat(devEui);
  sendCommand(str);
  while (retries != 0) {
    if (retries > 0) {
      retries--;
    }
    if (!sendCommand(F("mac join otaa"))) {
      debugPrintLn(F("Send join command failed"));
      delay(retryDelay);
      continue;
    }
    String response = readLine(10000);
    if (response != F("accepted")) {
      debugPrint(F("Join not accepted: "));
      debugPrintLn(response);
      delay(retryDelay);
      continue;
    }
    debugPrint(F("Join accepted. Status: "));
    debugPrintLn(readValue(F("mac get status")));
    debugPrint(F("DevAddr: "));
    debugPrintLn(readValue(F("mac get devaddr")));
    return true;
  }
  return false;
}

bool TheThingsNetwork::join(const byte appEui[8], const byte appKey[16], int retries, long int retryDelay) {
  reset();
  provision(appEui, appKey);
  return join(retries, retryDelay);
}

int TheThingsNetwork::sendBytes(const byte* payload, int length, int port, bool confirm) {
  String str = "";
  str.concat(F("mac tx "));
  str.concat(confirm ? F("cnf ") : F("uncnf "));
  str.concat(port);
  if (!sendCommand(str, payload, length)) {
    debugPrintLn(F("Send command failed"));
    return -1;
  }

  String response = readLine(10000);
  if (response == "") {
    debugPrintLn(F("Time-out"));
    return -2;
  }
  if (response == F("mac_tx_ok")) {
    debugPrintLn(F("Successful transmission"));
    return 1;
  }
  if (response.startsWith(F("mac_rx"))) {
    int portEnds = response.indexOf(" ", 7);
    int downlinkPort = response.substring(7, portEnds).toInt();
    String data = response.substring(portEnds + 1);
    int downlinkLength = data.length() / 2;
    byte downlink[64];
    for (int i = 0, d = 0; i < downlinkLength; i++, d += 2)
      downlink[i] = TTN_HEX_PAIR_TO_BYTE(data[d], data[d+1]);
    debugPrint(F("Successful transmission. Received "));
    debugPrint(downlinkLength);
    debugPrintLn(F(" bytes"));
    if (this->messageCallback)
      this->messageCallback(downlink, downlinkLength, downlinkPort);
    return 2;
  }

  debugPrint(F("Unexpected response: "));
  debugPrintLn(response);
  return -10;
}

int TheThingsNetwork::poll(int port, bool confirm) {
  byte payload[] = { 0x00 };
  return sendBytes(payload, 1, port, confirm);
}

void TheThingsNetwork::showStatus() {
  debugPrint(F("EUI: "));
  debugPrintLn(readValue(F("sys get hweui")));
  debugPrint(F("Battery: "));
  debugPrintLn(readValue(F("sys get vdd")));
  debugPrint(F("AppEUI: "));
  debugPrintLn(readValue(F("mac get appeui")));
  debugPrint(F("DevEUI: "));
  debugPrintLn(readValue(F("mac get deveui")));
  
  if (this->model == F("RN2483")) {
    debugPrint(F("Band: "));
    debugPrintLn(readValue(F("mac get band")));
  }

  debugPrint(F("Data Rate: "));
  debugPrintLn(readValue(F("mac get dr")));
  debugPrint(F("RX Delay 1: "));
  debugPrintLn(readValue(F("mac get rxdelay1")));
  debugPrint(F("RX Delay 2: "));
  debugPrintLn(readValue(F("mac get rxdelay2")));
}

void TheThingsNetwork::configure_EU868(int sf) {
  int ch;
  int dr = -1;
  long int freq = 867100000;
  String str = "";

  str.concat(F("mac set rx2 3 869525000"));
  sendCommand(str);
  str = "";
  for (ch = 0; ch <= 7; ch++) {
    if (ch >= 3) {
      str.concat(F("mac set ch freq "));
      str.concat(ch);
      str.concat(F(" "));
      str.concat(freq);
      sendCommand(str);
      str = "";
      str.concat(F("mac set ch drrange "));
      str.concat(ch);
      str.concat(F(" 0 5"));
      sendCommand(str);
      str = "";
      str.concat(F("mac set ch status "));
      str.concat(ch);
      str.concat(F(" on"));
      sendCommand(str);
      str = "";
      freq = freq + 200000;
    }
    str.concat(F("mac set ch dcycle "));
    str.concat(ch);
    str.concat(F(" 799"));
    sendCommand(str);
    str = "";
  }
  str.concat(F("mac set ch drrange 1 0 6"));
  sendCommand(str);
  str = "";
  str.concat(F("mac set pwridx "));
  str.concat(TTN_PWRIDX_868);
  sendCommand(str);
  switch (sf) {
    case 7:
      dr = 5;
      break;
    case 8:
      dr = 4;
      break;
    case 9:
      dr = 3;
      break;
    case 10:
      dr = 2;
      break;
    case 11:
      dr = 1;
      break;
    case 12:
      dr = 0;
      break;
    default:
      debugPrintLn(F("Invalid SF"))
      break;
  }
  if (dr > -1){
    str = "";
    str.concat(F("mac set dr "));
    str.concat(dr);
    sendCommand(str);
  }
}

void TheThingsNetwork::configure_US915(int sf, int fsb) {
  int ch;
  int dr = -1;
  String str = "";
 
  sendCommand(F("radio set freq 904200000"));
  str = "";
  str.concat(F("mac set pwridx "));
  str.concat(TTN_PWRIDX_915);
  sendCommand(str);
  enableFsbChannels(fsb);
  for (ch = 0; ch <= 63; ch++) {
    str = "";
    str.concat(F("mac set ch drrange "));
    str.concat(ch);
    str.concat(F(" 0 3"));
    sendCommand(str);
    str = "";
  }
  switch (sf) {
    case 7:
      dr = 3;
      break;
    case 8:
     dr = 2;
     break;
    case 9:
      dr = 1;
      break;
    case 10:
      dr = 0;
      break;
    default:
      debugPrintLn(F("Invalid SF"))
      break;
  }
  if (dr > -1){
    str = "";
    str.concat(F("mac set dr "));
    str.concat(dr);
    sendCommand(str);
  }
}

void TheThingsNetwork::configure_channels(int sf, int fsb) {
  switch (this->fp) {
    case TTN_FP_EU868:
      configure_EU868(sf);
      break;
    case TTN_FP_US915:
      configure_US915(sf, fsb);
      break;
    default:
      debugPrintLn("Invalid frequency plan");
      break;
  }
}

TheThingsNetwork::TheThingsNetwork(fp_ttn_t fp) {
  this->fp = fp;
}
