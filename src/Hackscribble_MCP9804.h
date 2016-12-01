/*

  Hackscribble_MCP9804 Library
  ============================

  Comprehensive Arduino library for Microchip Technology's
  MCP9804 I2C digital temperature sensor.


  Created on 13 November 2014
  By Ray Benitez
  Last modified on ---
  By ---
  Change history in "README.md"

  This software is licensed by Ray Benitez under the MIT License.

  git@hackscribble.com | http://www.hackscribble.com | http://www.twitter.com/hackscribble

*/

#ifndef HACKSCRIBBLE_MCP9804_H
#define HACKSCRIBBLE_MCP9804_H


#include "Arduino.h"
#include <Wire.h>


enum MCP9804_Resolution
{
  R_DEGREES_0_5000 = 0x00,  // See MCP9804 datasheet section 5.1.6
  R_DEGREES_0_2500 = 0x01,  //
  R_DEGREES_0_1250 = 0x02,  //
  R_DEGREES_0_0625 = 0x03,  //
  numberOfResolutionOptions
};

enum MCP9804_Hysteresis
{
  H_DEGREES_0_0 = 0x00,   // See MCP9804 datasheet section 5.1.1
  H_DEGREES_1_5 = 0x01,   //
  H_DEGREES_3_0 = 0x02,   //
  H_DEGREES_6_0 = 0x03,   //
  numberOfHysteresisOptions
};

enum MCP9804_Register
{
  REG_CONFIG =      0x01, // See MCP9804 datasheet section 5.1
  REG_TUPPER =      0x02, //
  REG_TLOWER =      0x03, //
  REG_TCRIT =       0x04, //
  REG_TA =        0x05, //
  REG_MANUFACTURER_ID =   0x06, //
  REG_DEVICE_ID =     0x07, //
  REG_RESOLUTION =    0x08, //
  numberOfRegisters
};

#define DISABLE          false
#define ENABLE           true
#define ALERT_DISABLE    0x00
#define ALERT_ENABLE     0x08
#define ALERT_ALL        0x00
#define ALERT_TCRIT      0x04
#define ALERT_LOW        0x00
#define ALERT_HIGH       0x02
#define ALERT_COMPARATOR 0x00
#define ALERT_INTERRUPT  0x01


class Hackscribble_MCP9804
{

private:
  uint8_t _readRegister8(MCP9804_Register reg);
  void _writeRegister8(MCP9804_Register reg, uint8_t val);
  int8_t _getThresholdInteger(MCP9804_Register reg);
  void _setThresholdInteger(MCP9804_Register reg, int8_t val);
  uint8_t _address;

public:
  Hackscribble_MCP9804(uint8_t address);
  void begin();
  int8_t getTAInteger();
  float getTAFloat();
  MCP9804_Resolution getResolution();
  void setResolution(MCP9804_Resolution res);
  MCP9804_Hysteresis getHysteresis();
  void setHysteresis(MCP9804_Hysteresis val);
  int8_t getTUPPER();
  void setTUPPER(int8_t val);
  int8_t getTLOWER();
  void setTLOWER(int8_t val);
  int8_t getTCRIT();
  void setTCRIT(int8_t val);
  void configureAlert();
  void configureAlert(boolean action, uint16_t settings = ALERT_ALL | ALERT_LOW | ALERT_INTERRUPT);
  boolean alertTCRIT();
  boolean alertTUPPER();
  boolean alertTLOWER();
  void clearAlert();
  uint16_t getManufacturerID();
  uint8_t getDeviceID();
  uint8_t getDeviceRevision();

  uint16_t _readRegister16(MCP9804_Register reg);
  void _writeRegister16(MCP9804_Register reg, uint16_t val);
};

#endif