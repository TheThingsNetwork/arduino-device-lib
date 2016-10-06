// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNODE_H_
#define _THETHINGSNODE_H_

#include <Arduino.h>
#include <Wire.h>
#include <Hackscribble_MCP9804.h>
#include <PinChangeInterrupt.h>

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

    bool getRedLED();
    bool getGreenLED();
    bool getBlueLED();
    void setLED(bool red = false, bool green = false, bool blue = false);
    void setRedLED(bool on = true);
    void setGreenLED(bool on = true);
    void setBlueLED(bool on = true);
};

#endif
