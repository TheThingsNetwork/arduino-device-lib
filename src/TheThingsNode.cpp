// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include <Arduino.h>
#include <TheThingsNode.h>

#define TTN_LDR_INPUT 10
#define TTN_LDR_GAIN1 12
#define TTN_LDR_GAIN2 4
#define TTN_RED_LED 13
#define TTN_GREEN_LED 5
#define TTN_BLUE_LED 6
#define TTN_BUTTON 16

#define TTN_TEMPERATURE_SENSOR_ADDRESS 0x18

Hackscribble_MCP9804 TTN_TEMPERATURE_SENSOR(TTN_TEMPERATURE_SENSOR_ADDRESS);

bool TTN_BUTTON_PRESSED = false;

void (*TTN_BUTTON_PRESS)(void);
void (*TTN_BUTTON_RELEASE)(void);

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
  Serial.print(F("Color: "));
  Serial.println(colorToString(getColor()));
  Serial.print(F("USB: "));
  Serial.println(getUSB() ? F("Yes") : F("No"));
}

uint16_t TheThingsNode::getLight()
{
  return analogRead(TTN_LDR_INPUT);
}

void TheThingsNode::configLight(int gain)
{
  switch (gain)
  {
    case 0:
      digitalWrite(TTN_LDR_GAIN1, LOW);
      digitalWrite(TTN_LDR_GAIN2, LOW);
    break;
    case 1:
      digitalWrite(TTN_LDR_GAIN1, HIGH);
      digitalWrite(TTN_LDR_GAIN2, LOW);
    break;
    case 2:
      digitalWrite(TTN_LDR_GAIN1, LOW);
      digitalWrite(TTN_LDR_GAIN2, HIGH);
    break;
    case 3:
      digitalWrite(TTN_LDR_GAIN1, HIGH);
      digitalWrite(TTN_LDR_GAIN2, HIGH);
    break;
  }
}

int8_t TheThingsNode::getTemperatureAsInt()
{
  return TTN_TEMPERATURE_SENSOR.getTAInteger();
}

float TheThingsNode::getTemperatureAsFloat()
{
  return TTN_TEMPERATURE_SENSOR.getTAFloat();
}

bool TheThingsNode::getRed()
{
  return (digitalRead(TTN_RED_LED) == LOW);
}

bool TheThingsNode::getGreen()
{
  return (digitalRead(TTN_GREEN_LED) == LOW);
}

bool TheThingsNode::getBlue()
{
  return (digitalRead(TTN_BLUE_LED) == LOW);
}

TTN_COLOR TheThingsNode::getColor()
{
  bool red = getRed();
  bool green = getGreen();
  bool blue = getBlue();

  if (red && green && blue)
  {
    return TTN_WHITE;
  }
  else if (red && green)
  {
    return TTN_YELLOW;
  }
  else if (red && blue)
  {
    return TTN_MAGENTA;
  }
  else if (green && blue)
  {
    return TTN_CYAN;
  }
  else if (red)
  {
    return TTN_RED;
  }
  else if (green)
  {
    return TTN_GREEN;
  }
  else if (blue)
  {
    return TTN_BLUE;
  }
  else
  {
    return TTN_BLACK;
  }
}

String TheThingsNode::colorToString(TTN_COLOR color)
{
  switch (color)
  {
    case TTN_RED:
      return String("Red");
      break;
    case TTN_GREEN:
      return String("Green");
      break;
    case TTN_BLUE:
      return String("Blue");
      break;
    case TTN_YELLOW:
      return String("Yellow");
      break;
    case TTN_CYAN:
      return String("Cyan");
      break;
    case TTN_MAGENTA:
      return String("Magenta");
      break;
    case TTN_WHITE:
      return String("White");
      break;
    case TTN_BLACK:
      return String("Black");
      break;
  }
}

void TheThingsNode::setRGB(bool red, bool green, bool blue)
{
  setRed(red);
  setGreen(green);
  setBlue(blue);
}

void TheThingsNode::setRed(bool on)
{
  digitalWrite(TTN_RED_LED, on ? LOW : HIGH);
}

void TheThingsNode::setGreen(bool on)
{
  digitalWrite(TTN_GREEN_LED, on ? LOW : HIGH);
}

void TheThingsNode::setBlue(bool on)
{
  digitalWrite(TTN_BLUE_LED, on ? LOW : HIGH);
}

void TheThingsNode::setColor(TTN_COLOR color)
{
  switch (color)
  {
    case TTN_RED:
      setRGB(true, false, false);
      break;
    case TTN_GREEN:
      setRGB(false, true, false);
      break;
    case TTN_BLUE:
      setRGB(false, false, true);
      break;
    case TTN_YELLOW:
      setRGB(true, true, false);
      break;
    case TTN_CYAN:
      setRGB(false, true, true);
      break;
    case TTN_MAGENTA:
      setRGB(true, false, true);
      break;
    case TTN_WHITE:
      setRGB(true, true, true);
      break;
    case TTN_BLACK:
      setRGB(false, false, false);
      break;
  }
}

void TheThingsNode::onButtonPress(void (*callback)(void))
{
  TTN_BUTTON_PRESS = callback;
}

void TheThingsNode::onButtonRelease(void (*callback)(void))
{
  TTN_BUTTON_RELEASE = callback;
}

bool TheThingsNode::getUSB()
{
  return USBSTA&(1<<VBUS);
}

/* PRIVATE */

void TheThingsNode::initLight()
{ 
  pinMode(TTN_LDR_GAIN1, OUTPUT);
  pinMode(TTN_LDR_GAIN2, OUTPUT);

  digitalWrite(TTN_LDR_GAIN1, LOW);
  digitalWrite(TTN_LDR_GAIN2, LOW);

  configLight(1);
}

void TheThingsNode::initTemperature()
{ 
  // TTN_TEMPERATURE_SENSOR.setResolution(R_DEGREES_0_0625);
}

void TheThingsNode::initLED()
{
  pinMode(TTN_RED_LED, OUTPUT);
  pinMode(TTN_GREEN_LED, OUTPUT);
  pinMode(TTN_BLUE_LED, OUTPUT);
}

void TTN_BUTTON_CALLBACK()
{
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(TTN_BUTTON));

  if (trigger == RISING)
  {
    if (TTN_BUTTON_RELEASE)
    {
      TTN_BUTTON_RELEASE();
    }
  }
  else if(trigger == FALLING)
  {
    if (TTN_BUTTON_PRESS)
    {
      TTN_BUTTON_PRESS();
    }
  }
}

void TheThingsNode::initButton()
{
  pinMode(TTN_BUTTON, INPUT);
  digitalWrite(TTN_BUTTON, HIGH);

  attachPCINT(digitalPinToPCINT(TTN_BUTTON), TTN_BUTTON_CALLBACK, CHANGE);
}