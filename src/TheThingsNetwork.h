// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNETWORK_H_
#define _THETHINGSNETWORK_H_

#include <Arduino.h>
#include <Stream.h>

#define TTN_DEFAULT_WAIT_TIME 120
#define TTN_DEFAULT_SF 7
#define TTN_DEFAULT_FSB 2

// Set ADR off as it is currently not supported by The Things Network
// The RN2xx3 module slows down to SF12 when no datarate commands are
// sent by the network, so disabling ADR is a work-around to avoid
// all the modules slowing down to SF12
#define TTN_ADR_SUPPORTED false

#define TTN_PWRIDX_868 1
#define TTN_PWRIDX_915 5

#define TTN_HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define TTN_HEX_PAIR_TO_BYTE(h, l) ((TTN_HEX_CHAR_TO_NIBBLE(h) << 4) + TTN_HEX_CHAR_TO_NIBBLE(l))

typedef unsigned long   fp_ttn_t;

#define TTN_FP_EU868 1
#define TTN_FP_US915 2

typedef struct  airtime_s
{
  int           sf;
  int           de;
  int           ps;
  int           band;
  int           header;
  int           cr;
}               airtime_t;

class TheThingsNetwork
{
  private:
    Stream* modemStream;
    Stream* debugStream;
    String model;
    fp_ttn_t fp;
    airtime_t info;
    float airtime;
    int sf;
    int fsb;
    void (* messageCallback)(const byte* payload, int length, int port);

    void fillAirtimeInfo();
    int getInfo(String str);
    void trackAirtime(int payloadSize);
    String readLine(int waitTime = TTN_DEFAULT_WAIT_TIME);
    bool waitForOK(int waitTime = TTN_DEFAULT_WAIT_TIME, String okMessage = "ok");
    String readValue(String key);
    bool sendCommand(String cmd, int waitTime = TTN_DEFAULT_WAIT_TIME);
    bool sendCommand(String cmd, String value, int waitTime = TTN_DEFAULT_WAIT_TIME);
    bool sendCommand(String cmd, const byte* buf, int length, int waitTime = TTN_DEFAULT_WAIT_TIME);
    void reset(bool adr = true);
    void configureEU868(int sf);
    void configureUS915(int sf, int fsb);
    void configureChannels(int sf, int fsb);

  public:
    TheThingsNetwork(Stream& modemStream, Stream& debugStream, fp_ttn_t fp, int sf = TTN_DEFAULT_SF, int fsb = TTN_DEFAULT_FSB);
    void showStatus();
    void onMessage(void (*cb)(const byte* payload, int length, int port));
    bool provision(const byte appEui[8], const byte appKey[16]);
    bool join(const byte appEui[8], const byte appKey[16], int retries = -1, long int retryDelay = 10000);
    bool join(int retries = -1, long int retryDelay = 10000);
    bool personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]);
    bool personalize();
    int sendBytes(const byte* payload, int length, int port = 1, bool confirm = false);
    int poll(int port = 1, bool confirm = false);
};

#endif
