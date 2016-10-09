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
#define TTN_VBAT_MEAS_EN A2
#define TTN_VBAT_MEAS 1
#define TTN_TEMPERATURE_SENSOR_ADDRESS 0x18
#define TTN_TEMPERATURE_ALERT 14
#define TTN_ACCELEROMETER_INT2 9

#define TTN_ADDR_ACC 0x1D
#define TTN_DR 5  //active data rate
#define TTN_SR 3  //sleep data rate
#define TTN_SC 4 //sleep delay
#define TTN_MT  4 //0.063g/LSB
#define TTN_MDC 2 //Debounce delay in samples
#define TTN_SYSMOD 0x0B
#define TTN_FF_MT_CFG 0x15
#define TTN_FF_MT_SRC 0x16
#define TTN_FF_MT_THS 0x17
#define TTN_FF_MT_COUNT 0x18
#define TTN_TRANSIENT_CFG 0x1D
#define TTN_TRANSIENT_SRC 0x1E
#define TTN_TRANSIENT_THS 0x1F
#define TTN_TRANSIENT_COUNT 0x20
#define TTN_ASLP_CNT 0x29
#define TTN_CTRL_REG1 0x2A
#define TTN_CTRL_REG2 0x2B
#define TTN_CTRL_REG3 0x2C
#define TTN_CTRL_REG4 0x2D
#define TTN_CTRL_REG5 0x2E

Hackscribble_MCP9804 TTN_TEMPERATURE_SENSOR(TTN_TEMPERATURE_SENSOR_ADDRESS);

void (*TTN_TEMPERATURE)(void);
void (*TTN_MOTION_START)(void);
void (*TTN_MOTION_STOP)(void);
void (*TTN_BUTTON_PRESS)(void);
void (*TTN_BUTTON_RELEASE)(void);

bool TTN_MOTION_MOVING = false;
bool TTN_BUTTON_PRESSED = false;

void TTN_MOTION_CALLBACK()
{
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(TTN_ACCELEROMETER_INT2));

  if (trigger == RISING)
  {
    // prevent multiple starts
    if (TTN_MOTION_MOVING)
    {
      return;
    }
    TTN_MOTION_MOVING = true;
    if (TTN_MOTION_START)
    {
      TTN_MOTION_START();
    }
  }
  else if(trigger == FALLING)
  {
    // prevent multiple ends and initial FALLING after init
    if (!TTN_MOTION_MOVING)
    {
      return;
    }
    TTN_MOTION_MOVING = false;
    if (TTN_MOTION_STOP)
    {
      TTN_MOTION_STOP();
    }
  }
}

void TTN_TEMPERATURE_CALLBACK()
{
  if (TTN_TEMPERATURE)
  {
    TTN_TEMPERATURE();
  }
  
  TTN_TEMPERATURE_SENSOR.clearAlert();
}

void TTN_BUTTON_CALLBACK()
{
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(TTN_BUTTON));

  if(trigger == FALLING)
  {
    // prevent multiple starts
    if (TTN_BUTTON_PRESSED)
    {
      return;
    }
    TTN_BUTTON_PRESSED = true;
    if (TTN_BUTTON_PRESS)
    {
      TTN_BUTTON_PRESS();
    }
  }
  else if (trigger == RISING)
  {
    // prevent multiple starts
    if (!TTN_BUTTON_PRESSED)
    {
      return;
    }
    TTN_BUTTON_PRESSED = false;
    if (TTN_BUTTON_RELEASE)
    {
      TTN_BUTTON_RELEASE();
    }
  }
}

/******************************************************************************
 * PUBLIC
 *****************************************************************************/

TheThingsNode::TheThingsNode() 
{
  Serial.begin(9600);

  initLight();
  initButton();
  initLED();
  initBattery();
}

void TheThingsNode::showStatus()
{
  Serial.print(F("Light: "));
  Serial.println(String(getLight()));
  Serial.print(F("Temperature as int: "));
  Serial.println(String(getTemperatureAsInt()) + F(" C"));
  Serial.print(F("Temperature as float: "));
  Serial.println(String(getTemperatureAsFloat()) + F(" C"));
  Serial.print(F("Moving: "));
  Serial.println(isMoving() ? F("Yes") : F("No"));
  Serial.print(F("Button pressed: "));
  Serial.println(isButtonPressed() ? F("Yes") : F("No"));
  Serial.print(F("Color: "));
  Serial.println(colorToString(getColor()));
  Serial.print(F("USB connected: "));
  Serial.println(getUSB() ? F("Yes") : F("No"));
  Serial.print(F("Battery voltage: "));
  Serial.println(String(getBattery()) + F(" MV"));
}

/******************************************************************************
 * LIGHT
 */

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

/******************************************************************************
 * TEMPERATURE
 */

int8_t TheThingsNode::getTemperatureAsInt()
{
  enableTemperature();
  int8_t value = TTN_TEMPERATURE_SENSOR.getTAInteger();
  if (!this->temperatureAlertEnabled)
  {
    disableTemperature();
  }
  return value;
}

float TheThingsNode::getTemperatureAsFloat()
{
  enableTemperature();
  int8_t value = TTN_TEMPERATURE_SENSOR.getTAFloat();
  if (!this->temperatureAlertEnabled)
  {
    disableTemperature();
  }
  return value;
}

void TheThingsNode::configTemperature(MCP9804_Resolution resolution)
{
  TTN_TEMPERATURE_SENSOR.setResolution(resolution);
}

void TheThingsNode::onTemperatureAlert(void (*callback)(void))
{
  TTN_TEMPERATURE = callback;

  if (this->temperatureAlertEnabled == true)
  {
    catchCurrentTemperatureAlert();
  }
  // no need to do this again
  else
  {
    configTemperatureAlert(true);
  }
}

void TheThingsNode::configTemperatureAlert(bool enabled)
{
  if (this->temperatureAlertEnabled == enabled)
  {
    return;
  }

  if (enabled)
  {
    // only need to do this once
    if (this->temperatureEnabled != false)
    {
      TTN_TEMPERATURE_SENSOR.configureAlert();
    }

    attachPCINT(digitalPinToPCINT(TTN_TEMPERATURE_ALERT), TTN_TEMPERATURE_CALLBACK, FALLING);

    catchCurrentTemperatureAlert();
  }
  else
  {
    // no need to do this when it was never enabled
    if (this->temperatureAlertEnabled == true)
    {
      detachPCINT(digitalPinToPCINT(TTN_TEMPERATURE_ALERT));
    }

    disableTemperature();
  }

  this->temperatureAlertEnabled = enabled;
}

void TheThingsNode::configTemperatureAlert(bool enabled, int8_t lower, int8_t upper, int8_t critical, MCP9804_Hysteresis hysteresis)
{
  TTN_TEMPERATURE_SENSOR.setTLOWER(lower);
  TTN_TEMPERATURE_SENSOR.setTUPPER(upper);
  TTN_TEMPERATURE_SENSOR.setTCRIT(critical);
  TTN_TEMPERATURE_SENSOR.setHysteresis(hysteresis);

  configTemperatureAlert(enabled);
}

/******************************************************************************
 * MOTION
 */

void TheThingsNode::onMotionStart(void (*callback)(void))
{
  configMotion(true);

  TTN_MOTION_START = callback;
}

void TheThingsNode::onMotionStop(void (*callback)(void))
{
  configMotion(true);

  TTN_MOTION_STOP = callback;
}

bool TheThingsNode::isMoving()
{
  return TTN_MOTION_MOVING;
}

void TheThingsNode::configMotion(bool enabled)
{
  if (this->motionEnabled == enabled)
  {
    return;
  }

  if (enabled)
  {
    // first time, when motionEnabled is undefined
    if (this->motionEnabled != false)
    {
      // describes motion interrupt setup for low power:
      // http://arduino.stackexchange.com/questions/1475/setting-up-the-mma8452-to-trigger-interrupt
      writeMotion(TTN_CTRL_REG1, (0 | TTN_DR << 3) | TTN_SR << 6); //DR and SR defined data rate and sleep rate
      writeMotion(TTN_CTRL_REG2, 0x1F);  //LP mode in sleep and active, autosleep on
      writeMotion(TTN_ASLP_CNT, TTN_SC);  //defined sleep count
      writeMotion(TTN_CTRL_REG3, 0x42); //Transient interrupt
      writeMotion(TTN_CTRL_REG4, 0x20); //Transient interrupt source on
      writeMotion(TTN_CTRL_REG5, 0x20); //Transient to pin INT1, the rest to INT2

      writeMotion(TTN_TRANSIENT_CFG, 0x0E); //Flag latch disabled, motion on all axes
      writeMotion(TTN_TRANSIENT_THS, TTN_MT); //Motion threshold, debounce in inc/dec mode
      writeMotion(TTN_TRANSIENT_COUNT, TTN_MDC);  //Motion delay
    }

    writeMotion(TTN_CTRL_REG1, (0x01 | TTN_DR << 3) | TTN_SR << 6); //Put ACC in active mode
    delay(300);
    readMotion(TTN_TRANSIENT_SRC);

    attachPCINT(digitalPinToPCINT(TTN_ACCELEROMETER_INT2), TTN_MOTION_CALLBACK, CHANGE);
  }
  else
  {
    // not on first time, when motionEnabled is undefined
    if (this->motionEnabled == true)
    {
      detachPCINT(digitalPinToPCINT(TTN_ACCELEROMETER_INT2));
    }

    writeMotion(TTN_CTRL_REG1, (0 | TTN_DR << 3) | TTN_SR << 6); //Put ACC in standby mode
  }

  this->motionEnabled = enabled;
}

/******************************************************************************
 * BUTTON
 */

void TheThingsNode::onButtonPress(void (*callback)(void))
{
  TTN_BUTTON_PRESS = callback;
}

void TheThingsNode::onButtonRelease(void (*callback)(void))
{
  TTN_BUTTON_RELEASE = callback;
}

bool TheThingsNode::isButtonPressed()
{
  return TTN_BUTTON_PRESSED;
}

/******************************************************************************
 * LED
 */

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

/******************************************************************************
 * BATTERY
 */

uint16_t TheThingsNode::getBattery()
{
  digitalWrite(TTN_VBAT_MEAS_EN, LOW);
  uint16_t val = analogRead(TTN_VBAT_MEAS);
  digitalWrite(TTN_VBAT_MEAS_EN, HIGH);
  uint16_t batteryVoltage = map(val,0,1024,0,3300) * 2; // *2 for voltage divider
  return batteryVoltage;
}

/******************************************************************************
 * USB
 */

bool TheThingsNode::getUSB()
{
  return USBSTA&(1<<VBUS);
}

/******************************************************************************
 * PRIVATE
 *****************************************************************************/

void TheThingsNode::initLight()
{ 
  pinMode(TTN_LDR_GAIN1, OUTPUT);
  pinMode(TTN_LDR_GAIN2, OUTPUT);

  digitalWrite(TTN_LDR_GAIN1, LOW);
  digitalWrite(TTN_LDR_GAIN2, LOW);

  configLight(1);
}

void TheThingsNode::initButton()
{
  pinMode(TTN_BUTTON, INPUT);
  digitalWrite(TTN_BUTTON, HIGH);

  attachPCINT(digitalPinToPCINT(TTN_BUTTON), TTN_BUTTON_CALLBACK, CHANGE);
}

void TheThingsNode::initLED()
{
  pinMode(TTN_RED_LED, OUTPUT);
  pinMode(TTN_GREEN_LED, OUTPUT);
  pinMode(TTN_BLUE_LED, OUTPUT);
}

void TheThingsNode::initBattery()
{
  pinMode(TTN_VBAT_MEAS_EN, OUTPUT);
  digitalWrite(TTN_VBAT_MEAS_EN, HIGH);
}

void TheThingsNode::enableTemperature()
{
  if (this->temperatureEnabled == true)
  {
    return;
  }

  uint16_t configurationRegister = TTN_TEMPERATURE_SENSOR._readRegister16(REG_CONFIG);
  configurationRegister &= ~0x100;
  TTN_TEMPERATURE_SENSOR._writeRegister16(REG_CONFIG,configurationRegister);  
  delay(255);

  this->temperatureEnabled = true;
}

void TheThingsNode::disableTemperature()
{
  if (this->temperatureEnabled == false)
  {
    return;
  }

  uint16_t configurationRegister = TTN_TEMPERATURE_SENSOR._readRegister16(REG_CONFIG);
  configurationRegister |= 0x100;
  TTN_TEMPERATURE_SENSOR._writeRegister16(REG_CONFIG,configurationRegister);

  this->temperatureEnabled = false;
}

void TheThingsNode::catchCurrentTemperatureAlert()
{
  if (!TTN_TEMPERATURE_CALLBACK)
  {
    return;
  }

  // From testing, it appears that if Ta is already out of range of the defined thresholds at startup, the Alert output is not activated.
  int8_t value = TTN_TEMPERATURE_SENSOR.getTAInteger();
  if ((value < TTN_TEMPERATURE_SENSOR.getTLOWER()) || (value > TTN_TEMPERATURE_SENSOR.getTUPPER()) || (value > TTN_TEMPERATURE_SENSOR.getTCRIT()))
  {
    TTN_TEMPERATURE_CALLBACK();
  }
}

void TheThingsNode::writeMotion(unsigned char REG_ADDRESS, unsigned  char DATA)  //SEND data to MMA8652
{
  Wire.beginTransmission(TTN_ADDR_ACC);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}

uint8_t TheThingsNode::readMotion(unsigned char REG_ADDRESS)
{
  uint8_t resp;
  Wire.beginTransmission(TTN_ADDR_ACC);
  Wire.write(REG_ADDRESS);
  Wire.endTransmission(false);
  Wire.requestFrom(TTN_ADDR_ACC, 1);
  resp = Wire.read();
  return resp;
}