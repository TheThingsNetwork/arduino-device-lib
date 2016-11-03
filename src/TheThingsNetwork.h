// Copyright © 2016 The Things Network
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
#define SEND_MSG "\r\n"

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

typedef uint8_t port_t;

enum ttn_fp_t {
  TTN_FP_EU868,
  TTN_FP_US915
};

typedef struct  airtime_s
{
  uint8_t sf;
  uint8_t de;
  uint8_t ps;
  uint16_t band;
  uint8_t header;
  uint8_t cr;
} airtime_t;

class TheThingsNetwork
{
  private:
    port_t port;
    Stream* modemStream;
    Stream* debugStream;
    String model;
    airtime_t info;
    float airtime;
    ttn_fp_t fp;
    uint8_t sf;
    uint8_t fsb;
    void (* messageCallback)(const byte* payload, size_t length, port_t port);

    const char *readLine();
    void fillAirtimeInfo();
    void trackAirtime(size_t payloadSize);
    void reset(bool adr = true);
    void configureEU868(uint8_t sf);
    void configureUS915(uint8_t sf, uint8_t fsb);
    void configureChannels(uint8_t sf, uint8_t fsb);
    bool waitForOk();
    void sendCommand(uint8_t table, uint8_t index, bool with_space, bool print = true);
    bool sendMacSet(uint8_t index, const char* setting);
    bool sendChSet(uint8_t index, uint8_t channel, const char* setting);
    bool sendJoinSet(uint8_t type);
    bool sendPayload(uint8_t mode, uint8_t port, uint8_t* payload, size_t len);
    void sendGetValue(uint8_t table, uint8_t prefix, uint8_t index);
    bool compareStrings(const char *str1, const char *str2, size_t length = -2);
    const char *subString(char *response, size_t start, size_t end = -2);
    uint8_t receivedPort(const char *response, size_t length);
    size_t bufLength(char *data);
    size_t portLength(size_t port);
    const char *readValue(uint8_t prefixTable, uint8_t indexTable, uint8_t index);
    const char *readValue(uint8_t table, uint8_t index);
    void errMessage(const char *err, char *errMsg);
    void errMessage(const char *err);

  public:
    TheThingsNetwork(Stream& modemStream, Stream& debugStream, ttn_fp_t fp, uint8_t sf = TTN_DEFAULT_SF, uint8_t fsb = TTN_DEFAULT_FSB);
    void showStatus();
    void onMessage(void (*cb)(const byte* payload, size_t length, port_t port));
    bool provision(const char *appEui, const char *appKey);
    bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000);
    bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
    bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
    bool personalize();
    int8_t sendBytes(const byte* payload, size_t length, port_t port = 1, bool confirm = false);
    int8_t poll(port_t port = 1, bool confirm = false);
};

#endif
