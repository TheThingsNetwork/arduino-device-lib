// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <TheThingsNetwork.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

#define TTN_HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define TTN_HEX_PAIR_TO_BYTE(h, l) ((TTN_HEX_CHAR_TO_NIBBLE(h) << 4) + TTN_HEX_CHAR_TO_NIBBLE(l))

#define ON "on"
#define OFF "off"
#define OK "ok"
#define ACCEPTED "accepted"
#define MAC_TX_OK "mac_tx_ok"
#define MAC_RX "mac_rx"

#define INVALID_SF "Invalid SF"
#define INVALID_FP "Invalid frequency plan"
#define UNEXPECTED_RESPONSE "Unexpected response: "
#define SEND_COMMAND_FAILED "Send command failed"
#define JOIN_FAILED "Send join command failed"
#define JOIN_NOT_ACCEPTED "Join not accepted: "
#define PERSONALIZE_NOT_ACCEPTED "Personalize not accepted"
#define RESPONSE_IS_NOT_OK "Response is not OK: "

#define SENDING "Sending: "
#define SEND_MSG "\r\n"

const char radio_prefix[] PROGMEM = "radio";
const char radio_set[] PROGMEM = "set";
const char radio_get[] PROGMEM = "get";
const char radio_get_bw[] PROGMEM = "bw";
const char radio_get_prlen[] PROGMEM = "prlen";
const char radio_get_crc[] PROGMEM = "crc";
const char radio_get_cr[] PROGMEM = "cr";
const char radio_get_sf[] PROGMEM = "sf";

const char* const radio_table[] PROGMEM = {radio_prefix,radio_set,radio_get,radio_get_bw,radio_get_prlen,radio_get_crc,radio_get_cr,radio_get_sf};

#define RADIO_PREFIX 0
#define RADIO_SET 1
#define RADIO_GET 2
#define RADIO_GET_BW 3
#define RADIO_GET_PRLEN 4
#define RADIO_GET_CRC 5
#define RADIO_GET_CR 6
#define RADIO_GET_SF 7

const char sys_prefix[] PROGMEM = "sys";
const char sys_sleep[] PROGMEM = "sleep";
const char sys_reset[] PROGMEM = "reset";
const char sys_erase_fw[] PROGMEM = "eraseFW";
const char sys_factory_rst[] PROGMEM = "factoryRESET";
const char sys_set[] PROGMEM = "set";
const char sys_get[] PROGMEM = "get";
const char sys_get_ver[] PROGMEM = "ver";
const char sys_get_vdd[] PROGMEM = "vdd";
const char sys_get_hweui[] PROGMEM = "hweui";
const char sys_set_get_nvm[] PROGMEM = "nvm";
const char sys_set_pindig[] PROGMEM = "pindig";

const char* const sys_table[] PROGMEM = {sys_prefix,sys_sleep,sys_reset,sys_erase_fw,sys_factory_rst,sys_set,sys_get,sys_get_ver,sys_get_vdd,sys_get_hweui,sys_set_get_nvm,sys_set_pindig};

#define SYS_PREFIX 0
#define SYS_SLEEP 1
#define SYS_RESET 2
#define SYS_ERASE_FW 3
#define SYS_FACTORY_RST 4
#define SYS_SET 5
#define SYS_GET 6
#define SYS_GET_VER 7
#define SYS_GET_VDD 8
#define SYS_GET_HWEUI 9
#define SYS_SET_GET_NVM 10
#define SYS_SET_PINDIG 11

const char mac_prefix[] PROGMEM = "mac";
const char mac_reset[] PROGMEM = "reset";
const char mac_tx[] PROGMEM = "tx";
const char mac_join[] PROGMEM = "join";
const char mac_save[] PROGMEM = "save";
const char mac_force_enable[] PROGMEM = "forceENABLE";
const char mac_pause[] PROGMEM = "pause";
const char mac_resume[] PROGMEM = "resume";
const char mac_set[] PROGMEM = "set";
const char mac_get[] PROGMEM = "get";

const char* const mac_table[] PROGMEM = {mac_prefix,mac_reset,mac_tx,mac_join,mac_save,mac_force_enable,mac_pause,mac_resume,mac_set,mac_get};


#define MAC_PREFIX 0
#define MAC_RESET 1
#define MAC_TX 2
#define MAC_JOIN 3
#define MAC_SAVE 4
#define MAC_FORCE_ENABLE 5
#define MAC_PAUSE 6
#define MAC_RESUME 7
#define MAC_SET 8
#define MAC_GET 9

const char mac_set_devaddr[] PROGMEM = "devaddr";
const char mac_set_deveui[] PROGMEM = "deveui";
const char mac_set_appeui[] PROGMEM = "appeui";
const char mac_set_nwkskey[] PROGMEM = "nwkskey";
const char mac_set_appskey[] PROGMEM = "appskey";
const char mac_set_appkey[] PROGMEM = "appkey";
const char mac_set_pwridx[] PROGMEM = "pwridx";
const char mac_set_dr[] PROGMEM = "dr";
const char mac_set_adr[] PROGMEM = "adr";
const char mac_set_bat[] PROGMEM = "bat";
const char mac_set_retx[] PROGMEM = "retx";
const char mac_set_linkchk[] PROGMEM = "linkchk";
const char mac_set_rxdelay1[] PROGMEM = "rxdelay1";
const char mac_set_rxdelay2[] PROGMEM = "rxdelay2";
const char mac_set_band[] PROGMEM = "band";
const char mac_set_ar[] PROGMEM = "ar";
const char mac_set_rx2[] PROGMEM = "rx2";
const char mac_set_ch[] PROGMEM = "ch";

const char* const mac_set_options[] PROGMEM = {mac_set_devaddr,mac_set_deveui,mac_set_appeui,mac_set_nwkskey,mac_set_appskey,mac_set_appkey,mac_set_pwridx,mac_set_dr,mac_set_adr,mac_set_bat,mac_set_retx,mac_set_linkchk,mac_set_rxdelay1,mac_set_rxdelay2,mac_set_band,mac_set_ar,mac_set_rx2,mac_set_ch};

#define MAC_SET_DEVICEADDRESS 0
#define MAC_SET_DEVEUI 1
#define MAC_SET_APPEUI 2
#define MAC_SET_NWKSKEY 3
#define MAC_SET_APPSKEY 4
#define MAC_SET_APPKEY 5
#define MAC_SET_PWRIDX 6
#define MAC_SET_DR 7
#define MAC_SET_ADR 8
#define MAC_SET_BAT 9
#define MAC_SET_RETX 10
#define MAC_SET_LINKCHK 11
#define MAC_SET_RXDELAY1 12
#define MAC_SET_RXDELAY2 13
#define MAC_SET_BAND 14
#define MAC_SET_AR 15
#define MAC_SET_RX2 16
#define MAC_SET_CH 17

const char mac_join_mode_otaa[] PROGMEM = "otaa";
const char mac_join_mode_abp[] PROGMEM = "abp";

const char* const mac_join_mode[] PROGMEM = {mac_join_mode_otaa,mac_join_mode_abp};

#define MAC_JOIN_MODE_OTAA 0
#define MAC_JOIN_MODE_ABP 1

const char channel_dcycle[] PROGMEM = "dcycle";
const char channel_drrange[] PROGMEM = "drrange";
const char channel_freq[] PROGMEM = "freq";
const char channel_status[] PROGMEM = "status";

const char* const mac_ch_options[] PROGMEM = {channel_dcycle,channel_drrange,channel_freq,channel_status};

#define MAC_CHANNEL_DCYCLE 0
#define MAC_CHANNEL_DRRANGE 1
#define MAC_CHANNEL_FREQ 2
#define MAC_CHANNEL_STATUS 3

const char mac_tx_type_cnf[] PROGMEM = "cnf";
const char mac_tx_type_ucnf[] PROGMEM = "uncnf";

const char* const mac_tx_table[] PROGMEM = {mac_tx_type_cnf,mac_tx_type_ucnf};

#define MAC_TX_TYPE_CNF 0
#define MAC_TX_TYPE_UCNF 1

#define MAC_TABLE 0
#define MAC_GET_SET_TABLE 1
#define MAC_JOIN_TABLE 2
#define MAC_CH_TABLE 3
#define MAC_TX_TABLE 4
#define SYS_TABLE 5
#define RADIO_TABLE 6

char set_buffer[TTN_BUFFER_SIZE];

void memsetBuffer(size_t i) {
  for (size_t set = i; set--; ) {
    set_buffer[set] = 0;
  }
}

TheThingsNetwork::TheThingsNetwork(Stream& modemStream, Stream& debugStream, ttn_fp_t fp, uint8_t sf, uint8_t fsb) {
  this->debugStream = &debugStream;
  this->modemStream = &modemStream;
  this->fp = fp;
  this->sf = sf;
  this->fsb = fsb;
}

const char *TheThingsNetwork::readLine() {
  char get_bytes[TTN_BUFFER_SIZE];
  char get_buffer[TTN_BUFFER_SIZE];
  for (size_t i = TTN_BUFFER_SIZE; i--; ) {
    get_buffer[i] = '\0';
    get_bytes[i] = '\0';
  }
  while (true) {
    size_t length = modemStream->readBytesUntil('\n', get_bytes, sizeof(get_bytes));
    if (length > 0) {
      for (size_t i = 0; i < (length - 1); i++) {
        get_buffer[i] = get_bytes[i];
      }
      get_buffer[length] = '\0';
      char *line = get_buffer;
      delay(1);
      return (line);
    }
  }
}

const char *TheThingsNetwork::readValue(uint8_t prefixTable, uint8_t index) {
  while (Serial1.available()) {
    Serial1.read();
  }
  sendCommand(prefixTable, 0, true, false);
  sendCommand(prefixTable, index, false, false);
  Serial1.write(SEND_MSG);
  return readLine();
}

const char *TheThingsNetwork::readValue(uint8_t prefixTable, uint8_t indexTable, uint8_t index) {
  while (Serial1.available()) {
    Serial1.read();
  }
  sendCommand(prefixTable, 0, true, false);
  sendCommand(MAC_TABLE, MAC_GET, true, false);
  sendCommand(indexTable, index, false, false);
  Serial1.write(SEND_MSG);
  return readLine();
}

void TheThingsNetwork::reset(bool adr) {
  char *version = readValue(SYS_TABLE, SYS_RESET);
  model = subString(version, 0, 6);
  debugPrint(F("Version is "));
  debugPrint(subString(version, 7));
  debugPrint(F(", model is "));
  debugPrintLn(model);

  char *devEui = readValue(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI);
  sendMacSet(MAC_SET_DEVEUI, devEui);
  if(adr){
    sendMacSet(MAC_SET_ADR, ON);
  } else {
    sendMacSet(MAC_SET_ADR, OFF);
  }
}

void TheThingsNetwork::onMessage(void (*cb)(const byte* payload, size_t length, port_t port)) {
  this->messageCallback = cb;
}

bool TheThingsNetwork::personalize(const char *devAddr, const char *nwkSKey, const char *appSKey) {
  reset();
  sendMacSet(MAC_SET_DEVICEADDRESS, devAddr);
  sendMacSet(MAC_SET_NWKSKEY, nwkSKey);
  sendMacSet(MAC_SET_APPSKEY, appSKey);
  return personalize();
}

bool TheThingsNetwork::personalize() {
  configureChannels(this->sf, this->fsb);
  sendJoinSet(MAC_JOIN_MODE_ABP);
  char *response = readLine();
  if (!compareStrings(response, ACCEPTED)) {
    errMessage(PERSONALIZE_NOT_ACCEPTED, response);
    return false;
  }

  fillAirtimeInfo();
  debugPrint(F("Personalize accepted. Status: "));
  debugPrintLn(readValue(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS));
  return true;
}

bool TheThingsNetwork::provision(const char *appEui, const char *appKey) {
  sendMacSet(MAC_SET_APPEUI, appEui);
  sendMacSet(MAC_SET_APPKEY, appKey);
  debugPrint(SENDING);
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SAVE, false);
  Serial1.write(SEND_MSG);
  debugPrintLn();
  return true;
}

bool TheThingsNetwork::join(int8_t retries, uint32_t retryDelay) {
  configureChannels(this->sf, this->fsb);
  char *devEui = readValue(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI);
  sendMacSet(MAC_SET_DEVEUI, devEui);
  while (--retries) {
    if (!sendJoinSet(MAC_JOIN_MODE_OTAA)) {
      errMessage(JOIN_FAILED);
      delay(retryDelay);
      continue;
    }
    char *response = readLine();
    if (!compareStrings(response, ACCEPTED)) {
      errMessage(JOIN_NOT_ACCEPTED, response);
      delay(retryDelay);
      continue;
    }
    debugPrint(F("Join accepted. Status: "));
    debugPrintLn(readValue(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS));
    debugPrint(F("DevAddr: "));
    debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_DEVICEADDRESS));
    fillAirtimeInfo();
    return true;
  }
  return false;
}

bool TheThingsNetwork::join(const char *appEui, const char *appKey, int8_t retries, uint32_t retryDelay) {
  reset();
  provision(appEui, appKey);
  return join(retries, retryDelay);
}

int8_t TheThingsNetwork::sendBytes(const byte* payload, size_t length, port_t port, bool confirm) {
  bool send;
  if (confirm) {
    send = sendPayload(MAC_TX_TYPE_CNF, port, (uint8_t *)payload, length);
  } else {
    send = sendPayload(MAC_TX_TYPE_UCNF, port, (uint8_t *)payload, length);
  }
  if (!send) {
    errMessage(SEND_COMMAND_FAILED);
    return -1;
  }

  char *response = readLine();
  float i = this->airtime;
  trackAirtime(length);
  if (!isnan(i) && !isinf(i) && !isnan(this->airtime) && !isinf(this->airtime)) {
    debugPrint(F("Airtime added: "));
    debugPrint(this->airtime - i);
    debugPrintLn(" s");
    debugPrint(F("Total airtime: "));
    debugPrint(this->airtime);
    debugPrintLn(F(" s"));
  }
  if (compareStrings(response, MAC_TX_OK)) {
    debugPrintLn(F("Successful transmission"));
    return 1;
  }
  if (compareStrings(response, MAC_RX, 5)) {
    port_t downlinkPort = receivedPort(response, 7);
    char *data = subString(response, (8 + portLength(downlinkPort)));
    size_t downlinkLength = bufLength(data) / 2;
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

  errMessage(UNEXPECTED_RESPONSE, response);
  return -10;
}

size_t TheThingsNetwork::bufLength(char *data) {
  size_t length = 0;
  while (data[length] != '\0') {
   length++;
  }
  return length;
}

size_t TheThingsNetwork::portLength(size_t port) {
  size_t length = 0;
  while (port > 0) {
    port = port / 10;
    length += 1;
  }
  return length;
}

const char *TheThingsNetwork::subString(char *response, size_t start, size_t end) {
  char sub[TTN_BUFFER_SIZE];
  for (size_t l = TTN_BUFFER_SIZE; l--; ) {
    sub[l] = '\0';
  }
  size_t i;
  char *newString;
  if (end != -2)
    response[end] = '\0';
  for (i = 0; response[start] != '\0'; start++, i++) {
    sub[i] = response[start];
  }
  sub[i] = '\0';
  delay(1);
  newString = sub;
  delay(1);
  return newString;
}

uint8_t TheThingsNetwork::receivedPort(const char *response, size_t length) {
  uint16_t port = 0;
  while (response[length] != ' ') {
    port = (port + (response[length] - 48)) * 10;
    length += 1;
  }
  port = port / 10;
  return port;
}

int8_t TheThingsNetwork::poll(port_t port, bool confirm) {
  byte payload[] = { 0x00 };
  return sendBytes(payload, 1, port, confirm);
}

void TheThingsNetwork::fillAirtimeInfo() {
  this->info = {0, 0, 0, 0, 0, 0};

  char *message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_SF);
  this->info.sf = message[3] ? (message[2] - 48) * 10 + message[3] - 48 : message[2] - 48;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_BW);
  this->info.band = (message[0] - 48) * 100 + (message[1] - 48) * 10 + message[2] - 48;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_PRLEN);
  this->info.ps = this->info.ps + message[0] - 48;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_CRC);
  this->info.header = compareStrings(message, ON) ? 1 : 0;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_CR);
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
  debugPrintLn(readValue(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI));
  debugPrint(F("Battery: "));
  debugPrintLn(readValue(SYS_TABLE, SYS_TABLE, SYS_GET_VDD));
  debugPrint(F("AppEUI: "));
  debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_APPEUI));
  debugPrint(F("DevEUI: "));
  debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_DEVEUI));

  if (this->model == F("RN2483")) {
    debugPrint(F("Band: "));
    debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_BAND));
  }
  debugPrint(F("Data Rate: "));
  debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_DR));
  debugPrint(F("RX Delay 1: "));
  debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_RXDELAY1));
  debugPrint(F("RX Delay 2: "));
  debugPrintLn(readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_RXDELAY2));
  debugPrint(F("Total airtime: "));
  debugPrint(this->airtime);
  debugPrintLn(F(" s"));
}

void TheThingsNetwork::configureEU868(uint8_t sf) {
  uint8_t ch;
  char dr[1];
  uint32_t freq = 867100000;

  uint32_t tmp;
  size_t length = 8;
  char buf[length + 1];
  buf[length + 1] = '\0';

  sendMacSet(MAC_SET_RX2, "3 869525000");
  sendChSet(MAC_CHANNEL_DRRANGE, 1, "0 6");
  for (ch = 0; ch < 8; ch++) {
   sendChSet(MAC_CHANNEL_DCYCLE, ch, "799");
    if (ch > 2) {
      size_t length = 8;
      tmp = freq;
      while (tmp > 0) {
        buf[length] = (tmp % 10) + 48;
        tmp = tmp / 10;
        length -= 1;
      }
      sendChSet(MAC_CHANNEL_FREQ, ch, buf);
      sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 5");
      sendChSet(MAC_CHANNEL_STATUS, ch, ON);
      freq = freq + 200000;
    }
  }
  sendMacSet(MAC_SET_PWRIDX, TTN_PWRIDX_868);
  switch (sf) {
    case 7:
      dr[0] = '5';
      break;
    case 8:
      dr[0] = '4';
      break;
    case 9:
      dr[0] = '3';
      break;
    case 10:
      dr[0] = '2';
      break;
    case 11:
      dr[0] = '1';
      break;
    case 12:
      dr[0] = '0';
      break;
    default:
      errMessage(INVALID_SF);
      break;
  }
  dr[1] = '\0';
  if (dr[0] >= '0' && dr[0] <= '5'){
    sendMacSet(MAC_SET_DR, dr);
  }
}

void TheThingsNetwork::configureUS915(uint8_t sf, uint8_t fsb) {
  uint8_t ch;
  char dr[1];
  uint8_t chLow = fsb > 0 ? (fsb - 1) * 8 : 0;
  uint8_t chHigh = fsb > 0 ? chLow + 7 : 71;
  uint8_t ch500 = fsb + 63;

  sendMacSet(MAC_SET_PWRIDX, TTN_PWRIDX_915);
  for (ch = 0; ch < 72; ch++) {
    if (ch == ch500 || ch <= chHigh && ch >= chLow) {
      sendChSet(MAC_CHANNEL_STATUS, ch, ON);
      if (ch < 63) {
        sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 3");
      }
    } else {
      sendChSet(MAC_CHANNEL_STATUS, ch, OFF);
    }
  }
  switch (sf) {
    case 7:
      dr[0] = '3';
      break;
    case 8:
      dr[0] = '2';
      break;
    case 9:
      dr[0] = '1';
      break;
    case 10:
      dr[0] = '0';
      break;
    default:
      errMessage(INVALID_SF);
      break;
  }
  dr[1] = '\0';
  if (dr[0] >= '0' && dr[0] < '4'){
    sendMacSet(MAC_SET_DR, dr);
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
      errMessage(INVALID_FP);
      break;
  }
  sendMacSet(MAC_SET_RETX, TTN_RETX);
}

void TheThingsNetwork::sendCommand(uint8_t table, uint8_t index, bool with_space, bool print) {
  switch(table) {
    case MAC_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(mac_table[index])));
      break;
    case MAC_GET_SET_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(mac_set_options[index])));
      break;
    case MAC_JOIN_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(mac_join_mode[index])));
      break;
    case MAC_CH_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(mac_ch_options[index])));
      break;
    case MAC_TX_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(mac_tx_table[index])));
      break;
    case SYS_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(sys_table[index])));
      break;
    case RADIO_TABLE:
      strcpy_P(set_buffer, (char *)pgm_read_word(&(radio_table[index])));
      break;
    default:
      return ;
      break;
  }

  Serial1.write(set_buffer);
  if (with_space) {
    Serial1.write(" ");
  }
  if (print) {
    debugPrint(set_buffer);
    debugPrint(F(" "));
  }
}

bool TheThingsNetwork::sendMacSet(uint8_t index, const char* setting) {
  while (Serial1.available()) {
    Serial1.read();
  }
  debugPrint(SENDING);
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SET, true);
  sendCommand(MAC_GET_SET_TABLE, index, true);
  Serial1.write(setting);
  debugPrint(setting);
  Serial1.write(SEND_MSG);
  debugPrintLn();

  return waitForOk();
}

bool TheThingsNetwork::waitForOk() {
  memsetBuffer(TTN_BUFFER_SIZE);
  uint16_t responseLength = Serial1.readBytesUntil('\n',set_buffer, TTN_BUFFER_SIZE);
  if (responseLength >= 5) {
    if (!compareStrings(set_buffer, ACCEPTED)) {
      errMessage(RESPONSE_IS_NOT_OK, set_buffer);
      return false;
    }
  }
  else if (responseLength >= 2) {
    if (!compareStrings(set_buffer, OK)) {
      errMessage(RESPONSE_IS_NOT_OK, set_buffer);
      return false;
    }
  }
  return true;
}

bool TheThingsNetwork::sendChSet(uint8_t index, uint8_t channel, const char* setting) {
  while(Serial1.available()) {
    Serial1.read();
  }
  char ch[5];
  if (channel > 9) {
    ch[0] = ((channel - (channel % 10)) / 10) + 48;
    ch[1] = (channel % 10) + 48;
  } else {
    ch[0] = channel + 48;
    ch[1] = '\0';
  }
  ch[2] = '\0';
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX,true);
  sendCommand(MAC_TABLE, MAC_SET,true);
  sendCommand(MAC_GET_SET_TABLE, MAC_SET_CH,true);
  sendCommand(MAC_CH_TABLE, index, true);
  Serial1.write(ch);
  Serial1.write(" ");
  Serial1.write(setting);
  Serial1.write(SEND_MSG);
  debugPrint(channel);
  debugPrint(F(" "));
  debugPrintLn(setting);
  return waitForOk();
}

bool TheThingsNetwork::sendJoinSet(uint8_t type) {
  while (Serial1.available()) {
    Serial1.read();
  }
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_JOIN, true);
  sendCommand(MAC_JOIN_TABLE, type, false);
  Serial1.write(SEND_MSG);
  debugPrintLn();
  return waitForOk();
}

bool TheThingsNetwork::sendPayload(uint8_t mode, uint8_t port, uint8_t* payload, size_t len) {
  while(Serial1.available()) {
    Serial1.read();
  }
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX,true);
  sendCommand(MAC_TABLE, MAC_TX,true);
  sendCommand(MAC_TX_TABLE, mode,true);
  char newPort[5];
  if (port > 99) {
    newPort[0] = ((port - (port % 100)) / 100) + 48;
    newPort[1] = (((port % 100) - (port % 10)) / 10) + 48;
    newPort[2] = (port % 10) + 48;
  } else if (port > 9) {
    newPort[0] = ((port - (port % 10)) / 10) + 48;
    newPort[1] = (port % 10) + 48;
    newPort[2] = '\0';
  } else {
    newPort[0] = port + 48;
    newPort[1] = '\0';
  }
  newPort[3] = '\0';
  Serial1.write(newPort);
  debugPrint(port);
  debugPrint(F(" "));
  Serial1.print(" ");
  uint8_t i = 0;
  for(i=0;i<len;i++) {
    if(payload[i] ==0) {
      Serial1.print("00");
      debugPrint(F("00"));
    } else if(payload[i] < 16) {
      Serial1.print("0");
      debugPrint(F("0"));
      Serial1.print(payload[i],HEX);
      debugPrint(payload[i], HEX);
    } else {
      Serial1.print(payload[i],HEX);
      debugPrint(payload[i], HEX);
    }
  }
  Serial1.write(SEND_MSG);
  debugPrintLn();
  return waitForOk();
}

bool TheThingsNetwork::compareStrings(const char *str1, const char *str2, size_t length) {
  size_t one = 0;
  while (str1[one] != '\0' && str2[one] != '\0') {
    if (str1[one] != str2[one]) {
      return false;
    }
    one = one + 1;
    if (one == length) {
      return true;
    }
  }
  if (str1[one] == '\r' && str2[one] == '\0') {
    return true;
  }
  else if (str1[one] != str2[one]) {
    return false;
  }
  return true;
}

void TheThingsNetwork::errMessage(const char *err) {
  debugPrintLn(err);
}

void TheThingsNetwork::errMessage(const char *err, char *errMsg) {
  debugPrint(err);
  debugPrintLn(errMsg);
}
