// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNETWORK_H_
#define _THETHINGSNETWORK_H_

#include <Arduino.h>
#include <Stream.h>

#define TTN_DEFAULT_SF 7
#define TTN_DEFAULT_FSB 2
#define TTN_RETX 7

#define TTN_PWRIDX_868 1
#define TTN_PWRIDX_915 5

#define TTN_HEX_CHAR_TO_NIBBLE(c) ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0'))
#define TTN_HEX_PAIR_TO_BYTE(h, l) ((TTN_HEX_CHAR_TO_NIBBLE(h) << 4) + TTN_HEX_CHAR_TO_NIBBLE(l))

enum ttn_fp_t {
  TTN_FP_EU868,
  TTN_FP_US915
};

class TheThingsNetwork
{
  private:
    Stream* modemStream;
    Stream* debugStream;
    String model;
    ttn_fp_t fp;
    int sf;
    int fsb;
    void (* messageCallback)(const byte* payload, int length, int port);

    String readLine();
    String readValue(String key);
    bool sendCommand(String cmd);
    bool sendCommand(String cmd, String value);
    bool sendCommand(String cmd, const byte* buf, int length);
    void reset(bool adr = true);
    void configureEU868(int sf);
    void configureUS915(int sf, int fsb);
    void configureChannels(int sf, int fsb);

  public:
    TheThingsNetwork(Stream& modemStream, Stream& debugStream, ttn_fp_t fp, int sf = TTN_DEFAULT_SF, int fsb = TTN_DEFAULT_FSB);
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
