// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNETWORK_H_
#define _THETHINGSNETWORK_H_

#include <Arduino.h>
#include <Stream.h>
#include <avr/pgmspace.h>

#define TTN_DEFAULT_SF 7
#define TTN_DEFAULT_FSB 2
#define TTN_RETX "7"

#define TTN_PWRIDX_868 "1"
#define TTN_PWRIDX_915 "5"

#define TTN_BUFFER_SIZE 300

typedef uint8_t port_t;

enum ttn_response_t
{
  TTN_ERROR_SEND_COMMAND_FAILED = (-1),
  TTN_ERROR_UNEXPECTED_RESPONSE = (-10),
  TTN_SUCCESSFUL_TRANSMISSION = 1,
  TTN_SUCCESSFUL_RECEIVE = 2
};

enum ttn_fp_t
{
  TTN_FP_EU868,
  TTN_FP_US915
};

class TheThingsNetwork
{
private:
  Stream *modemStream;
  Stream *debugStream;
  ttn_fp_t fp;
  uint8_t sf;
  uint8_t fsb;
  char buffer[512];
  void (*messageCallback)(const uint8_t *payload, size_t size, port_t port);

  void clearReadBuffer();
  size_t readLine(char *buffer, size_t size);
  size_t readResponse(uint8_t prefixTable, uint8_t indexTable, uint8_t index, char *buffer, size_t size);
  size_t readResponse(uint8_t table, uint8_t index, char *buffer, size_t size);

  void debugPrintIndex(uint8_t index, const char *value = NULL);
  void debugPrintMessage(uint8_t type, uint8_t index, const char *value = NULL);

  void reset(bool adr = true);
  void configureEU868(uint8_t sf);
  void configureUS915(uint8_t sf, uint8_t fsb);
  void configureChannels(uint8_t sf, uint8_t fsb);
  bool waitForOk();

  void sendCommand(uint8_t table, uint8_t index, bool appendSpace, bool print = true);
  bool sendMacSet(uint8_t index, const char *value);
  bool sendChSet(uint8_t index, uint8_t channel, const char *value);
  bool sendJoinSet(uint8_t type);
  bool sendPayload(uint8_t mode, uint8_t port, uint8_t *payload, size_t len);
  void sendGetValue(uint8_t table, uint8_t prefix, uint8_t index);

public:
  TheThingsNetwork(Stream &modemStream, Stream &debugStream, ttn_fp_t fp, uint8_t sf = TTN_DEFAULT_SF, uint8_t fsb = TTN_DEFAULT_FSB);
  void showStatus();
  size_t getHardwareEui(char *buffer, size_t size);
  void onMessage(void (*cb)(const uint8_t *payload, size_t size, port_t port));
  bool provision(const char *appEui, const char *appKey);
  bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000);
  bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
  bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
  bool personalize();
  ttn_response_t sendBytes(const uint8_t *payload, size_t length, port_t port = 1, bool confirm = false);
  ttn_response_t poll(port_t port = 1, bool confirm = false);
  void sleep(uint32_t mseconds);
  void saveState();
};

#endif
