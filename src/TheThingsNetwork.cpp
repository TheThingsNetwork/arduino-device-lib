// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <TheThingsNetwork.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

#define TTN_HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define TTN_HEX_PAIR_TO_BYTE(h, l) ((TTN_HEX_CHAR_TO_NIBBLE(h) << 4) + TTN_HEX_CHAR_TO_NIBBLE(l))

const char ok[] PROGMEM = "ok";
const char on[] PROGMEM = "on";
const char off[] PROGMEM = "off";
const char accepted[] PROGMEM = "accepted";
const char mac_tx_ok[] PROGMEM = "mac_tx_ok";
const char mac_rx[] PROGMEM = "mac_rx";
const char rn2483[] PROGMEM = "RN2483";

const char* const compare_table[] PROGMEM = {ok,on,off,accepted,mac_tx_ok,mac_rx,rn2483};

#define CMP_OK 0
#define CMP_ON 1
#define CMP_OFF 2
#define CMP_ACCEPTED 3
#define CMP_MAC_TX_OK 4
#define CMP_MAC_RX 5
#define CMP_RN2483 6

#define SENDING "Sending: "
#define SEND_MSG "\r\n"

const char eui[] PROGMEM = "EUI: ";
const char battery[] PROGMEM = "Battery: ";
const char appEui[] PROGMEM = "AppEui: ";
const char devEui[] PROGMEM = "DevEui: ";
const char band[] PROGMEM = "Band: ";
const char data_rate[] PROGMEM = "Data Rate: ";
const char rx_delay_1[] PROGMEM = "RX Delay 1: ";
const char rx_delay_2[] PROGMEM = "RX Delay 2: ";
const char total_airtime[] PROGMEM = "Total Airtime: ";
const char airtime_added[] PROGMEM = "Airtime added: ";
const char version[] PROGMEM = "Version is ";
const char model[] PROGMEM = ", model is ";

const char* const show_table[] PROGMEM = {eui,battery,appEui,devEui,band,data_rate,rx_delay_1,rx_delay_2,total_airtime,airtime_added,version,model};

#define SHOW_EUI 0
#define SHOW_BATTERY 1
#define SHOW_APPEUI 2
#define SHOW_DEVEUI 3
#define SHOW_BAND 4
#define SHOW_DATA_RATE 5
#define SHOW_RX_DELAY_1 6
#define SHOW_RX_DELAY_2 7
#define SHOW_TOTAL_AIRTIME 8
#define SHOW_AIRTIME_ADDED 9
#define SHOW_VERSION 10
#define SHOW_MODEL 11

const char invalid_sf[] PROGMEM = "Invalid SF";
const char invalid_fp[] PROGMEM = "Invalid frequency plan";
const char unexpected_response[] PROGMEM = "Unexpected response: ";
const char send_command_failed[] PROGMEM = "Send command failed";
const char join_failed[] PROGMEM = "Send join command failed";
const char join_not_accepted[] PROGMEM = "Join not accepted: ";
const char personalize_not_accepted[] PROGMEM = "Personalize not accepted";
const char response_is_not_ok[] PROGMEM = "Response is not OK: ";
const char error_key_length[] PROGMEM = "One or more keys are of invalid length.";
const char check_configuration[] PROGMEM = "Check your coverage, keys and backend status.";

const char* const error_msg[] PROGMEM = {invalid_sf,invalid_fp,unexpected_response,send_command_failed,join_failed,join_not_accepted,personalize_not_accepted,response_is_not_ok,error_key_length,check_configuration};

#define ERR_INVALID_SF 0
#define ERR_INVALID_FP 1
#define ERR_UNEXPECTED_RESPONSE 2
#define ERR_SEND_COMMAND_FAILED 3
#define ERR_JOIN_FAILED 4
#define ERR_JOIN_NOT_ACCEPTED 5
#define ERR_PERSONALIZE_NOT_ACCEPTED 6
#define ERR_RESPONSE_IS_NOT_OK 7
#define ERR_KEY_LENGTH 8
#define ERR_CHECK_CONFIGURATION 9

const char personalize_accepted[] PROGMEM = "Personalize accepted. Status: ";
const char join_accepted[] PROGMEM = "Join accepted. Status: ";
const char successful_transmission[] PROGMEM = "Successful transmission";
const char successful_transmission_received[] PROGMEM = "Successful transmission. Received ";

const char* const success_msg[] PROGMEM = {personalize_accepted,join_accepted,successful_transmission,successful_transmission_received};

#define SCS_PERSONALIZE_ACCEPTED 0
#define SCS_JOIN_ACCEPTED 1
#define SCS_SUCCESSFUL_TRANSMISSION 2
#define SCS_SUCCESSFUL_TRANSMISSION_RECEIVED 3

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
#define ERR_MESSAGE 7
#define SUCCESS_MESSAGE 8

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
  while (modemStream->available()) {
    modemStream->read();
  }
  sendCommand(prefixTable, 0, true, false);
  sendCommand(prefixTable, index, false, false);
  modemStream->write(SEND_MSG);
  return readLine();
}

const char *TheThingsNetwork::readValue(uint8_t prefixTable, uint8_t indexTable, uint8_t index) {
  while (modemStream->available()) {
    modemStream->read();
  }
  sendCommand(prefixTable, 0, true, false);
  sendCommand(MAC_TABLE, MAC_GET, true, false);
  sendCommand(indexTable, index, false, false);
  modemStream->write(SEND_MSG);
  return readLine();
}

void TheThingsNetwork::reset(bool adr) {
  const char *version = readValue(SYS_TABLE, SYS_RESET);
  valueToShow(SHOW_VERSION, subString(version, 7));
  const char *model = subString(version, 0, 6);
  this->model_EU = compareStrings(model, CMP_RN2483) ? true : false;
  valueToShow(SHOW_MODEL, model);

  const char *devEui = readValue(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI);
  sendMacSet(MAC_SET_DEVEUI, devEui);
  if(adr){
    sendMacSet(MAC_SET_ADR, "on");
  } else {
    sendMacSet(MAC_SET_ADR, "off");
  }
}

void TheThingsNetwork::onMessage(void (*cb)(const byte* payload, size_t length, port_t port)) {
  this->messageCallback = cb;
}

bool TheThingsNetwork::personalize(const char *devAddr, const char *nwkSKey, const char *appSKey) {
  reset();
  if (bufLength(devAddr) == 8 && bufLength(appSKey) == 32 && bufLength(nwkSKey) == 32) {
    sendMacSet(MAC_SET_DEVICEADDRESS, devAddr);
    sendMacSet(MAC_SET_NWKSKEY, nwkSKey);
    sendMacSet(MAC_SET_APPSKEY, appSKey);
    return personalize();
  }
  stateMessage(ERR_MESSAGE, ERR_KEY_LENGTH);
  return false;
}

bool TheThingsNetwork::personalize() {
  configureChannels(this->sf, this->fsb);
  sendJoinSet(MAC_JOIN_MODE_ABP);
  const char *response = readLine();
  if (!compareStrings(response, CMP_ACCEPTED)) {
    stateMessage(ERR_MESSAGE, ERR_PERSONALIZE_NOT_ACCEPTED, response);
    stateMessage(ERR_MESSAGE, ERR_CHECK_CONFIGURATION);
    return false;
  }

  fillAirtimeInfo();
  stateMessage(SUCCESS_MESSAGE, SCS_PERSONALIZE_ACCEPTED, readValue(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS));
  return true;
}

bool TheThingsNetwork::provision(const char *appEui, const char *appKey) {
  if (bufLength(appEui) == 16 && bufLength(appKey) == 32) {
    sendMacSet(MAC_SET_APPEUI, appEui);
    sendMacSet(MAC_SET_APPKEY, appKey);
    debugPrint(SENDING);
    sendCommand(MAC_TABLE, MAC_PREFIX, true);
    sendCommand(MAC_TABLE, MAC_SAVE, false);
    modemStream->write(SEND_MSG);
    debugPrintLn();
    return true;
  }
  stateMessage(ERR_MESSAGE, ERR_KEY_LENGTH);
  return false;
}

bool TheThingsNetwork::join(int8_t retries, uint32_t retryDelay) {
  configureChannels(this->sf, this->fsb);
  int8_t nbr_retries = retries;
  const char *devEui = readValue(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI);
  sendMacSet(MAC_SET_DEVEUI, devEui);
  while (--retries) {
    if (!sendJoinSet(MAC_JOIN_MODE_OTAA)) {
      stateMessage(ERR_MESSAGE, ERR_JOIN_FAILED);
      if ((nbr_retries - retries) >= 3) {
        stateMessage(ERR_MESSAGE, ERR_CHECK_CONFIGURATION);
      }
      delay(retryDelay);
      continue;
    }
    const char *response = readLine();
    if (!compareStrings(response, CMP_ACCEPTED)) {
      stateMessage(ERR_MESSAGE, ERR_JOIN_NOT_ACCEPTED, response);
       if ((nbr_retries - retries) >= 3) {
         stateMessage(ERR_MESSAGE, ERR_CHECK_CONFIGURATION);
      }
      delay(retryDelay);
      continue;
    }
    stateMessage(SUCCESS_MESSAGE, SCS_JOIN_ACCEPTED, readValue(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS));
    valueToShow(SHOW_DEVEUI, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_DEVEUI));
    fillAirtimeInfo();
    return true;
  }
  return false;
}

bool TheThingsNetwork::join(const char *appEui, const char *appKey, int8_t retries, uint32_t retryDelay) {
  reset();
  if (!provision(appEui, appKey)) {
    return false;
  }
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
    stateMessage(ERR_MESSAGE, ERR_SEND_COMMAND_FAILED);
    return -1;
  }

  const char *response = readLine();
  float i = this->airtime;
  trackAirtime(length);
  if (!isnan(i) && !isinf(i) && !isnan(this->airtime) && !isinf(this->airtime)) {
    valueToShow(SHOW_AIRTIME_ADDED);
    debugPrint(this->airtime - i);
    debugPrintLn(F(" s"));
    valueToShow(SHOW_TOTAL_AIRTIME);
  }
  if (compareStrings(response, CMP_MAC_TX_OK)) {
    stateMessage(SUCCESS_MESSAGE, SCS_SUCCESSFUL_TRANSMISSION);
    return 1;
  }
  if (compareStrings(response, CMP_MAC_RX, 5)) {
    port_t downlinkPort = receivedPort(response, 7);
    const char *data = subString(response, (8 + portLength(downlinkPort)));
    size_t downlinkLength = bufLength(data) / 2;
    byte downlink[64];
    for (size_t i = 0, d = 0; i < downlinkLength; i++, d += 2) {
      downlink[i] = TTN_HEX_PAIR_TO_BYTE(data[d], data[d+1]);
    }
    stateMessage(SUCCESS_MESSAGE, SCS_SUCCESSFUL_TRANSMISSION_RECEIVED, (const char *)itoa(downlinkLength, set_buffer, 10));
    if (this->messageCallback)
      this->messageCallback(downlink, downlinkLength, downlinkPort);
    return 2;
  }

  stateMessage(ERR_MESSAGE, ERR_UNEXPECTED_RESPONSE, response);
  return -10;
}

size_t TheThingsNetwork::bufLength(const char *data) {
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

const char *TheThingsNetwork::subString(const char *response, int16_t start, int16_t end) {
  char sub[TTN_BUFFER_SIZE];
  for (size_t l = TTN_BUFFER_SIZE; l--; ) {
    sub[l] = '\0';
  }
  char *newString;
  for (size_t i = 0; (end == -2 && response[start] != '\0') || start < end; start++, i++) {
    sub[i] = response[start];
  }
  sub[start] = '\0';
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

  const char *message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_SF);
  this->info.sf = message[3] ? (message[2] - 48) * 10 + message[3] - 48 : message[2] - 48;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_BW);
  this->info.band = (message[0] - 48) * 100 + (message[1] - 48) * 10 + message[2] - 48;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_PRLEN);
  this->info.ps = this->info.ps + message[0] - 48;

  message = readValue(RADIO_TABLE, RADIO_TABLE, RADIO_GET_CRC);
  this->info.header = compareStrings(message, CMP_ON) ? 1 : 0;

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
  valueToShow(SHOW_EUI, readValue(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI));
  valueToShow(SHOW_BATTERY, readValue(SYS_TABLE, SYS_TABLE, SYS_GET_VDD));
  valueToShow(SHOW_APPEUI, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_APPEUI));
  valueToShow(SHOW_DEVEUI, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_DEVEUI));

  if (this->model_EU) {
    valueToShow(SHOW_BAND, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_BAND));
  }
  valueToShow(SHOW_DATA_RATE, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_DR));
  valueToShow(SHOW_RX_DELAY_1, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_RXDELAY1));
  valueToShow(SHOW_RX_DELAY_2, readValue(MAC_TABLE, MAC_GET_SET_TABLE, MAC_SET_RXDELAY2));
  valueToShow(SHOW_TOTAL_AIRTIME);
}

void TheThingsNetwork::valueToShow(uint8_t index, const char *value) {
  char buffer[100];
  strcpy_P(buffer, (char *)pgm_read_word(&(show_table[index])));
  debugPrint(buffer);
  if (value && index == SHOW_VERSION) {
    debugPrint(value);
  } else if (value) {
    debugPrintLn(value);
  } else if (index == SHOW_TOTAL_AIRTIME) {
    debugPrint(this->airtime);
    debugPrintLn(F(" s"));
  }
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
      sendChSet(MAC_CHANNEL_STATUS, ch, "on");
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
      stateMessage(ERR_MESSAGE, ERR_INVALID_SF);
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
    if (ch == ch500 || (ch <= chHigh && ch >= chLow)) {
      sendChSet(MAC_CHANNEL_STATUS, ch, "on");
      if (ch < 63) {
        sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 3");
      }
    } else {
      sendChSet(MAC_CHANNEL_STATUS, ch, "off");
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
      stateMessage(ERR_MESSAGE, ERR_INVALID_SF);
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
      stateMessage(ERR_MESSAGE, ERR_INVALID_FP);
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

  modemStream->write(set_buffer);
  if (with_space) {
    modemStream->write(" ");
  }
  if (print) {
    debugPrint(set_buffer);
    debugPrint(F(" "));
  }
}

bool TheThingsNetwork::sendMacSet(uint8_t index, const char* setting) {
  while (modemStream->available()) {
    modemStream->read();
  }
  debugPrint(SENDING);
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SET, true);
  sendCommand(MAC_GET_SET_TABLE, index, true);
  modemStream->write(setting);
  debugPrint(setting);
  modemStream->write(SEND_MSG);
  debugPrintLn();

  return waitForOk();
}

bool TheThingsNetwork::waitForOk() {
  memsetBuffer(TTN_BUFFER_SIZE);
  uint16_t responseLength = modemStream->readBytesUntil('\n',set_buffer, TTN_BUFFER_SIZE);
  if (responseLength >= 5) {
    if (!compareStrings(set_buffer, CMP_ACCEPTED)) {
      stateMessage(ERR_MESSAGE, ERR_RESPONSE_IS_NOT_OK, set_buffer);
      return false;
    }
  }
  else if (responseLength >= 2) {
    if (!compareStrings(set_buffer, CMP_OK)) {
      stateMessage(ERR_MESSAGE, ERR_RESPONSE_IS_NOT_OK, set_buffer);
      return false;
    }
  }
  return true;
}

bool TheThingsNetwork::sendChSet(uint8_t index, uint8_t channel, const char* setting) {
  while(modemStream->available()) {
    modemStream->read();
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
  modemStream->write(ch);
  modemStream->write(" ");
  modemStream->write(setting);
  modemStream->write(SEND_MSG);
  debugPrint(channel);
  debugPrint(F(" "));
  debugPrintLn(setting);
  return waitForOk();
}

bool TheThingsNetwork::sendJoinSet(uint8_t type) {
  while (modemStream->available()) {
    modemStream->read();
  }
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_JOIN, true);
  sendCommand(MAC_JOIN_TABLE, type, false);
  modemStream->write(SEND_MSG);
  debugPrintLn();
  return waitForOk();
}

bool TheThingsNetwork::sendPayload(uint8_t mode, uint8_t port, uint8_t* payload, size_t len) {
  while(modemStream->available()) {
    modemStream->read();
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
  modemStream->write(newPort);
  debugPrint(port);
  debugPrint(F(" "));
  modemStream->print(" ");
  uint8_t i = 0;
  for(i=0;i<len;i++) {
    if(payload[i] ==0) {
      modemStream->print("00");
      debugPrint(F("00"));
    } else if(payload[i] < 16) {
      modemStream->print("0");
      debugPrint(F("0"));
      modemStream->print(payload[i],HEX);
      debugPrint(payload[i], HEX);
    } else {
      modemStream->print(payload[i],HEX);
      debugPrint(payload[i], HEX);
    }
  }
  modemStream->write(SEND_MSG);
  debugPrintLn();
  return waitForOk();
}

bool TheThingsNetwork::compareStrings(const char *str1, uint8_t index, size_t length) {
  size_t one = 0;
  char str2[10];
  strcpy_P(str2, (char *)pgm_read_word(&(compare_table[index])));
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

void TheThingsNetwork::stateMessage(uint8_t type, uint8_t indexMsg, const char *output) {
  char buffer[50];
  switch (type) {
    case ERR_MESSAGE:
      strcpy_P(buffer, (char *)pgm_read_word(&(error_msg[indexMsg])));
      break;
    case SUCCESS_MESSAGE:
      strcpy_P(buffer, (char *)pgm_read_word(&(success_msg[indexMsg])));
      break;
  }
  debugPrint(buffer);
  if (type == SUCCESS_MESSAGE && indexMsg == SCS_SUCCESSFUL_TRANSMISSION_RECEIVED) {
    debugPrint(output);
    debugPrintLn(" bytes");
  } else if (output) {
    debugPrintLn(output);
  } else {
    debugPrintLn();
  }
}
