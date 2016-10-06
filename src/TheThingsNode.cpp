// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <Arduino.h>
#include <TheThingsNode.h>

#define THETHINGSNODE_LDR_INPUT 10
#define THETHINGSNODE_LDR_GAIN1 12
#define THETHINGSNODE_LDR_GAIN2 4
#define THETHINGSNODE_RED_LED 13
#define THETHINGSNODE_GREEN_LED 5
#define THETHINGSNODE_BLUE_LED 6
#define THETHINGSNODE_BUTTON 16

#define THETHINGSNODE_TEMPERATURE_SENSOR_ADDRESS 0x18

Hackscribble_MCP9804 THETHINGSNODE_TEMPERATURE_SENSOR(THETHINGSNODE_TEMPERATURE_SENSOR_ADDRESS);

bool THETHINGSNODE_BUTTON_PRESSED = false;

void (*THETHINGSNODE_BUTTON_PRESS)(void);
void (*THETHINGSNODE_BUTTON_RELEASE)(void);

/* PUBLIC */

TheThingsNode::TheThingsNode() 
{
  Serial.begin(9600);

  initLight();
  initTemperature();
  initLED();
  initButton();
}

void TheThingsNode::showStatus()
{
  Serial.print(F("Light: "));
  Serial.println(String(getLight()));
  Serial.print(F("Temperature as int: "));
  Serial.println(String(getTemperatureAsInt()));
  Serial.print(F("Temperature as float: "));
  Serial.println(String(getTemperatureAsFloat()));
  Serial.print(F("Red LED: "));
  Serial.println(getRedLED() ? F("on") : F("off"));
  Serial.print(F("Green LED: "));
  Serial.println(getGreenLED() ? F("on") : F("off"));
  Serial.print(F("Blue LED: "));
  Serial.println(getBlueLED() ? F("on") : F("off"));
}

uint16_t TheThingsNode::getLight()
{
  return analogRead(THETHINGSNODE_LDR_INPUT);
}

void TheThingsNode::configLight(int gain)
{
  switch(gain)
  {
    case 0:
      digitalWrite(THETHINGSNODE_LDR_GAIN1, LOW);
      digitalWrite(THETHINGSNODE_LDR_GAIN2, LOW);
    break;
    case 1:
      digitalWrite(THETHINGSNODE_LDR_GAIN1, HIGH);
      digitalWrite(THETHINGSNODE_LDR_GAIN2, LOW);
    break;
    case 2:
      digitalWrite(THETHINGSNODE_LDR_GAIN1, LOW);
      digitalWrite(THETHINGSNODE_LDR_GAIN2, HIGH);
    break;
    case 3:
      digitalWrite(THETHINGSNODE_LDR_GAIN1, HIGH);
      digitalWrite(THETHINGSNODE_LDR_GAIN2, HIGH);
    break;
  }
}

int8_t TheThingsNode::getTemperatureAsInt()
{
  return THETHINGSNODE_TEMPERATURE_SENSOR.getTAInteger();
}

float TheThingsNode::getTemperatureAsFloat()
{
  return THETHINGSNODE_TEMPERATURE_SENSOR.getTAFloat();
}

bool TheThingsNode::getRedLED()
{
  return (digitalRead(THETHINGSNODE_RED_LED) == LOW);
}

bool TheThingsNode::getGreenLED()
{
  return (digitalRead(THETHINGSNODE_GREEN_LED) == LOW);
}

bool TheThingsNode::getBlueLED()
{
  return (digitalRead(THETHINGSNODE_BLUE_LED) == LOW);
}

void TheThingsNode::setLED(bool red, bool green, bool blue)
{
  setRedLED(red);
  setGreenLED(green);
  setBlueLED(blue);
}

void TheThingsNode::setRedLED(bool on)
{
  digitalWrite(THETHINGSNODE_RED_LED, on ? LOW : HIGH);
}

void TheThingsNode::setGreenLED(bool on)
{
  digitalWrite(THETHINGSNODE_GREEN_LED, on ? LOW : HIGH);
}

void TheThingsNode::setBlueLED(bool on)
{
  digitalWrite(THETHINGSNODE_BLUE_LED, on ? LOW : HIGH);
}

void TheThingsNode::onButtonPress(void (*callback)(void))
{
  THETHINGSNODE_BUTTON_PRESS = callback;
}

void TheThingsNode::onButtonRelease(void (*callback)(void))
{
  THETHINGSNODE_BUTTON_RELEASE = callback;
}

/* PRIVATE */

void TheThingsNode::initLight()
{ 
  pinMode(THETHINGSNODE_LDR_GAIN1, OUTPUT);
  pinMode(THETHINGSNODE_LDR_GAIN2, OUTPUT);

  digitalWrite(THETHINGSNODE_LDR_GAIN1, LOW);
  digitalWrite(THETHINGSNODE_LDR_GAIN2, LOW);

  configLight(1);
}

void TheThingsNode::initTemperature()
{ 
  // THETHINGSNODE_TEMPERATURE_SENSOR.setResolution(R_DEGREES_0_0625);
}

void TheThingsNode::initLED()
{
  pinMode(THETHINGSNODE_RED_LED, OUTPUT);
  pinMode(THETHINGSNODE_GREEN_LED, OUTPUT);
  pinMode(THETHINGSNODE_BLUE_LED, OUTPUT);
}

void THETHINGSNODE_BUTTON_CALLBACK()
{
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(THETHINGSNODE_BUTTON));

  if (trigger == RISING)
  {
    if (THETHINGSNODE_BUTTON_RELEASE)
    {
      THETHINGSNODE_BUTTON_RELEASE();
    }
  }
  else if(trigger == FALLING)
  {
    if (THETHINGSNODE_BUTTON_PRESS)
    {
      THETHINGSNODE_BUTTON_PRESS();
    }
  }
}

void TheThingsNode::initButton()
{
  pinMode(THETHINGSNODE_BUTTON, INPUT);
  digitalWrite(THETHINGSNODE_BUTTON, HIGH);

  attachPCINT(digitalPinToPCINT(THETHINGSNODE_BUTTON), THETHINGSNODE_BUTTON_CALLBACK, CHANGE);
}