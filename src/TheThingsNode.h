// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNODE_H_
#define _THETHINGSNODE_H_

#include <Arduino.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <Hackscribble_MCP9804.h>

enum ttn_color: byte
{
  TTN_RED,
  TTN_GREEN,
  TTN_BLUE,
  TTN_YELLOW,
  TTN_CYAN,
  TTN_MAGENTA,
  TTN_WHITE,
  TTN_BLACK
};

class TheThingsNode
{
  public:

    // static method to get the instance 
    static TheThingsNode* setup() {
      static TheThingsNode node;
      return &node;
    };

    void onWake(void(*callback)(void));
    void loop();
    void onSleep(void(*callback)(void));

    void showStatus();

    void onInterval(void(*callback)(void));
    void configInterval(bool enabled, uint32_t ms);
    void configInterval(bool enabled);

    void configLight(bool enabled, uint8_t gain);
    void configLight(bool enabled);
    uint16_t getLight();

    int8_t getTemperatureAsInt();
    float getTemperatureAsFloat();
    void configTemperature(bool enabled, MCP9804_Resolution resolution, int8_t lower, int8_t upper, int8_t critical, MCP9804_Hysteresis hysteresis);
    void configTemperature(bool enabled, MCP9804_Resolution resolution);
    void configTemperature(bool enabled);
    void onTemperature(void(*callback)(void));
    bool hasTemperatureAlert();

    void onMotionStart(void(*callback)(void));
    void onMotionStop(void(*callback)(unsigned long duration));
    bool isMoving();
    void configMotion(bool enabled);

    void onButtonPress(void(*callback)(void));
    void onButtonRelease(void(*callback)(unsigned long duration));
    bool isButtonPressed();
    void configButton(bool enabled);

    void setColor(ttn_color color);
    void setRGB(bool red = false, bool green = false, bool blue = false);
    void setRed(bool on = true);
    void setGreen(bool on = true);
    void setBlue(bool on = true);
    ttn_color getColor();
    String colorToString(ttn_color color);
    bool getRed();
    bool getGreen();
    bool getBlue();

    bool isUSBConnected();
    void configUSB(bool deepSleep);

    uint16_t getBattery();

  private:

    // private constructor to achieve singleton pattern
    TheThingsNode();
    TheThingsNode(TheThingsNode const&); // copy disabled
    void operator=(TheThingsNode const&); // assigment disabled

    bool intervalEnabled;
    uint32_t intervalMs;
    uint32_t intervalSince;
    bool lightEnabled;
    uint8_t lightGain;
    bool temperatureEnabled;
    bool temperatureSleep;
    bool motionStarted;
    unsigned long motionStartedAt;
    bool motionEnabled;
    bool buttonEnabled;
    bool buttonPressed;
    unsigned long buttonPressedAt;
    bool wasUSBDisconnected;
    bool USBDeepSleep;
    bool wdtStarted;

    void (*wakeCallback)(void);
    void (*sleepCallback)(void);
    void (*temperatureCallback)(void);
    void (*motionStartCallback)(void);
    void (*motionStopCallback)(unsigned long duration);
    void (*buttonPressCallback)(void);
    void (*buttonReleaseCallback)(unsigned long duration);
    void (*intervalCallback)(void);

    void wakeTemperature();
    void sleepTemperature();
    void wakeMotion();
    void sleepMotion();
    void writeMotion(unsigned char REG_ADDRESS, unsigned  char DATA);
    uint8_t readMotion(unsigned char REG_ADDRESS);
    void WDT_start();
    void WDT_stop();
    void deepSleep(void);
};

#endif
