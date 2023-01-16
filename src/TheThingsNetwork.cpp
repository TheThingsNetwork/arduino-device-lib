// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include "TheThingsNetwork.h"

#define debugPrintLn(...)                \
  {                                      \
    if (debugStream)                     \
      debugStream->println(__VA_ARGS__); \
  }
#define debugPrint(...)                \
  {                                    \
    if (debugStream)                   \
      debugStream->print(__VA_ARGS__); \
  }

#define TTN_HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define TTN_HEX_PAIR_TO_BYTE(h, l) ((TTN_HEX_CHAR_TO_NIBBLE(h) << 4) + TTN_HEX_CHAR_TO_NIBBLE(l))

const char ok[] PROGMEM = "ok";
const char on[] PROGMEM = "on";
const char off[] PROGMEM = "off";
const char accepted[] PROGMEM = "accepted";
const char mac_tx_ok[] PROGMEM = "mac_tx_ok";
const char mac_rx[] PROGMEM = "mac_rx";
const char mac_err[] PROGMEM = "mac_err";
const char rn2483[] PROGMEM = "RN2483";
const char rn2483a[] PROGMEM = "RN2483A";
const char rn2903[] PROGMEM = "RN2903";
const char rn2903as[] PROGMEM = "RN2903AS";

const char *const compare_table[] PROGMEM = {ok, on, off, accepted, mac_tx_ok, mac_rx, mac_err, rn2483, rn2483a, rn2903, rn2903as};

#define CMP_OK 0
#define CMP_ON 1
#define CMP_OFF 2
#define CMP_ACCEPTED 3
#define CMP_MAC_TX_OK 4
#define CMP_MAC_RX 5
#define CMP_MAC_ERR 6
#define CMP_RN2483 7
#define CMP_RN2483A 8
#define CMP_RN2903 9
#define CMP_RN2903AS 10

// CMP OK
const char busy[] PROGMEM = "busy";
const char fram_counter_err_rejoin_needed[] PROGMEM = "fram_counter_err_rejoin_needed";
const char invalid_class[] PROGMEM = "invalid_class";
const char invalid_data_len[] PROGMEM = "invalid_data_len";
const char invalid_param[] PROGMEM = "invalid_param";
const char keys_not_init[] PROGMEM = "keys_not_init";
const char mac_paused[] PROGMEM = "mac_paused";
const char multicast_keys_not_set[] PROGMEM = "multicast_keys_not_set";
const char no_free_ch[] PROGMEM = "no_free_ch";
const char not_joined[] PROGMEM = "not_joined";
const char silent[] PROGMEM = "silent";
const char err[] PROGMEM = "err";

const char *const compareerr_table[] PROGMEM = {ok, busy, fram_counter_err_rejoin_needed, invalid_class, invalid_data_len, invalid_param, keys_not_init, mac_paused, multicast_keys_not_set, no_free_ch, not_joined, silent, err};

#define CMP_ERR_OK 0
#define CMP_ERR_BUSY 1
#define CMP_ERR_FRMCNT 2
#define CMP_ERR_INVCLS 3
#define CMP_ERR_INVDLEN 4
#define CMP_ERR_INVPAR 5
#define CMP_ERR_NKEYINT 6
#define CMP_ERR_MACPAUSE 7
#define CMP_ERR_NKYMLTCST 8
#define CMP_ERR_NFRCHN 9
#define CMP_ERR_NJOIN 10
#define CMP_ERR_SILENT 11
#define CMP_ERR_ERR 12

#define CMP_ERR_LAST CMP_ERR_ERR

#define SENDING "Sending: "
#define SEND_MSG "\r\n"

const char eui[] PROGMEM = "EUI: ";
const char battery[] PROGMEM = "Battery: ";
const char appEui[] PROGMEM = "AppEUI: ";
const char devEui[] PROGMEM = "DevEUI: ";
const char band[] PROGMEM = "Band: ";
const char data_rate[] PROGMEM = "Data Rate: ";
const char rx_delay_1[] PROGMEM = "RX Delay 1: ";
const char rx_delay_2[] PROGMEM = "RX Delay 2: ";
const char version[] PROGMEM = "Version: ";
const char model[] PROGMEM = "Model: ";
const char devaddr[] PROGMEM = "DevAddr: ";

const char *const show_table[] PROGMEM = {eui, battery, appEui, devEui, band, data_rate, rx_delay_1, rx_delay_2, version, model, devaddr};

#define SHOW_EUI 0
#define SHOW_BATTERY 1
#define SHOW_APPEUI 2
#define SHOW_DEVEUI 3
#define SHOW_BAND 4
#define SHOW_DATA_RATE 5
#define SHOW_RX_DELAY_1 6
#define SHOW_RX_DELAY_2 7
#define SHOW_VERSION 8
#define SHOW_MODEL 9
#define SHOW_DEVADDR 10

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
const char no_response[] PROGMEM =  "No response from RN module.";
const char invalid_module[] PROGMEM = "Invalid module (must be RN2xx3[xx]).";

const char *const error_msg[] PROGMEM = {invalid_sf, invalid_fp, unexpected_response, send_command_failed, join_failed, join_not_accepted, personalize_not_accepted, response_is_not_ok, error_key_length, check_configuration, no_response, invalid_module};

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
#define ERR_NO_RESPONSE 10
#define ERR_INVALID_MODULE 11

const char personalize_accepted[] PROGMEM = "Personalize accepted. Status: ";
const char join_accepted[] PROGMEM = "Join accepted. Status: ";
const char successful_transmission[] PROGMEM = "Successful transmission";
const char successful_transmission_received[] PROGMEM = "Successful transmission. Received ";
const char valid_module[] PROGMEM = "Valid module connected.";

const char *const success_msg[] PROGMEM = {personalize_accepted, join_accepted, successful_transmission, successful_transmission_received, valid_module};

#define SCS_PERSONALIZE_ACCEPTED 0
#define SCS_JOIN_ACCEPTED 1
#define SCS_SUCCESSFUL_TRANSMISSION 2
#define SCS_SUCCESSFUL_TRANSMISSION_RECEIVED 3
#define SCS_VALID_MODULE 4

const char radio_prefix[] PROGMEM = "radio";
const char radio_set[] PROGMEM = "set";
const char radio_get[] PROGMEM = "get";
const char radio_get_bw[] PROGMEM = "bw";
const char radio_get_prlen[] PROGMEM = "prlen";
const char radio_get_crc[] PROGMEM = "crc";
const char radio_get_cr[] PROGMEM = "cr";
const char radio_get_sf[] PROGMEM = "sf";
const char radio_get_freq[] PROGMEM = "freq";
const char radio_get_rxbw[] PROGMEM = "rxbw";
const char radio_get_wdt[] PROGMEM = "wdt";
const char radio_get_pwr[] PROGMEM = "pwr";
const char radio_get_rssi[] PROGMEM = "rssi";
const char radio_get_snr[] PROGMEM = "snr";

const char *const radio_table[] PROGMEM = {radio_prefix, radio_set, radio_get, radio_get_bw, radio_get_prlen, radio_get_crc, radio_get_cr, radio_get_sf, radio_get_freq, radio_get_rxbw, radio_get_wdt, radio_get_pwr, radio_get_rssi, radio_get_snr};

#define RADIO_PREFIX 0
#define RADIO_SET 1
#define RADIO_GET 2
#define RADIO_GET_BW 3
#define RADIO_GET_PRLEN 4
#define RADIO_GET_CRC 5
#define RADIO_GET_CR 6
#define RADIO_GET_SF 7
#define RADIO_GET_FREQ 8
#define RADIO_GET_RXBW 9
#define RADIO_GET_WDT 10
#define RADIO_GET_PWR 11
#define RADIO_GET_RSSI 12
#define RADIO_GET_SNR 13

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

const char *const sys_table[] PROGMEM = {sys_prefix, sys_sleep, sys_reset, sys_erase_fw, sys_factory_rst, sys_set, sys_get, sys_get_ver, sys_get_vdd, sys_get_hweui, sys_set_get_nvm, sys_set_pindig};

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

const char *const mac_table[] PROGMEM = {mac_prefix, mac_reset, mac_tx, mac_join, mac_save, mac_force_enable, mac_pause, mac_resume, mac_set, mac_get};

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

const char mac_devaddr[] PROGMEM = "devaddr";
const char mac_deveui[] PROGMEM = "deveui";
const char mac_appeui[] PROGMEM = "appeui";
const char mac_nwkskey[] PROGMEM = "nwkskey";
const char mac_appskey[] PROGMEM = "appskey";
const char mac_appkey[] PROGMEM = "appkey";
const char mac_pwridx[] PROGMEM = "pwridx";
const char mac_dr[] PROGMEM = "dr";
const char mac_adr[] PROGMEM = "adr";
const char mac_bat[] PROGMEM = "bat";
const char mac_retx[] PROGMEM = "retx";
const char mac_linkchk[] PROGMEM = "linkchk";
const char mac_rxdelay1[] PROGMEM = "rxdelay1";
const char mac_rxdelay2[] PROGMEM = "rxdelay2";
const char mac_band[] PROGMEM = "band";
const char mac_ar[] PROGMEM = "ar";
const char mac_rx2[] PROGMEM = "rx2";
const char mac_ch[] PROGMEM = "ch";
const char mac_gwnb[] PROGMEM = "gwnb";
const char mac_mrgn[] PROGMEM = "mrgn";
const char mac_class[] PROGMEM = "class";
const char mac_status[] PROGMEM = "status";
const char mac_upctr[] PROGMEM = "upctr";
const char mac_dnctr[] PROGMEM = "dnctr";

const char *const mac_options[] PROGMEM = {mac_devaddr, mac_deveui, mac_appeui, mac_nwkskey, mac_appskey, mac_appkey, mac_pwridx, mac_dr, mac_adr, mac_bat, mac_retx, mac_linkchk, mac_rxdelay1, mac_rxdelay2, mac_band,
			mac_ar, mac_rx2, mac_ch, mac_gwnb, mac_mrgn, mac_class, mac_status, mac_upctr, mac_dnctr};

#define MAC_DEVADDR 0
#define MAC_DEVEUI 1
#define MAC_APPEUI 2
#define MAC_NWKSKEY 3
#define MAC_APPSKEY 4
#define MAC_APPKEY 5
#define MAC_PWRIDX 6
#define MAC_DR 7
#define MAC_ADR 8
#define MAC_BAT 9
#define MAC_RETX 10
#define MAC_LINKCHK 11
#define MAC_RXDELAY1 12
#define MAC_RXDELAY2 13
#define MAC_BAND 14
#define MAC_AR 15
#define MAC_RX2 16
#define MAC_CH 17
#define MAC_GWNB 18
#define MAC_MRGN 19
#define MAC_CLASS 20
#define MAC_STATUS 21
#define MAC_UPCTR 22
#define MAC_DNCTR 23

const char mac_join_mode_otaa[] PROGMEM = "otaa";
const char mac_join_mode_abp[] PROGMEM = "abp";

const char *const mac_join_mode[] PROGMEM = {mac_join_mode_otaa, mac_join_mode_abp};

#define MAC_JOIN_MODE_OTAA 0
#define MAC_JOIN_MODE_ABP 1

const char channel_dcycle[] PROGMEM = "dcycle";
const char channel_drrange[] PROGMEM = "drrange";
const char channel_freq[] PROGMEM = "freq";
const char channel_status[] PROGMEM = "status";

const char *const mac_ch_options[] PROGMEM = {channel_dcycle, channel_drrange, channel_freq, channel_status};

#define MAC_CHANNEL_DCYCLE 0
#define MAC_CHANNEL_DRRANGE 1
#define MAC_CHANNEL_FREQ 2
#define MAC_CHANNEL_STATUS 3

const char mac_tx_type_cnf[] PROGMEM = "cnf";
const char mac_tx_type_ucnf[] PROGMEM = "uncnf";

const char *const mac_tx_table[] PROGMEM = {mac_tx_type_cnf, mac_tx_type_ucnf};

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
#define CMP_TABLE 9
#define CMP_ERR_TABLE 10

int pgmstrcmp(const char *str1, uint8_t str2Index, uint8_t table = CMP_TABLE)
{
  if (0 == strlen(str1))
	  return -1;

  char str2[128];

  switch (table) {
  case CMP_ERR_TABLE:
    strcpy_P(str2, (char *)pgm_read_word(&(compareerr_table[str2Index])));
    break;

  default:
  case CMP_TABLE:
    strcpy_P(str2, (char *)pgm_read_word(&(compare_table[str2Index])));
  }

  return memcmp(str1, str2, min(strlen(str1), strlen(str2)));
}

uint8_t digits(uint8_t port)
{
  if (port >= 100)
  {
    return 3;
  }
  else if (port >= 10)
  {
    return 2;
  }
  return 1;
}

uint8_t receivedPort(const char *s)
{
  uint8_t port = 0;
  uint8_t i = 0;
  while (s[i] != ' ' && s[i] != '\0')
  {
    port *= 10;
    port += s[i] - 48;
    i++;
  }
  return port;
}

TheThingsNetwork::TheThingsNetwork(Stream &modemStream, Stream &debugStream, ttn_fp_t fp, uint8_t sf, uint8_t fsb)
{
  this->debugStream = &debugStream;
  this->modemStream = &modemStream;
  this->modemStream->setTimeout(TTN_DEFAULT_TIMEOUT);
  this->fp = fp;
  this->sf = sf;
  this->fsb = fsb;
  this->adr = false;
  this->messageCallback = NULL;
}

size_t TheThingsNetwork::getAppEui(char *buffer, size_t size)
{
  return readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_APPEUI, buffer, size);
}

size_t TheThingsNetwork::getHardwareEui(char *buffer, size_t size)
{
  return readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI, buffer, size);
}

size_t TheThingsNetwork::getVersion(char *buffer, size_t size)
{
  return readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_VER, buffer, size);
}

uint16_t TheThingsNetwork::getVDD()
{
  if (readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_VDD, buffer, sizeof(buffer)) > 0) {
    return atoi(buffer);
  }
  return 0;
}

uint8_t TheThingsNetwork::getBW()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_BW, buffer, sizeof(buffer)) > 0) {
    return atoi(buffer);
  }
  return 0;
}

uint8_t TheThingsNetwork::getCR()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_CR, buffer, sizeof(buffer)) > 2) {
    return atoi(buffer+2); // skip the first two chars "4/"
  }
  return 0;
}

uint32_t TheThingsNetwork::getFrequency()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_FREQ, buffer, sizeof(buffer)) > 0) {
    return atol(buffer);
  }
  return 0;
}

uint32_t TheThingsNetwork::getFCU()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_UPCTR, buffer, sizeof(buffer)) > 0) {
    return strtoul(buffer, NULL, 10);
  }
  return 0;
}

uint32_t TheThingsNetwork::getFCD()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_DNCTR, buffer, sizeof(buffer)) > 0) {
    return strtoul(buffer, NULL, 10);
  }
  return 0;
}

uint32_t TheThingsNetwork::getWatchDogTimer()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_WDT, buffer, sizeof(buffer)) > 0) {
    return atol(buffer);
  }
  return 0;
}

enum ttn_modem_status_t TheThingsNetwork::getStatus()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_STATUS, buffer, sizeof(buffer)) > 0) {
	char **endptr = NULL;
	int status = (strtol(buffer, endptr, 16) & 0x0F); // Mask out only active status

	if (endptr == NULL)
		return (enum ttn_modem_status_t)status;
  }
  return TTN_MODEM_READ_ERR; // unable to read status
}

int8_t TheThingsNetwork::getPower()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_PWR, buffer, sizeof(buffer)) > 0) {
    return atoi(buffer);
  }
  return -128;
}

int16_t TheThingsNetwork::getRSSI()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_RSSI, buffer, sizeof(buffer)) > 0) {
    return atoi(buffer);
  }
  return -255;
}

int8_t TheThingsNetwork::getSNR()
{
  if (readResponse(RADIO_TABLE, RADIO_TABLE, RADIO_GET_SNR, buffer, sizeof(buffer)) > 0) {
    return atoi(buffer);
  }
  return -128;
}

int8_t TheThingsNetwork::getDR()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_DR, buffer, sizeof(buffer))){
    return atoi(buffer);
  }
  return -1;
}

int8_t TheThingsNetwork::getPowerIndex()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_PWRIDX, buffer, sizeof(buffer)) > 0) {
    return atoi(buffer);
  }
  return -1;
}

bool TheThingsNetwork::getChannelStatus (uint8_t channel)
{
  char str[5];
  if (channel > 9)
  {
	str[0] = ((channel - (channel % 10)) / 10) + 48;
	str[1] = (channel % 10) + 48;
	str[2] = '\0';
  }
  else
  {
	str[0] = channel + 48;
	str[1] = '\0';
  }
  sendCommand(MAC_TABLE, MAC_PREFIX, true, false);
  sendCommand(MAC_TABLE, MAC_GET, true, false); // default "get " in between (same as radio get)
  sendCommand(MAC_GET_SET_TABLE, MAC_CH, true, false);
  sendCommand(MAC_CH_TABLE, MAC_CHANNEL_STATUS, true, false);
  modemStream->write(str);
  modemStream->write(SEND_MSG);

  if (readLine(buffer, sizeof(buffer)))
	  return (pgmstrcmp(buffer, CMP_ON) == 0); // true if on, false if off or an error occurs
  else
	  return false; // error
}

ttn_response_code_t TheThingsNetwork::getLastError(){

	int match, pos;
	for (pos=0; pos <= CMP_ERR_LAST; pos++){
		match = pgmstrcmp(buffer, pos, CMP_ERR_TABLE);
		if (match == 0)
			break;
	}

	return (ttn_response_code_t)(-1* pos); // code order is equal
}

void TheThingsNetwork::debugPrintIndex(uint8_t index, const char *value)
{
  char message[100];
  strcpy_P(message, (char *)pgm_read_word(&(show_table[index])));
  debugPrint(message);
  if (value)
  {
    debugPrintLn(value);
  }
}

void TheThingsNetwork::debugPrintMessage(uint8_t type, uint8_t index, const char *value)
{
  char message[100];
  switch (type)
  {
  case ERR_MESSAGE:
    strcpy_P(message, (char *)pgm_read_word(&(error_msg[index])));
    break;
  case SUCCESS_MESSAGE:
    strcpy_P(message, (char *)pgm_read_word(&(success_msg[index])));
    break;
  }
  debugPrint(message);
  if (value)
  {
    debugPrintLn(value);
  }
  else
  {
    debugPrintLn();
  }
}

void TheThingsNetwork::clearReadBuffer()
{
  while (modemStream->available())
  {
    modemStream->read();
  }
}

size_t TheThingsNetwork::readLine(char *buffer, size_t size, uint8_t attempts)
{
  size_t read = 0;
  while (!read && attempts--)
  {
    read = modemStream->readBytesUntil('\n', buffer, size);
  }
  if (!read)
  { // If attempts is activated return 0 and set RN state marker
    this->needsHardReset = true; // Inform the application about the radio module is not responsive.
    debugPrintMessage(ERR_MESSAGE, ERR_NO_RESPONSE);
    return 0;
  }
  buffer[read - 1] = '\0'; // set \r to \0
  return read;
}

size_t TheThingsNetwork::readResponse(uint8_t prefixTable, uint8_t index, char *buffer, size_t size)
{
  clearReadBuffer();
  sendCommand(prefixTable, 0, true, false);
  sendCommand(prefixTable, index, false, false);
  modemStream->write(SEND_MSG);
  return readLine(buffer, size);
}

size_t TheThingsNetwork::readResponse(uint8_t prefixTable, uint8_t indexTable, uint8_t index, char *buffer, size_t size)
{
  clearReadBuffer();
  sendCommand(prefixTable, 0, true, false);
  sendCommand(MAC_TABLE, MAC_GET, true, false);
  sendCommand(indexTable, index, false, false);
  modemStream->write(SEND_MSG);
  return readLine(buffer, size);
}

void TheThingsNetwork::autoBaud()
{
  // Courtesy of @jpmeijers
  modemStream->setTimeout(2000);
  uint8_t attempts = 10;
  size_t length = 0;
  while (attempts-- && length == 0)
  {
    delay(100);
    modemStream->write((byte)0x00);
    modemStream->write(0x55);
    modemStream->write(SEND_MSG);
    sendCommand(SYS_TABLE, 0, true, false);
    sendCommand(SYS_TABLE, SYS_GET, true, false);
    sendCommand(SYS_TABLE, SYS_GET_VER, false, false);
    modemStream->write(SEND_MSG);
    length = modemStream->readBytesUntil('\n', buffer, sizeof(buffer));
  }
  delay(100);
  clearReadBuffer();
  modemStream->setTimeout(TTN_DEFAULT_TIMEOUT);
  baudDetermined = true;
}

void TheThingsNetwork::reset(bool adr)
{
  // autobaud and send "sys reset"
  autoBaud();
  readResponse(SYS_TABLE, SYS_RESET, buffer, sizeof(buffer));

  // autobaud (again, because baudrate was reset with "sys reset") and get HW model and SW version
  autoBaud();
  getVersion(buffer, sizeof(buffer));

  // buffer contains "RN2xx3[xx] x.x.x ...", splitting model from version
  char *model = strtok(buffer, " ");
  debugPrintIndex(SHOW_MODEL, model);
  char *version = strtok(NULL, " ");
  debugPrintIndex(SHOW_VERSION, version);

  // set DEVEUI as HWEUI
  readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI, buffer, sizeof(buffer));
  sendMacSet(MAC_DEVEUI, buffer);
  // set ADR
  setADR(adr);
}

void TheThingsNetwork::resetHard(uint8_t resetPin){
  digitalWrite(resetPin, LOW);
  delay(1000);
  digitalWrite(resetPin, HIGH);
}

void TheThingsNetwork::saveState()
{
  debugPrint(SENDING);
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SAVE, false);
  modemStream->write(SEND_MSG);
  debugPrintLn();
  waitForOk();
}

void TheThingsNetwork::onMessage(void (*cb)(const uint8_t *payload, size_t size, port_t port))
{
  messageCallback = cb;
}

bool TheThingsNetwork::personalize(const char *devAddr, const char *nwkSKey, const char *appSKey, bool resetFirst)
{
  if(resetFirst) {
    reset(adr);
  }
  if (strlen(devAddr) != 8 || strlen(appSKey) != 32 || strlen(nwkSKey) != 32)
  {
    debugPrintMessage(ERR_MESSAGE, ERR_KEY_LENGTH);
    return false;
  }
  sendMacSet(MAC_DEVADDR, devAddr);
  sendMacSet(MAC_NWKSKEY, nwkSKey);
  sendMacSet(MAC_APPSKEY, appSKey);
  return personalize();
}

bool TheThingsNetwork::personalize()
{
  configureChannels(fsb);
  setSF(sf);
  sendJoinSet(MAC_JOIN_MODE_ABP);
  readLine(buffer, sizeof(buffer));
  if (pgmstrcmp(buffer, CMP_ACCEPTED) != 0)
  {
    debugPrintMessage(ERR_MESSAGE, ERR_PERSONALIZE_NOT_ACCEPTED, buffer);
    debugPrintMessage(ERR_MESSAGE, ERR_CHECK_CONFIGURATION);
    return false;
  }

  readResponse(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS, buffer, sizeof(buffer));
  debugPrintMessage(SUCCESS_MESSAGE, SCS_PERSONALIZE_ACCEPTED, buffer);
  return true;
}

bool TheThingsNetwork::provision(const char *appEui, const char *appKey, bool resetFirst)
{
  if(resetFirst) {
    reset(adr);
  }
  if (strlen(appEui) != 16 || strlen(appKey) != 32)
  {
    debugPrintMessage(ERR_MESSAGE, ERR_KEY_LENGTH);
    return false;
  }
  readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI, buffer, sizeof(buffer));
  sendMacSet(MAC_DEVEUI, buffer);
  sendMacSet(MAC_APPEUI, appEui);
  sendMacSet(MAC_APPKEY, appKey);
  saveState();
  return true;
}

bool TheThingsNetwork::join(int8_t retries, uint32_t retryDelay)
{
  int8_t attempts = 0;
  configureChannels(fsb);
  setSF(sf);
  while (retries == -1 || attempts <= retries)
  {
    attempts++;
    if (!sendJoinSet(MAC_JOIN_MODE_OTAA))
    {
      debugPrintMessage(ERR_MESSAGE, ERR_JOIN_FAILED);
      delay(retryDelay);
      continue;
    }
    readLine(buffer, sizeof(buffer));
    if (pgmstrcmp(buffer, CMP_ACCEPTED) != 0)
    {
      debugPrintMessage(ERR_MESSAGE, ERR_JOIN_NOT_ACCEPTED, buffer);
      debugPrintMessage(ERR_MESSAGE, ERR_CHECK_CONFIGURATION);
      delay(retryDelay);
      continue;
    }
    readResponse(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS, buffer, sizeof(buffer));
    debugPrintMessage(SUCCESS_MESSAGE, SCS_JOIN_ACCEPTED, buffer);
    readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_DEVADDR, buffer, sizeof(buffer));
    debugPrintIndex(SHOW_DEVADDR, buffer);
    return true;
  }
  return false;
}

bool TheThingsNetwork::setClass(lorawan_class_t p_lw_class)
{
  switch(p_lw_class)
  {

  case CLASS_A:
    {
      lw_class = p_lw_class;
      return sendMacSet(MAC_CLASS, "a");
    }

  // case CLASS_B: // Not yet supported. Use default case.

  case CLASS_C:
    {
      bool result = sendMacSet(MAC_CLASS, "c");
      // Older firmware does not support Class C. If setting change fails, keep on using Class A.
      if(result) lw_class = p_lw_class;
      return result;
    }

  default:
    return false;

  }
}

bool TheThingsNetwork::join(const char *appEui, const char *appKey, int8_t retries, uint32_t retryDelay, lorawan_class_t p_lw_class)
{
  return provision(appEui, appKey) && join(retries, retryDelay) && setClass(p_lw_class);
}

ttn_response_t TheThingsNetwork::parseBytes(){
    if (buffer[0]=='\0')
  	  return TTN_UNSUCCESSFUL_RECEIVE;

	if (pgmstrcmp(buffer, CMP_MAC_RX) == 0)
    {
		port_t downlinkPort = receivedPort(buffer + 7);
		char *data = buffer + 7 + digits(downlinkPort) + 1;
		size_t downlinkLength = strlen(data) / 2;
		if (downlinkLength > 0)
		{
		  uint8_t downlink[downlinkLength];
		  for (size_t i = 0, d = 0; i < downlinkLength; i++, d += 2)
		  {
			downlink[i] = TTN_HEX_PAIR_TO_BYTE(data[d], data[d + 1]);
		  }
		  debugPrintMessage(SUCCESS_MESSAGE, SCS_SUCCESSFUL_TRANSMISSION_RECEIVED, data);
		  if (messageCallback)
		  {
			messageCallback(downlink, downlinkLength, downlinkPort);
		  }
		}
		else
		{
		  debugPrintMessage(SUCCESS_MESSAGE, SCS_SUCCESSFUL_TRANSMISSION);
		}
		return TTN_SUCCESSFUL_RECEIVE;
	}
    // not REC but buffer FULL
    debugPrintMessage(ERR_MESSAGE, ERR_UNEXPECTED_RESPONSE, buffer);
    return TTN_ERROR_UNEXPECTED_RESPONSE;
}

ttn_response_t TheThingsNetwork::sendBytes(const uint8_t *payload, size_t length, port_t port, bool confirm, uint8_t sf)
{
  if (sf != 0)
  {
    setSF(sf);
  }

  uint8_t mode = confirm ? MAC_TX_TYPE_CNF : MAC_TX_TYPE_UCNF;
  if (!sendPayload(mode, port, (uint8_t *)payload, length))
  {
    debugPrintMessage(ERR_MESSAGE, ERR_SEND_COMMAND_FAILED);
    return TTN_ERROR_SEND_COMMAND_FAILED;
  }

  // read modem response
  if (!readLine(buffer, sizeof(buffer)) && confirm) // Read response
	  // confirmed and RX timeout -> ask to poll if necessary
	  return TTN_UNSUCCESSFUL_RECEIVE;

  // TX only?
  if (pgmstrcmp(buffer, CMP_MAC_TX_OK) == 0)
  {
    debugPrintMessage(SUCCESS_MESSAGE, SCS_SUCCESSFUL_TRANSMISSION);
    return TTN_SUCCESSFUL_TRANSMISSION;
  }
  else if (pgmstrcmp(buffer, CMP_MAC_ERR) == 0)
	return TTN_UNSUCCESSFUL_RECEIVE;

  // Received downlink message?
  return parseBytes();
}

ttn_response_t TheThingsNetwork::poll(port_t port, bool confirm, bool modem_only)
{
  switch(lw_class)
  {

  case CLASS_A:
	  if (!modem_only)
		{
		  // Class A: send uplink and wait for rx windows
		  uint8_t payload[] = {0x00};
		  return sendBytes(payload, 1, port, confirm);
		}
	  else
	  {
		  if (!readLine(buffer, sizeof(buffer)) && confirm) // Read response
			  // confirmed and RX timeout -> ask to poll if necessary
			  return TTN_UNSUCCESSFUL_RECEIVE;

		  // Here we can have the result of pending TX, or pending RX (for confirmed messages)
		  if (pgmstrcmp(buffer, CMP_MAC_TX_OK) == 0)
		  {
		    debugPrintMessage(SUCCESS_MESSAGE, SCS_SUCCESSFUL_TRANSMISSION);
		    return TTN_SUCCESSFUL_TRANSMISSION;
		  }
		  else if (pgmstrcmp(buffer, CMP_MAC_ERR) == 0)
			return TTN_UNSUCCESSFUL_RECEIVE;

		  // Receive Message
		  return parseBytes();
	  }


  case CLASS_C:
	  {
		  // Class C: check rx buffer for any received data
		  memset(buffer, 0, sizeof(buffer));

		  uint32_t timeout = this->modemStream->getTimeout();
		  this->modemStream->setTimeout(100);
		  this->modemStream->readBytesUntil('\n', buffer, sizeof(buffer));
		  this->modemStream->setTimeout(timeout);

		  return parseBytes();
	  }

  default:
  case CLASS_B: // Not yet supported. Use default case.
	  return TTN_UNSUCCESSFUL_RECEIVE;

  }
}

void TheThingsNetwork::showStatus()
{
  readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_HWEUI, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_EUI, buffer);
  readResponse(SYS_TABLE, SYS_TABLE, SYS_GET_VDD, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_BATTERY, buffer);
  readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_APPEUI, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_APPEUI, buffer);
  readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_DEVEUI, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_DEVEUI, buffer);
  readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_DR, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_DATA_RATE, buffer);
  readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_RXDELAY1, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_RX_DELAY_1, buffer);
  readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_RXDELAY2, buffer, sizeof(buffer));
  debugPrintIndex(SHOW_RX_DELAY_2, buffer);
}

bool TheThingsNetwork::checkValidModuleConnected(bool autoBaudFirst)
{
  // check if we want to autobaud first
  if(autoBaudFirst)
  {
    autoBaud();
  }
  // send "sys get ver" to check if (and what) module is connected
  getVersion(buffer, sizeof(buffer));
  // check if we got a response (whatever it might be)
  // needsHardReset flag is set by readLine() (called at some point down the line by getVersion())
  if(this->needsHardReset)
  {
    return false;                                               // no response
  }
  // buffer contains "RN2xx3[xx] x.x.x ...", getting only model (RN2xx3[xx])
  char *model = strtok(buffer, " ");
  debugPrintIndex(SHOW_MODEL, model);
  // check if module is valid (must be RN2483, RN2483A, RN2903 or RN2903AS)
  if(pgmstrcmp(model, CMP_RN2483) == 0 || pgmstrcmp(model, CMP_RN2483A) == 0 || pgmstrcmp(model, CMP_RN2903) == 0 || pgmstrcmp(model, CMP_RN2903AS) == 0)
  {
    debugPrintMessage(SUCCESS_MESSAGE, SCS_VALID_MODULE);
    return true;                                                // module responded and is valid (recognized/supported)
  }
  debugPrintMessage(ERR_MESSAGE, ERR_INVALID_MODULE);
  return false;                                                 // module responded but is invalid (unrecognized/unsupported)
}

void TheThingsNetwork::configureEU868()
{
  sendMacSet(MAC_RX2, 3, 869525000);
  sendChSet(MAC_CHANNEL_DRRANGE, 1, "0 6");

  char buf[10];
  uint32_t freq = 867100000;
  uint8_t ch;
  for (ch = 0; ch < 8; ch++)
  {
    if (ch > 2)
    {
      sprintf(buf, "%lu", freq);
      sendChSet(MAC_CHANNEL_FREQ, ch, buf);
      sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 5");
      sendChSet(MAC_CHANNEL_DCYCLE, ch, 499); // 5*0.2% ETSI band G1, total 1%
      setChannelStatus(ch, true);;
      freq = freq + 200000;
    }
    else
    	sendChSet(MAC_CHANNEL_DCYCLE, ch, 299); // 3*0.33% ETSI band G, total 1%
  }
  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_EU868);
}

void TheThingsNetwork::configureUS915(uint8_t fsb)
{
  uint8_t ch;
  uint8_t chLow = fsb > 0 ? (fsb - 1) * 8 : 0;
  uint8_t chHigh = fsb > 0 ? chLow + 7 : 71;
  uint8_t ch500 = fsb + 63;
  for (ch = 0; ch < 72; ch++)
  {
    if (ch == ch500 || (ch <= chHigh && ch >= chLow))
    {
      setChannelStatus(ch, true);
      if (ch < 63)
      {
        sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 3");
      }
    }
    else
    {
    	setChannelStatus(ch, false);
    }
  }
  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_US915);
}

void TheThingsNetwork::configureAU915(uint8_t fsb)
{
  uint8_t ch;
  uint8_t chLow = fsb > 0 ? (fsb - 1) * 8 : 0;
  uint8_t chHigh = fsb > 0 ? chLow + 7 : 71;
  uint8_t ch500 = fsb + 63;
  for (ch = 0; ch < 72; ch++)
  {
    if (ch == ch500 || (ch <= chHigh && ch >= chLow))
    {
      setChannelStatus(ch, true);
      if (ch < 63)
      {
        sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 3");
      }
    }
    else
    {
    	setChannelStatus(ch, false);
    }
  }
  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_AU915);
}

void TheThingsNetwork::configureAS920_923()
{
  /* RN2903AS 1.0.3rc9 defaults
   * CH0 = 923.2MHz
   * CH1 = 923.4MHz
   */
  setADR(false); // TODO: remove when ADR is implemented for this plan
  sendMacSet(MAC_RX2, 2, 923200000);

  char buf[10];
  uint32_t freq = 922000000;
  uint8_t ch;
  for (ch = 0; ch < 8; ch++)
  {
    sendChSet(MAC_CHANNEL_DCYCLE, ch, 799);
    if (ch > 1)
    {
      sprintf(buf, "%lu", freq);
      sendChSet(MAC_CHANNEL_FREQ, ch, buf);
      sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 5");
      setChannelStatus(ch, true);
      freq = freq + 200000;
    }
  }
  // TODO: SF7BW250/DR6 channel, not properly supported by RN2903AS yet
  //sendChSet(MAC_CHANNEL_DCYCLE, 8, 799);
  //sendChSet(MAC_CHANNEL_FREQ, 8, 922100000);
  //sendChSet(MAC_CHANNEL_DRRANGE, 8, "6 6");
  //setChannelStatus(8, true);
  // TODO: Add FSK channel on 921800000
  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_AS920_923);
}

void TheThingsNetwork::configureAS923_925()
{
  /* RN2903AS 1.0.3rc9 defaults
   * CH0 = 923.2MHz
   * CH1 = 923.4MHz
   */
  setADR(false); // TODO: remove when ADR is implemented for this plan
  sendMacSet(MAC_RX2, 2, 923200000);

  char buf[10];
  uint32_t freq = 923600000;
  uint8_t ch;
  for (ch = 0; ch < 8; ch++)
  {
    sendChSet(MAC_CHANNEL_DCYCLE, ch, 799);
    if (ch > 1)
    {
      sprintf(buf, "%lu", freq);
      sendChSet(MAC_CHANNEL_FREQ, ch, buf);
      sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 5");
      setChannelStatus(ch, true);
      freq = freq + 200000;
    }
  }
  // TODO: SF7BW250/DR6 channel, not properly supported by RN2903AS yet
  //sendChSet(MAC_CHANNEL_DCYCLE, 8, 799);
  //sendChSet(MAC_CHANNEL_FREQ, 8, 924500000);
  //sendChSet(MAC_CHANNEL_DRRANGE, 8, "6 6");
  //setChannelStatus(8, true);
  // TODO: Add FSK channel on 924800000
  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_AS923_925);
}

void TheThingsNetwork::configureKR920_923()
{
  setADR(false); // TODO: remove when ADR is implemented for this plan
  sendMacSet(MAC_RX2, 0, 921900000); // KR still uses SF12 for now. Might change to SF9 later.

  //disable two default LoRaWAN channels
  setChannelStatus(0, false);
  setChannelStatus(1, false);

  char buf[10];
  uint32_t freq = 922100000;
  uint8_t ch;
  for (ch = 2; ch < 9; ch++)
  {
    sendChSet(MAC_CHANNEL_DCYCLE, ch, 799);
    sprintf(buf, "%lu", freq);
    sendChSet(MAC_CHANNEL_FREQ, ch, buf);
    sendChSet(MAC_CHANNEL_DRRANGE, ch, "0 5");
    setChannelStatus(ch, true);
    freq = freq + 200000;
  }
  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_KR920_923);
}

void TheThingsNetwork::configureIN865_867()
{
  setADR(false); // TODO: remove when ADR is implemented for this plan
  sendMacSet(MAC_RX2, 2, 866550000); // SF10

  // Disable the three default LoRaWAN channels
  setChannelStatus(0, false);
  setChannelStatus(1, false);
  setChannelStatus(2, false);

  // Channel 3
  sendChSet(MAC_CHANNEL_DCYCLE, 3, 299);
  sendChSet(MAC_CHANNEL_FREQ, 3, 865062500);
  sendChSet(MAC_CHANNEL_DRRANGE, 3, "0 5");
  setChannelStatus(3, true);

  // Channel 4
  sendChSet(MAC_CHANNEL_DCYCLE, 4, 299);
  sendChSet(MAC_CHANNEL_FREQ, 4, 865402500);
  sendChSet(MAC_CHANNEL_DRRANGE, 4, "0 5");
  setChannelStatus(4, true);

  // Channel 5
  sendChSet(MAC_CHANNEL_DCYCLE, 5, 299);
  sendChSet(MAC_CHANNEL_FREQ, 5, 865985000);
  sendChSet(MAC_CHANNEL_DRRANGE, 5, "0 5");
  setChannelStatus(5, true);

  sendMacSet(MAC_PWRIDX, TTN_PWRIDX_IN865_867);
}

void TheThingsNetwork::configureChannels(uint8_t fsb)
{
  switch (fp)
  {
  case TTN_FP_EU868:
    configureEU868();
    break;
  case TTN_FP_US915:
    configureUS915(fsb);
    break;
  case TTN_FP_AU915:
    configureAU915(fsb);
    break;
  case TTN_FP_AS920_923:
    configureAS920_923();
    break;
  case TTN_FP_AS923_925:
    configureAS923_925();
    break;
  case TTN_FP_KR920_923:
    configureKR920_923();
    break;
  case TTN_FP_IN865_867:
    configureIN865_867();
    break;
  default:
    debugPrintMessage(ERR_MESSAGE, ERR_INVALID_FP);
    break;
  }
  sendMacSet(MAC_RETX, TTN_RETX);
}

bool TheThingsNetwork::setChannel(uint8_t channel, uint32_t frequency, uint8_t dr_min, uint8_t dr_max){

  bool done = true;

  if (channel > 15)
	  return false;

  if (frequency){
	char buf[11];
	sprintf(buf, "%lu", frequency);
	done &= sendChSet(MAC_CHANNEL_FREQ, channel, buf);
  }

  if (done && (dr_min < 16) && (dr_max < 16)){
	char buf[11];
	sprintf(buf, "%u %u", dr_min, dr_max);
	done &= sendChSet(MAC_CHANNEL_DRRANGE, channel, buf);
  }
  else
	done &= (dr_min == 255) && (dr_max == 255);

  return done;
}

bool TheThingsNetwork::setRx2Channel(uint32_t frequency, uint8_t dr){

  char buf[15];
  sprintf(buf, "%u %lu", dr, frequency);
  return sendMacSet(MAC_RX2, buf);
}

bool TheThingsNetwork::setChannelStatus (uint8_t channel, bool status){
  if (status)
    return sendChSet(MAC_CHANNEL_STATUS, channel, "on");
  else
    return sendChSet(MAC_CHANNEL_STATUS, channel, "off");
}

bool TheThingsNetwork::setChannelDCycle (uint8_t channel, float duty_cycle){ // in percent
  if (channel > 15 || duty_cycle > 100.0 || duty_cycle < 0.0)
	return false;

  char buf[6]; // number 99999
  if (0.0 == duty_cycle)
	  (void)sprintf(buf, "%u", 65535u);
  else
	  (void)sprintf(buf, "%u", (uint16_t)((100.0/duty_cycle) - 1));

  return sendChSet(MAC_CHANNEL_DCYCLE, channel, buf);
}

bool TheThingsNetwork::setPowerIndex(uint8_t index){
  char buf[4];
  sprintf(buf, "%u",index);
  return sendMacSet(MAC_PWRIDX, buf);
}

bool TheThingsNetwork::setDR(uint8_t dr){
  char buf[4];
  sprintf(buf, "%u",dr);
  return sendMacSet(MAC_DR, buf);
}

bool TheThingsNetwork::setADR(bool adr){
	bool ret;
	if (adr)
	{
		ret = sendMacSet(MAC_ADR, "on");
	}
	else
	{
		ret = sendMacSet(MAC_ADR, "off");
	}
	this->adr = adr;
	return ret;
}

bool TheThingsNetwork::setSF(uint8_t sf)
{
  uint8_t dr;
  switch (fp)
  {
  case TTN_FP_EU868:
  case TTN_FP_IN865_867:
  case TTN_FP_AS920_923:
  case TTN_FP_AS923_925:
  case TTN_FP_KR920_923:
  default:
    dr = 12 - sf;
    break;
  case TTN_FP_US915:
  case TTN_FP_AU915:
    dr = 10 - sf;
    break;
  }
  char s[2];
  s[0] = '0' + dr;
  s[1] = '\0';
  return sendMacSet(MAC_DR, s);
}

bool TheThingsNetwork::setRX1Delay(uint16_t delay){
	  char buf[6];
	  sprintf(buf, "%u",delay);
	  return sendMacSet(MAC_RXDELAY1, buf);
}

bool TheThingsNetwork::setFCU(uint32_t fcu){
  char buf[10];
  sprintf(buf, "%lu", fcu);
  return sendMacSet(MAC_UPCTR, buf);
}

bool TheThingsNetwork::setFCD(uint32_t fcd){
  char buf[10];
  sprintf(buf, "%lu", fcd);
  return sendMacSet(MAC_DNCTR, buf);
}

void TheThingsNetwork::sendCommand(uint8_t table, uint8_t index, bool appendSpace, bool print)
{
  char command[100];
  switch (table)
  {
  case MAC_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(mac_table[index])));
    break;
  case MAC_GET_SET_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(mac_options[index])));
    break;
  case MAC_JOIN_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(mac_join_mode[index])));
    break;
  case MAC_CH_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(mac_ch_options[index])));
    break;
  case MAC_TX_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(mac_tx_table[index])));
    break;
  case SYS_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(sys_table[index])));
    break;
  case RADIO_TABLE:
    strcpy_P(command, (char *)pgm_read_word(&(radio_table[index])));
    break;
  default:
    return;
  }
  modemStream->write(command);
  if (appendSpace)
  {
    modemStream->write(" ");
  }
  if (print)
  {
    debugPrint(command);
    debugPrint(F(" "));
  }
}

bool TheThingsNetwork::sendMacSet(uint8_t index, uint8_t value1, unsigned long value2)
{
	char buf[15];
	sprintf(buf, "%u %lu", value1, value2);
	return sendMacSet(index, buf);
}

bool TheThingsNetwork::sendMacSet(uint8_t index, const char *value)
{
  clearReadBuffer();
  debugPrint(SENDING);
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SET, true);
  sendCommand(MAC_GET_SET_TABLE, index, true);
  modemStream->write(value);
  modemStream->write(SEND_MSG);
  debugPrintLn(value);
  return waitForOk();
}

bool TheThingsNetwork::waitForOk()
{
  readLine(buffer, sizeof(buffer));
  if (pgmstrcmp(buffer, CMP_OK) != 0)
  {
    debugPrintMessage(ERR_MESSAGE, ERR_RESPONSE_IS_NOT_OK, buffer);
    return false;
  }
  return true;
}

bool TheThingsNetwork::sendChSet(uint8_t index, uint8_t channel, unsigned long value)
{
	char buf[11];
	sprintf(buf, "%lu", value);
	return sendChSet(index, channel, buf);
}

bool TheThingsNetwork::sendChSet(uint8_t index, uint8_t channel, const char *value)
{
  clearReadBuffer();
  char ch[5];
  if (channel > 9)
  {
    ch[0] = ((channel - (channel % 10)) / 10) + 48;
    ch[1] = (channel % 10) + 48;
    ch[2] = '\0';
  }
  else
  {
    ch[0] = channel + 48;
    ch[1] = '\0';
  }
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SET, true);
  sendCommand(MAC_GET_SET_TABLE, MAC_CH, true);
  sendCommand(MAC_CH_TABLE, index, true);
  modemStream->write(ch);
  modemStream->write(" ");
  modemStream->write(value);
  modemStream->write(SEND_MSG);
  debugPrint(channel);
  debugPrint(F(" "));
  debugPrintLn(value);
  return waitForOk();
}

bool TheThingsNetwork::sendJoinSet(uint8_t type)
{
  clearReadBuffer();
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_JOIN, true);
  sendCommand(MAC_JOIN_TABLE, type, false);
  modemStream->write(SEND_MSG);
  debugPrintLn();
  return waitForOk();
}

bool TheThingsNetwork::sendPayload(uint8_t mode, uint8_t port, uint8_t *payload, size_t length)
{
  clearReadBuffer();
  debugPrint(F(SENDING));
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_TX, true);
  sendCommand(MAC_TX_TABLE, mode, true);
  char sport[4];
  if (port > 99)
  {
    sport[0] = ((port - (port % 100)) / 100) + 48;
    sport[1] = (((port % 100) - (port % 10)) / 10) + 48;
    sport[2] = (port % 10) + 48;
    sport[3] = '\0';
  }
  else if (port > 9)
  {
    sport[0] = ((port - (port % 10)) / 10) + 48;
    sport[1] = (port % 10) + 48;
    sport[2] = '\0';
  }
  else
  {
    sport[0] = port + 48;
    sport[1] = '\0';
  }
  modemStream->write(sport);
  modemStream->print(" ");
  debugPrint(sport);
  debugPrint(F(" "));
  uint8_t i = 0;
  for (i = 0; i < length; i++)
  {
    if (payload[i] < 16)
    {
      modemStream->print("0");
      modemStream->print(payload[i], HEX);
      debugPrint(F("0"));
      debugPrint(payload[i], HEX);
    }
    else
    {
      modemStream->print(payload[i], HEX);
      debugPrint(payload[i], HEX);
    }
  }
  modemStream->write(SEND_MSG);
  debugPrintLn();
  return waitForOk();
}

void TheThingsNetwork::sleep(uint32_t mseconds)
{
  if (mseconds < 100)
  {
    return;
  }

  debugPrint(F(SENDING));
  sendCommand(SYS_TABLE, SYS_PREFIX, true);
  sendCommand(SYS_TABLE, SYS_SLEEP, true);

  sprintf(buffer, "%lu", mseconds);
  modemStream->write(buffer);
  modemStream->write(SEND_MSG);
  debugPrintLn(buffer);
}

void TheThingsNetwork::wake()
{
  autoBaud();
}

void TheThingsNetwork::linkCheck(uint16_t seconds)
{
  clearReadBuffer();
  debugPrint(SENDING);
  sendCommand(MAC_TABLE, MAC_PREFIX, true);
  sendCommand(MAC_TABLE, MAC_SET, true);
  sendCommand(MAC_GET_SET_TABLE, MAC_LINKCHK, true);

  sprintf(buffer, "%u", seconds);
  modemStream->write(buffer);
  modemStream->write(SEND_MSG);
  debugPrintLn(buffer);
  waitForOk();
}

uint8_t TheThingsNetwork::getLinkCheckGateways()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_GWNB, buffer, sizeof(buffer))){
	  char **endptr = NULL;
	  uint8_t gwnb = strtol(buffer, endptr, 10);
	  if (endptr == NULL)
		  return gwnb;
  }
  return 0; // Gateway number defaults to 0
}

uint8_t TheThingsNetwork::getLinkCheckMargin()
{
  if (readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_MRGN, buffer, sizeof(buffer))){
	  char **endptr = NULL;
	  uint8_t mgn = strtol(buffer, endptr, 10);
	  if (endptr == NULL)
		  return mgn;
  }
  return 255; // Signal margin defaults to 255
}
