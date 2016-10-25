// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <Arduino.h>
#include <TheThingsNetwork.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

#define TTN_HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define TTN_HEX_PAIR_TO_BYTE(h, l) ((TTN_HEX_CHAR_TO_NIBBLE(h) << 4) + TTN_HEX_CHAR_TO_NIBBLE(l))

String TheThingsNetwork::readLine() {
  while (true) {
    String line = modemStream->readStringUntil('\n');
    if (line.length() > 0) {
      return line.substring(0, line.length() - 1);
    }
  }
}

String TheThingsNetwork::readValue(String cmd) {
  while(modemStream->available()) {
    modemStream->read();
  }

  modemStream->println(cmd);
  return readLine();
}

bool TheThingsNetwork::sendCommand(String cmd) {
  debugPrint(F("Sending: "));
  debugPrintLn(cmd);

  String line = readValue(cmd);

  if (line != "ok") {
    debugPrint(F("Response is not OK: "));
    debugPrintLn(line);
    return false;
  }

  return true;
}

bool TheThingsNetwork::sendCommand(String cmd, String value) {
  size_t l = value.length();
  byte buf[l];
  value.getBytes(buf, l);

  return sendCommand(cmd, buf, l);
}

char btohexa_high(unsigned char b) {
  b >>= 4;
  return (b > 0x9u) ? b + 'A' - 10 : b + '0';
}

char btohexa_low(unsigned char b) {
  b &= 0x0F;
  return (b > 0x9u) ? b + 'A' - 10 : b + '0';
}

bool TheThingsNetwork::sendCommand(String cmd, const byte *buf, size_t length) {
  String str = cmd + " ";

  for (size_t i = 0; i < length; i++) {
    str += btohexa_high(buf[i]);
    str += btohexa_low(buf[i]);
  }

  return sendCommand(str);
}

void TheThingsNetwork::reset(bool adr) {
  String version = readValue(F("sys reset"));
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

  str = "";
  str.concat(F("mac set adr "));
  if(adr){
    str.concat(F("on"));
  } else {
    str.concat(F("off"));
  }
  sendCommand(str);
}

void TheThingsNetwork::onMessage(void (*cb)(const byte* payload, size_t length, port_t port)) {
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
  configureChannels(this->sf, this->fsb);
  sendCommand(F("mac join abp"));
  String response = readLine();
  if (response != F("accepted")) {
    debugPrint(F("Personalize not accepted: "));
    debugPrintLn(response);
    return false;
  }

  fillAirtimeInfo();
  debugPrint(F("Personalize accepted. Status: "));
  debugPrintLn(readValue(F("mac get status")));
  return true;
}

bool TheThingsNetwork::provision(const byte appEui[8], const byte appKey[16]) {
  sendCommand(F("mac set appeui"), appEui, 8);
  sendCommand(F("mac set appkey"), appKey, 16);
  return sendCommand(F("mac save"));
}

bool TheThingsNetwork::join(int8_t retries, uint32_t retryDelay) {
  configureChannels(this->sf, this->fsb);
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
    String response = readLine();
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
    fillAirtimeInfo();
    return true;
  }
  return false;
}

bool TheThingsNetwork::join(const byte appEui[8], const byte appKey[16], int8_t retries, uint32_t retryDelay) {
  reset();
  provision(appEui, appKey);
  return join(retries, retryDelay);
}

int TheThingsNetwork::sendBytes(const byte* payload, size_t length, port_t port, bool confirm) {
  String str = "";
  str.concat(F("mac tx "));
  str.concat(confirm ? F("cnf ") : F("uncnf "));
  str.concat(port);
  if (!sendCommand(str, payload, length)) {
    debugPrintLn(F("Send command failed"));
    return -1;
  }

  String response = readLine();
  float i = this->airtime;
  trackAirtime(length);
  debugPrint(F("Airtime added: "));
  debugPrint(this->airtime - i);
  debugPrintLn(F(" s"));
  debugPrint(F("Total airtime: "));
  debugPrint(this->airtime);
  debugPrintLn(F(" s"));
  if (response == F("mac_tx_ok")) {
    debugPrintLn(F("Successful transmission"));
    return 1;
  }
  if (response.startsWith(F("mac_rx"))) {
    uint8_t portEnds = response.indexOf(" ", 7);
    port_t downlinkPort = response.substring(7, portEnds).toInt();
    String data = response.substring(portEnds + 1);
    size_t downlinkLength = data.length() / 2;
    byte downlink[64];
    for (size_t i = 0, d = 0; i < downlinkLength; i++, d += 2) {
      downlink[i] = TTN_HEX_PAIR_TO_BYTE(data[d], data[d+1]);
    }
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

int TheThingsNetwork::poll(port_t port, bool confirm) {
  byte payload[] = { 0x00 };
  return sendBytes(payload, 1, port, confirm);
}

void TheThingsNetwork::fillAirtimeInfo() {
  this->info.sf = 0;
  this->info.ps = 0;
  this->info.band = 0;
  this->info.header = 0;
  this->info.cr = 0;
  this->info.de = 0;

  uint8_t i;
  String message = readValue(F("radio get sf"));
  for (i = 2; message[i] && i <= 3; i++) {
    this->info.sf = (this->info.sf + message[i] - 48) * 10;
  }
  this->info.sf = this->info.sf / 10;

  message = readValue(F("radio get bw"));
  for (i = 0; message[i] && i <= 2; i++) {
    this->info.band = (this->info.band + message[i] - 48) * 10;
  }
  this->info.band = this->info.band / 10;

  message = readValue(F("radio get prlen"));
  this->info.ps = this->info.ps + message[0] - 48;

  message = readValue(F("radio get crc"));
  this->info.header = message == F("on") ? 1 : 0;

  message = readValue(F("radio get cr"));
  this->info.cr = (this->info.cr + message[2] - 48);

  this->info.de = this->info.sf >= 11 ? 1 : 0;
}

void TheThingsNetwork::trackAirtime(size_t payloadSize) {
  payloadSize = 13 + payloadSize;

  float Tsym = pow(2, this->info.sf) / this->info.band;
  float Tpreamble = (this->info.ps + 4.25) * Tsym;
  uint16_t payLoadSymbNb = 8 + (max(ceil((8 * payloadSize - 4 * this->info.sf + 28 + 16 - 20 * this->info.header) / (4 * (this->info.sf - 2 * this->info.de))) * (this->info.cr + 4), 0));
  float Tpayload = payLoadSymbNb * Tsym;
  float Tpacket = Tpreamble + Tpayload;
  this->airtime = this->airtime + (Tpacket / 1000);
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
  debugPrint(F("Total airtime: "));
  debugPrint(this->airtime);
  debugPrintLn(F(" s"));
}

void TheThingsNetwork::configureEU868(uint8_t sf) {
  uint8_t ch;
  int8_t dr = -1;
  uint32_t freq = 867100000;
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

void TheThingsNetwork::configureUS915(uint8_t sf, uint8_t fsb) {
  uint8_t ch;
  int8_t dr = -1;
  String str = "";
  uint8_t chLow = fsb > 0 ? (fsb - 1) * 8 : 0;
  uint8_t chHigh = fsb > 0 ? chLow + 7 : 71;
  uint8_t ch500 = fsb + 63;

  sendCommand(F("radio set freq 904200000"));
  str = "";
  str.concat(F("mac set pwridx "));
  str.concat(TTN_PWRIDX_915);
  sendCommand(str);
  for (ch = 0; ch < 72; ch++) {
    str = "";
    str.concat(F("mac set ch status "));
    str.concat(ch);
    if (ch == ch500 || ch <= chHigh && ch >= chLow) {
      str.concat(F(" on"));
      sendCommand(str);
      if (ch < 63) {
        str = "";
        str.concat(F("mac set ch drrange "));
        str.concat(ch);
        str.concat(F(" 0 3"));
        sendCommand(str);
        str = "";
      }
    }
    else {
      str.concat(F(" off"));
      sendCommand(str);
    }
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

void TheThingsNetwork::configureChannels(uint8_t sf, uint8_t fsb) {
  switch (this->fp) {
    case TTN_FP_EU868:
      configureEU868(sf);
      break;
    case TTN_FP_US915:
      configureUS915(sf, fsb);
      break;
    default:
      debugPrintLn("Invalid frequency plan");
      break;
  }
  String retries = "";
  retries.concat(F("mac set retx "));
  retries.concat(TTN_RETX);
  sendCommand(retries);
}

TheThingsNetwork::TheThingsNetwork(Stream& modemStream, Stream& debugStream, ttn_fp_t fp, uint8_t sf, uint8_t fsb) {
  this->debugStream = &debugStream;
  this->modemStream = &modemStream;
  this->fp = fp;
  this->sf = sf;
  this->fsb = fsb;
}
