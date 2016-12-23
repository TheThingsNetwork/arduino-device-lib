/*

	Hackscribble_MCP9804 Library
	============================

	Comprehensive Arduino library for Microchip Technology's
	MCP9804 I2C digital temperature sensor.


	Created on 13 November 2014
	By Ray Benitez
	Last modified on 7 December 2016
	By Ray Benitez
	Change history in "README.md"

	This software is licensed by Ray Benitez under the MIT License.

	git@hackscribble.com | http://www.hackscribble.com | http://www.twitter.com/hackscribble

*/


//
// Uncomment statement below to enable debug print statements for register read errors
//

// #define MCP9804_DEBUG


#include "Arduino.h"
#include <Hackscribble_MCP9804.h>


Hackscribble_MCP9804::Hackscribble_MCP9804(uint8_t address): _address(address)
{
	// Nil
}


//
// Private methods
//

uint8_t Hackscribble_MCP9804::_readRegister8(MCP9804_Register reg)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();
  if (Wire.requestFrom(_address, (uint8_t)1) == 1)
  {
    return Wire.read();
  }
  else
  {
#ifdef MCP9804_DEBUG
    Serial.print("Hackscribble_MCP9804: error reading register 0x");
    Serial.println(reg, HEX);
#endif
  }
}


void Hackscribble_MCP9804::_writeRegister8(MCP9804_Register reg, uint8_t val)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}


uint16_t Hackscribble_MCP9804::_readRegister16(MCP9804_Register reg)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();
  if (Wire.requestFrom(_address, (uint8_t)2) == 2)
  {
    uint8_t MSB = Wire.read();
    uint8_t LSB = Wire.read();
    return (MSB * 256 + LSB);
  }
  else
  {
#ifdef MCP9804_DEBUG
    Serial.print("Hackscribble_MCP9804: error reading register 0x");
    Serial.println(reg, HEX);
#endif
  }
}


void Hackscribble_MCP9804::_writeRegister16(MCP9804_Register reg, uint16_t val)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(val / 256);
  Wire.write(val % 256);
  Wire.endTransmission();
}


int8_t Hackscribble_MCP9804::_getThresholdInteger(MCP9804_Register reg)
{
  uint16_t raw = _readRegister16(reg);
  uint8_t temp = (((raw & 0x0FFF) >> 3) + 1) >> 1; // Round to nearest integer
  if (temp & 0x80)
  {
    return -(256 - temp);
  }
  else
  {
    return temp;
  }
}


float Hackscribble_MCP9804::_getThresholdFloat(MCP9804_Register reg)
{
	uint16_t raw = _readRegister16(reg);
	raw &= 0x0FFC;
	float temp = raw / 16.0;
	if (raw & 0x0800)
	{
		return -(256.0 - temp);
	}
	else
	{
		return temp;
	}
}


boolean Hackscribble_MCP9804::_setThresholdInteger(MCP9804_Register reg, int8_t val)
{
  // For Tupper and Tcrit, alert is triggered when Ta:
  //  1. rises above threshold, and then
  //  2. falls back below (threshold - hysteresis)
  // For Tlower, alert is triggered when Ta:
  //  1. falls below (threshold - hysteresis), and then
  //  2. rises back above threshold

  uint16_t temp = (uint8_t)val << 4;
  if (val < 0)
  {
	  temp |= 0x1000;
  }
  //temp |= (val < 0 ? 0x1000 : 0x0000);
  _writeRegister16(reg, temp);
  return (_getThresholdInteger(reg) == val);
}


boolean Hackscribble_MCP9804::_setThresholdFloat(MCP9804_Register reg, float val)
{
	// For Tupper and Tcrit, alert is triggered when Ta:
	//  1. rises above threshold, and then
	//  2. falls back below (threshold - hysteresis)
	// For Tlower, alert is triggered when Ta:
	//  1. falls below (threshold - hysteresis), and then
	//  2. rises back above threshold

	float tempF = val;
	if (val < 0)
	{
		tempF += 256.0;
	}
	uint16_t tempI = (uint16_t)((tempF * 4.0) + 0.5);
	tempI = (tempI << 2) & 0x0FFC;
	if (val < 0)
	{
		tempI |= 0x1000;
	}
	_writeRegister16(reg, tempI);
  // Values are rounded to 0.25 degrees before writing
  // Check that read value minus requested value is within half of this 
	return (abs(_getThresholdFloat(reg) - val) <= 0.125);
}


//
// Public methods
//

void Hackscribble_MCP9804::begin()
{
	Wire.begin();
#ifdef MCP9804_DEBUG
	Serial.println("Hackscribble_MCP9804: debug enabled");
#endif
}


int8_t Hackscribble_MCP9804::getTAInteger()
{
  uint8_t temp = (((_readRegister16(REG_TA) & 0x0FFF) >> 3) + 1) >> 1; // Round to nearest integer
  if (temp & 0x80)
  {
    return -(256 - temp);
  }
  else
  {
    return temp;
  }
}


float Hackscribble_MCP9804::getTA()
{
  uint8_t numBinaryPlaces = (uint8_t) getResolution() + 1;
  uint16_t rawTA = _readRegister16(REG_TA);
  float temp = (float)((rawTA & 0x0FFF) >> (4 - numBinaryPlaces)) / (1 << numBinaryPlaces);
  if ((rawTA & 0x1000) > 0)
  {
    return -(256 - temp);
  }
  else
  {
    return temp;
  }
}


MCP9804_Resolution Hackscribble_MCP9804::getResolution()
{
  return (MCP9804_Resolution)(_readRegister8(REG_RESOLUTION) & 0x03);
}


void Hackscribble_MCP9804::setResolution(MCP9804_Resolution res)
{
  _writeRegister8(REG_RESOLUTION, res & 0x03);
}


MCP9804_Hysteresis Hackscribble_MCP9804::getHysteresis()
{
  return (MCP9804_Hysteresis)((_readRegister16(REG_CONFIG) & 0x0600) >> 9);
}


boolean Hackscribble_MCP9804::setHysteresis(MCP9804_Hysteresis val)
{
  // For Tupper and Tcrit, alert is triggered when Ta:
  //  1. rises above threshold, and then
  //  2. falls back below (threshold - hysteresis)
  // For Tlower, alert is triggered when Ta:
  //  1. falls below (threshold - hysteresis), and then
  //  2. rises back above threshold
	uint16_t current = _readRegister16(REG_CONFIG);
	current &= 0xF9FF;
	current |= ((uint16_t)val << 9);
	_writeRegister16(REG_CONFIG, current);
	return (_readRegister16(REG_CONFIG) == current);
}


float Hackscribble_MCP9804::getTUPPER()
{
	return _getThresholdFloat(REG_TUPPER);
}


boolean Hackscribble_MCP9804::setTUPPER(int8_t val)
{
  	return _setThresholdInteger(REG_TUPPER, val);
}


boolean Hackscribble_MCP9804::setTUPPER(float val)
{
	return _setThresholdFloat(REG_TUPPER, val);
}


float Hackscribble_MCP9804::getTLOWER()
{
	return _getThresholdFloat(REG_TLOWER);
}


boolean Hackscribble_MCP9804::setTLOWER(int8_t val)
{
	return _setThresholdInteger(REG_TLOWER, val);
}


boolean Hackscribble_MCP9804::setTLOWER(float val)
{
	return _setThresholdFloat(REG_TLOWER, val);
}


float Hackscribble_MCP9804::getTCRIT()
{
	return _getThresholdFloat(REG_TCRIT);
}


boolean Hackscribble_MCP9804::setTCRIT(int8_t val)
{
	return _setThresholdInteger(REG_TCRIT, val);
}


boolean Hackscribble_MCP9804::setTCRIT(float val)
{
	return _setThresholdFloat(REG_TCRIT, val);
}


boolean Hackscribble_MCP9804::configureAlert()
{
	uint16_t current = _readRegister16(REG_CONFIG) & 0xFFF0;
	// Alert enabled for upper and lower, active low, interrupt mode
  current |= (ALERT_ENABLE | ALERT_ALL | ALERT_LOW | ALERT_INTERRUPT);
	_writeRegister16(REG_CONFIG, current);
	return (_readRegister16(REG_CONFIG) == current);
}


boolean Hackscribble_MCP9804::configureAlert(boolean action, uint16_t settings)
{
  	uint16_t current = _readRegister16(REG_CONFIG) & 0xFFF0;
  	if (action == ENABLE)
  	{
    		current |= (ALERT_ENABLE | settings);
  	}
  	else
  	{
    		current |= ALERT_DISABLE;
  	}
  	_writeRegister16(REG_CONFIG, current);
  	return (_readRegister16(REG_CONFIG) == current);
}


boolean Hackscribble_MCP9804::alertTCRIT()
{
  return ((_readRegister16(REG_TA) & 0x8000) > 0);
}


boolean Hackscribble_MCP9804::alertTUPPER()
{
	return ((_readRegister16(REG_TA) & 0x4000) > 0);
}


boolean Hackscribble_MCP9804::alertTLOWER()
{
	return ((_readRegister16(REG_TA) & 0x2000) > 0);
}


void Hackscribble_MCP9804::clearAlert()
{
	uint16_t current = _readRegister16(REG_CONFIG);
	current |= 0x0020;
	_writeRegister16(REG_CONFIG, current);
}


uint16_t Hackscribble_MCP9804::getManufacturerID()
{
	return (_readRegister16(REG_MANUFACTURER_ID));
}


uint8_t Hackscribble_MCP9804::getDeviceID()
{
	return (_readRegister16(REG_DEVICE_ID) / 256);
}


uint8_t Hackscribble_MCP9804::getDeviceRevision()
{
	return (_readRegister16(REG_DEVICE_ID) % 256);
}


void Hackscribble_MCP9804::setLockTCRIT()
{
	uint16_t current = _readRegister16(REG_CONFIG);
	current |= 0x0080;
	_writeRegister16(REG_CONFIG, current);
}


boolean Hackscribble_MCP9804::isLockedTCRIT()
{
	return ((_readRegister16(REG_CONFIG) & 0x0080) > 0);
}


void Hackscribble_MCP9804::setLockTWINDOW()
{
	uint16_t current = _readRegister16(REG_CONFIG);
	current |= 0x0040;
	_writeRegister16(REG_CONFIG, current);
}


boolean Hackscribble_MCP9804::isLockedTWINDOW()
{
	return ((_readRegister16(REG_CONFIG) & 0x0040) > 0);
}


boolean Hackscribble_MCP9804::setMode(MCP9804_Mode mode)
{
	uint16_t current = _readRegister16(REG_CONFIG);
	current &= 0xFEFF;
	if (mode == MODE_SHUTDOWN)
	{
		current |= 0x0100;
	}
	_writeRegister16(REG_CONFIG, current);
	return (_readRegister16(REG_CONFIG) == current);
}


MCP9804_Mode Hackscribble_MCP9804::getMode()
{
	uint16_t current = _readRegister16(REG_CONFIG);
	if ((current & 0x0100) > 0)
	{
		return MODE_SHUTDOWN;
	}
	else
	{
		return MODE_CONTINUOUS;
	}
}