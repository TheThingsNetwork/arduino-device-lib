// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifdef ARDUINO_ARCH_AVR

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
#define TTN_DR 5  // active data rate
#define TTN_SR 3  // sleep data rate
#define TTN_SC 4  // sleep delay
#define TTN_MT 4  // 0.063g/LSB
#define TTN_MDC 2 // debounce delay in samples
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

bool TTN_TEMPERATURE = false;
bool TTN_MOTION_START = false;
bool TTN_MOTION_STOP = false;
bool TTN_BUTTON_PRESS = false;
bool TTN_BUTTON_RELEASE = false;
uint32_t TTN_INTERVAL = 0;

void TTN_TEMPERATURE_FN()
{
  TTN_TEMPERATURE = true;
  TTN_TEMPERATURE_SENSOR.clearAlert();
}

void TTN_MOTION_FN()
{
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(TTN_ACCELEROMETER_INT2));
  if (trigger == RISING)
  {
    TTN_MOTION_START = true;
  }
  else if (trigger == FALLING)
  {
    TTN_MOTION_STOP = true;
  }
}

void TTN_BUTTON_FN()
{
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(TTN_BUTTON));
  if (trigger == FALLING)
  {
    TTN_BUTTON_PRESS = true;
  }
  else if (trigger == RISING)
  {
    TTN_BUTTON_RELEASE = true;
  }
}

ISR(WDT_vect)
{
  TTN_INTERVAL = TTN_INTERVAL + 8000;
}

/******************************************************************************
 * PUBLIC
 *****************************************************************************/

void TheThingsNode::onWake(void (*callback)(void))
{
  this->wakeCallback = callback;
}

void TheThingsNode::loop()
{
  // USB is connected and last time we checked it wasn't or visa versa
  if (this->isUSBConnected() == this->wasUSBDisconnected)
  {
    this->wasUSBDisconnected = !this->isUSBConnected();

    // It is no longer connected
    if (this->wasUSBDisconnected)
    {
      Serial.end();
      WDT_start();
    }
    else
    {

      // Stop watchdog unless deep sleep is enabled for USB
      if (!this->USBDeepSleep)
      {
        WDT_stop();
      }

      // Restore communication
      if (Serial)
      {
        Serial.begin(9600);
      }
    }
  }

  if (this->wakeCallback)
  {
    this->wakeCallback();
  }

  if (TTN_BUTTON_PRESS)
  {
    if (!this->buttonPressed)
    {
      this->buttonPressedAt = millis();
      if (this->buttonEnabled && this->buttonPressCallback)
      {
        this->buttonPressCallback();
      }
      this->buttonPressed = true;
    }
    TTN_BUTTON_PRESS = false;
  }

  if (TTN_BUTTON_RELEASE)
  {
    if (this->buttonPressed)
    {
      if (this->buttonEnabled && this->buttonReleaseCallback)
      {
        this->buttonReleaseCallback(millis() - this->buttonPressedAt);
      }
      this->buttonPressed = false;
    }
    TTN_BUTTON_RELEASE = false;
  }

  if (TTN_MOTION_START)
  {
    if (!this->motionStarted)
    {
      this->motionStartedAt = millis();
      if (this->motionEnabled && this->motionStartCallback)
      {
        this->motionStartCallback();
      }
      this->motionStarted = true;
    }
    TTN_MOTION_START = false;
  }

  if (TTN_MOTION_STOP)
  {
    if (this->motionStarted)
    {
      if (this->motionEnabled && this->motionStopCallback)
      {
        this->motionStopCallback(millis() - this->motionStartedAt);
      }
      this->motionStarted = false;
    }
    TTN_MOTION_STOP = false;
  }

  if (TTN_TEMPERATURE)
  {
    if (this->temperatureEnabled && this->temperatureCallback)
    {
      this->temperatureCallback();
    }
    TTN_TEMPERATURE = false;
  }

  if (TTN_INTERVAL >= this->intervalMs)
  {
    if (this->intervalEnabled && this->intervalCallback)
    {
      this->intervalCallback();
    }
    TTN_INTERVAL = 0;
  }

  if (this->sleepCallback)
  {
    this->sleepCallback();
  }

  if (this->isUSBConnected() && !this->USBDeepSleep)
  {

    while (!TTN_BUTTON_PRESS && !TTN_BUTTON_RELEASE && !TTN_MOTION_START && !TTN_MOTION_STOP && !TTN_TEMPERATURE && TTN_INTERVAL < this->intervalMs)
    {
      delay(100);
      TTN_INTERVAL = TTN_INTERVAL + 100;
    }
  }
  else
  {
    Serial.flush();
    deepSleep();
  }
}

void TheThingsNode::onSleep(void (*callback)(void))
{
  this->sleepCallback = callback;
}

void TheThingsNode::showStatus()
{
  Serial.print(F("Light: "));
  if (this->lightEnabled)
  {
    Serial.println(String(getLight()));
  }
  else
  {
    Serial.println(F("disabled"));
  }
  Serial.print(F("Temperature: "));
  Serial.println(String(getTemperatureAsFloat()) + F(" C"));
  Serial.print(F("Temperature alert: "));
  if (this->temperatureEnabled)
  {
    Serial.println(hasTemperatureAlert() ? F("Yes") : F("No"));
  }
  else
  {
    Serial.println(F("disabled"));
  }
  Serial.print(F("Moving: "));
  if (this->motionEnabled)
  {
    Serial.println(isMoving() ? F("Yes") : F("No"));
  }
  else
  {
    Serial.println(F("disabled"));
  }
  Serial.print(F("Button pressed: "));
  if (this->buttonEnabled)
  {
    Serial.println(isButtonPressed() ? F("Yes") : F("No"));
  }
  else
  {
    Serial.println(F("disabled"));
  }
  Serial.print(F("Color: "));
  Serial.println(colorToString(getColor()));
  Serial.print(F("USB connected: "));
  Serial.println(isUSBConnected() ? F("Yes") : F("No"));
  Serial.print(F("Battery voltage: "));
  Serial.println(String(getBattery()) + F(" MV"));
}

/******************************************************************************
 * INTERVAL
 */

void TheThingsNode::configInterval(bool enabled, uint32_t ms)
{
  this->intervalMs = ms;

  configInterval(enabled);
}

void TheThingsNode::configInterval(bool enabled)
{
  this->intervalEnabled = enabled;
}

void TheThingsNode::onInterval(void (*callback)(void))
{
  this->intervalCallback = callback;

  configInterval(true);
}

/******************************************************************************
 * LIGHT
 */

void TheThingsNode::configLight(bool enabled, uint8_t gain)
{
  this->lightGain = gain;

  configLight(enabled);
}

void TheThingsNode::configLight(bool enabled)
{
  if (enabled == this->lightEnabled)
  {
    return;
  }

  if (enabled)
  {
    switch (this->lightGain)
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
  else
  {
    digitalWrite(TTN_LDR_GAIN1, LOW);
    digitalWrite(TTN_LDR_GAIN2, LOW);
  }

  this->lightEnabled = enabled;
}

uint16_t TheThingsNode::getLight()
{
  // TODO: if (this->lightEnabled)

  return analogRead(TTN_LDR_INPUT);
}

/******************************************************************************
 * TEMPERATURE
 */

void TheThingsNode::configTemperature(bool enabled, MCP9804_Resolution resolution, int8_t lower, int8_t upper, int8_t critical, MCP9804_Hysteresis hysteresis)
{
  wakeTemperature();
  TTN_TEMPERATURE_SENSOR.setTLOWER(lower);
  TTN_TEMPERATURE_SENSOR.setTUPPER(upper);
  TTN_TEMPERATURE_SENSOR.setTCRIT(critical);
  TTN_TEMPERATURE_SENSOR.setHysteresis(hysteresis);
  configTemperature(enabled, resolution);
  sleepTemperature();
}

void TheThingsNode::configTemperature(bool enabled, MCP9804_Resolution resolution)
{
  wakeTemperature();
  TTN_TEMPERATURE_SENSOR.setResolution(resolution);
  configTemperature(enabled);
  sleepTemperature();
}

void TheThingsNode::configTemperature(bool enabled)
{
  if (this->temperatureEnabled == enabled)
  {
    return;
  }

  wakeTemperature();

  if (enabled)
  {
    TTN_TEMPERATURE_SENSOR.configureAlert(true);
    attachPCINT(digitalPinToPCINT(TTN_TEMPERATURE_ALERT), TTN_TEMPERATURE_FN, FALLING);

    if (hasTemperatureAlert() && this->temperatureCallback)
    {
      this->temperatureCallback();
    }
  }
  else
  {
    TTN_TEMPERATURE_SENSOR.configureAlert(false);

    detachPCINT(digitalPinToPCINT(TTN_TEMPERATURE_ALERT));

    sleepTemperature();
  }

  this->temperatureEnabled = enabled;
}

void TheThingsNode::onTemperature(void (*callback)(void))
{
  this->temperatureCallback = callback;

  if (this->temperatureEnabled)
  {
    if (hasTemperatureAlert())
    {
      callback();
    }
  }
  else
  {
    configTemperature(true);
  }
}

int8_t TheThingsNode::getTemperatureAsInt()
{
  wakeTemperature();
  int8_t value = TTN_TEMPERATURE_SENSOR.getTAInteger();
  sleepTemperature();
  return value;
}

float TheThingsNode::getTemperatureAsFloat()
{
  wakeTemperature();
  float value = TTN_TEMPERATURE_SENSOR.getTA();
  sleepTemperature();
  return value;
}

bool TheThingsNode::hasTemperatureAlert()
{
  // From testing, it appears that if Ta is already out of range of the defined thresholds at startup, the Alert output is not activated.
  int8_t value = getTemperatureAsInt();
  return ((value < TTN_TEMPERATURE_SENSOR.getTLOWER()) || (value > TTN_TEMPERATURE_SENSOR.getTUPPER()) || (value > TTN_TEMPERATURE_SENSOR.getTCRIT()));
}

/******************************************************************************
 * MOTION
 */

void TheThingsNode::configMotion(bool enabled)
{
  if (this->motionEnabled == enabled)
  {
    return;
  }

  if (enabled)
  {
    wakeMotion();
    attachPCINT(digitalPinToPCINT(TTN_ACCELEROMETER_INT2), TTN_MOTION_FN, CHANGE);
  }
  else
  {
    detachPCINT(digitalPinToPCINT(TTN_ACCELEROMETER_INT2));
    sleepMotion();
  }

  this->motionEnabled = enabled;
}

void TheThingsNode::onMotionStart(void (*callback)(void))
{
  configMotion(true);

  this->motionStartCallback = callback;
}

void TheThingsNode::onMotionStop(void (*callback)(unsigned long duration))
{
  configMotion(true);

  this->motionStopCallback = callback;
}

bool TheThingsNode::isMoving()
{
  return this->motionStarted;
}

/******************************************************************************
 * BUTTON
 */

void TheThingsNode::configButton(bool enabled)
{
  if (this->buttonEnabled == enabled)
  {
    return;
  }

  if (enabled)
  {
    attachPCINT(digitalPinToPCINT(TTN_BUTTON), TTN_BUTTON_FN, CHANGE);
  }
  else
  {
    detachPCINT(digitalPinToPCINT(TTN_BUTTON));
  }

  this->buttonEnabled = enabled;
}

void TheThingsNode::onButtonPress(void (*callback)(void))
{
  this->buttonPressCallback = callback;

  configButton(true);
}

void TheThingsNode::onButtonRelease(void (*callback)(unsigned long duration))
{
  this->buttonReleaseCallback = callback;

  configButton(true);
}

bool TheThingsNode::isButtonPressed()
{
  return this->buttonPressed;
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

ttn_color TheThingsNode::getColor()
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

String TheThingsNode::colorToString(ttn_color color)
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

void TheThingsNode::setColor(ttn_color color)
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
  uint16_t batteryVoltage = map(val, 0, 1024, 0, 3300) * 2; // *2 for voltage divider
  return batteryVoltage;
}

/******************************************************************************
 * USB
 */

void TheThingsNode::configUSB(bool deepSleep)
{
  this->USBDeepSleep = deepSleep;

  if (this->USBDeepSleep || !isUSBConnected())
  {
    WDT_start();
  }
  else
  {
    WDT_stop();
  }
}

bool TheThingsNode::isUSBConnected()
{
  return USBSTA & (1 << VBUS);
}

/******************************************************************************
 * PRIVATE
 *****************************************************************************/

TheThingsNode::TheThingsNode()
{
  configInterval(false, 60000);

  pinMode(TTN_LDR_GAIN1, OUTPUT);
  pinMode(TTN_LDR_GAIN2, OUTPUT);
  configLight(false, 1);

  configTemperature(false, R_DEGREES_0_0625, 0, 30, 55, H_DEGREES_0_0);

  configMotion(false);

  pinMode(TTN_BUTTON, INPUT);
  digitalWrite(TTN_BUTTON, HIGH);

  pinMode(TTN_RED_LED, OUTPUT);
  pinMode(TTN_GREEN_LED, OUTPUT);
  pinMode(TTN_BLUE_LED, OUTPUT);
  setColor(TTN_BLACK);

  // TODO: Can we enable/disable this at will to save memory?
  USBCON |= (1 << OTGPADE);

  pinMode(TTN_VBAT_MEAS_EN, OUTPUT);
  digitalWrite(TTN_VBAT_MEAS_EN, HIGH);

  if (!isUSBConnected() || this->USBDeepSleep)
  {
    WDT_start();
  }
}

void TheThingsNode::sleepTemperature()
{
  if (this->temperatureSleep || this->temperatureEnabled)
  {
    return;
  }

  TTN_TEMPERATURE_SENSOR.setMode(MODE_SHUTDOWN);

  this->temperatureSleep = true;
}

void TheThingsNode::wakeTemperature()
{
  if (!this->temperatureSleep)
  {
    return;
  }

  TTN_TEMPERATURE_SENSOR.setMode(MODE_CONTINUOUS);

  this->temperatureSleep = false;
}

void TheThingsNode::sleepMotion()
{
  writeMotion(TTN_CTRL_REG1, (0 | TTN_DR << 3) | TTN_SR << 6); //Put ACC in standby mode
}

void TheThingsNode::wakeMotion()
{
  // describes motion interrupt setup for low power:
  // http://arduino.stackexchange.com/questions/1475/setting-up-the-mma8452-to-trigger-interrupt
  writeMotion(TTN_CTRL_REG1, (0 | TTN_DR << 3) | TTN_SR << 6); //DR and SR defined data rate and sleep rate
  writeMotion(TTN_CTRL_REG2, 0x1F);                            //LP mode in sleep and active, autosleep on
  writeMotion(TTN_ASLP_CNT, TTN_SC);                           //defined sleep count
  writeMotion(TTN_CTRL_REG3, 0x42);                            //Transient interrupt
  writeMotion(TTN_CTRL_REG4, 0x20);                            //Transient interrupt source on
  writeMotion(TTN_CTRL_REG5, 0x20);                            //Transient to pin INT1, the rest to INT2
  writeMotion(TTN_TRANSIENT_CFG, 0x0E);                        //Flag latch disabled, motion on all axes
  writeMotion(TTN_TRANSIENT_THS, TTN_MT);                      //Motion threshold, debounce in inc/dec mode
  writeMotion(TTN_TRANSIENT_COUNT, TTN_MDC);                   //Motion delay

  writeMotion(TTN_CTRL_REG1, (0x01 | TTN_DR << 3) | TTN_SR << 6); //Put ACC in active mode
  delay(300);
  readMotion(TTN_TRANSIENT_SRC);
}

void TheThingsNode::writeMotion(unsigned char REG_ADDRESS, unsigned char DATA) //SEND data to MMA8652
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

void TheThingsNode::WDT_start()
{
  if (this->wdtStarted)
  {
    return;
  }

  cli();
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = 1 << WDP0 | 0 << WDP1 | 0 << WDP2 | 1 << WDP3; /* 2.0 seconds */
  WDTCSR |= _BV(WDIE);
  sei();

  this->wdtStarted = true;
}

void TheThingsNode::WDT_stop()
{
  if (!this->wdtStarted)
  {
    return;
  }

  cli();
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = 0x00;
  WDTCSR = 0 << WDP0 | 1 << WDP1 | 0 << WDP2 | 0 << WDP3;
  sei();

  this->wdtStarted = true;
}

void TheThingsNode::deepSleep(void)
{
  ADCSRA &= ~_BV(ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  MCUCR |= (1 << JTD);
  USBCON |= (1 << FRZCLK);
  //USBCON &= ~_BV(USBE);
  PLLCSR &= ~_BV(PLLE);
  sleep_enable();
  sleep_mode(); //Sweet dreams!

  //wake up, after ISR we arrive here ->
  sleep_disable();
  PLLCSR |= (1 << PLLE);
  power_all_enable();
  //USBCON |= (1 << USBE);
  USBCON &= ~_BV(FRZCLK);
  ADCSRA |= (1 << ADEN);
}

#endif