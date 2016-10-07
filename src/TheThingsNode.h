// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNODE_H_
#define _THETHINGSNODE_H_

#include <Arduino.h>
#include <Wire.h>
#include <Hackscribble_MCP9804.h>
#include <PinChangeInterrupt.h>

enum TTN_COLOR: byte
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
    TheThingsNode(); 
    void showStatus();

    uint16_t getLight();
    void setLight(int gain);

    int8_t getTemperatureAsInt();
    float getTemperatureAsFloat();
    void onTemperature(void(*callback)(void), int8_t lower = 0, int8_t upper = 40, int8_t critical = 55, MCP9804_Hysteresis hysteresis = H_DEGREES_0_0);
    void configTemperature(bool enabled, MCP9804_Resolution resolution = R_DEGREES_0_0625);

    void onMotionStart(void(*callback)(void));
    void onMotionStop(void(*callback)(void));
    bool isMoving();
    void configMotion(bool enabled);

    void onButtonPress(void(*callback)(void));
    void onButtonRelease(void(*callback)(void));

    bool getRed();
    bool getGreen();
    bool getBlue();
    TTN_COLOR getColor();
    String colorToString(TTN_COLOR color);
    void setRGB(bool red = false, bool green = false, bool blue = false);
    void setRed(bool on = true);
    void setGreen(bool on = true);
    void setBlue(bool on = true);
    void setColor(TTN_COLOR color);

    bool getUSB();
    uint16_t getBattery();

  private:
    bool temperatureEnabled;
    bool temperatureAlert;
    bool motionEnabled;

    void initLight();
    void initButton();
    void initLED();
    void initBattery();

    void enableTemperature(bool enabled);
    void writeMotion(unsigned char REG_ADDRESS, unsigned  char DATA);
    uint8_t readMotion(unsigned char REG_ADDRESS);
};

#endif
