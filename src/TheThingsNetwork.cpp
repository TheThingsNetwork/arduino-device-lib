// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <Arduino.h>
#include <TheThingsNetwork.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

void TheThingsNetwork::init(Stream& modemStream, Stream& debugStream) {
  this->modemStream = &modemStream;
  this->debugStream = &debugStream;
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
    debugPrintLn("Wait for OK time-out expired");
    return false;
  }

  if (line != okMessage) {
    debugPrintLn("Response is not OK: " + line);
    return false;
  }

  return true;
}

String TheThingsNetwork::readValue(String cmd) {
  modemStream->println(cmd);
  return readLine();
}

bool TheThingsNetwork::sendCommand(String cmd, int waitTime) {
  debugPrintLn("Sending: " + cmd);

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
  debugPrintLn("Sending: " + cmd + " with " + String(length) + " bytes");

  modemStream->print(cmd + " ");

  for (int i = 0; i < length; i++) {
    modemStream->print(btohexa_high(buf[i]));
    modemStream->print(btohexa_low(buf[i]));
  }
  modemStream->println();

  return waitForOK(waitTime);
}

void TheThingsNetwork::reset(bool adr, int sf, int fsb) {
#if !ADR_SUPPORTED
  adr = false;
#endif

  modemStream->println("sys reset");
  String version = readLine(3000);
  if (version == "") {
    debugPrintLn("Invalid version");
    return;
  }

  model = version.substring(0, version.indexOf(' '));
  debugPrintLn("Version is " + version + ", model is " + model);

  sendCommand("mac set adr " + String(adr ? "on" : "off"));

  int dr = -1;
  if (model == "RN2483") {
    sendCommand("mac set pwridx " + String(PWRIDX_868));

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
        debugPrintLn("Invalid SF")
        break;
    }
  }
  else if (model == "RN2903") {
    sendCommand("mac set pwridx " + String(PWRIDX_915));
    enableFsbChannels(fsb);

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
        debugPrintLn("Invalid SF")
        break;
    }
  }

  if (dr > -1)
    sendCommand("mac set dr " + String(dr));
}

bool TheThingsNetwork::enableFsbChannels(int fsb) {
  int chLow = fsb > 0 ? (fsb - 1) * 8 : 0;
  int chHigh = fsb > 0 ? chLow + 7 : 71;
  int ch500 = fsb + 63;

  for (int i = 0; i < 72; i++)
    if (i == ch500 || chLow <= i && i <= chHigh)
      sendCommand("mac set ch status " + String(i) + " on");
    else
      sendCommand("mac set ch status " + String(i) + " off");
  return true;
}

bool TheThingsNetwork::personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]) {
  sendCommand("mac set devaddr", devAddr, 4);
  sendCommand("mac set nwkskey", nwkSKey, 16);
  sendCommand("mac set appskey", appSKey, 16);
  sendCommand("mac join abp");

  String response = readLine();
  if (response != "accepted") {
    debugPrintLn("Personalize not accepted: " + response);
    return false;
  }

  debugPrintLn("Personalize accepted. Status: " + readValue("mac get status"));
  return true;
}

bool TheThingsNetwork::join(const byte appEui[8], const byte appKey[16]) {
  String devEui = readValue("sys get hweui");
  sendCommand("mac set appeui", appEui, 8);
  sendCommand("mac set deveui " + devEui);
  sendCommand("mac set appkey", appKey, 16);
  if (!sendCommand("mac join otaa")) {
    debugPrintLn("Send join command failed");
    return false;
  }

  String response = readLine(10000);
  if (response != "accepted") {
    debugPrintLn("Join not accepted: " + response);
    return false;
  }

  debugPrintLn("Join accepted. Status: " + readValue("mac get status"));
  return true;
}

int TheThingsNetwork::sendBytes(const byte* buffer, int length, int port, bool confirm) {
  if (!sendCommand("mac tx " + String(confirm ? "cnf" : "uncnf") + " " + String(port), buffer, length)) {
    debugPrintLn("Send command failed");
    return 0;
  }

  String response = readLine(10000);
  if (response == "") {
    debugPrintLn("Time-out");
    return 0;
  }
  if (response == "mac_tx_ok") {
    debugPrintLn("Successful transmission");
    return 0;
  }
  if (response.startsWith("mac_rx")) {
    int portEnds = response.indexOf(" ", 7);
    this->downlinkPort = response.substring(7, portEnds).toInt();
    String data = response.substring(portEnds + 1);
    int downlinkLength = data.length() / 2;
    for (int i = 0, d = 0; i < downlinkLength; i++, d += 2)
      this->downlink[i] = HEX_PAIR_TO_BYTE(data[d], data[d+1]);
    debugPrintLn("Successful transmission. Received " + String(downlinkLength) + " bytes");
    return downlinkLength;
  }

  debugPrintLn("Unexpected response: " + response);
}

int TheThingsNetwork::sendString(String message, int port, bool confirm) {
  int l = message.length();
  byte buf[l + 1];
  message.getBytes(buf, l + 1);

  return sendBytes(buf, l, port, confirm);
}

void TheThingsNetwork::showStatus() {
  debugPrintLn("EUI: " + readValue("sys get hweui"));
  debugPrintLn("Battery: " + readValue("sys get vdd"));
  debugPrintLn("AppEUI: " + readValue("mac get appeui"));
  debugPrintLn("DevEUI: " + readValue("mac get deveui"));
  debugPrintLn("DevAddr: " + readValue("mac get devaddr"));

  if (this->model == "RN2483") {
    debugPrintLn("Band: " + readValue("mac get band"));
  }

  debugPrintLn("Data Rate: " + readValue("mac get dr"));
  debugPrintLn("RX Delay 1: " + readValue("mac get rxdelay1"));
  debugPrintLn("RX Delay 2: " + readValue("mac get rxdelay2"));
}
