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
  private:
    void(*onButtonPressCallback)(void);
    void(*onButtonReleaseCallback)(void);

    void initLight();
    void initTemperature();
    void initLED();
    void initButton();

  public:
    TheThingsNode(); 
    void showStatus();

    uint16_t getLight();
    void configLight(int gain);

    int8_t getTemperatureAsInt();
    float getTemperatureAsFloat();

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
};

#endif
