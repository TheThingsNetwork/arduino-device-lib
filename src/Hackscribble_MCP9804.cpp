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
    Serial.print("Hackscribble_MCP9804: error reading register 0x");
    Serial.println(reg, HEX);
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
    Serial.print("Hackscribble_MCP9804: error reading register 0x");
    Serial.println(reg, HEX);
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


void Hackscribble_MCP9804::_setThresholdInteger(MCP9804_Register reg, int8_t val)
{
  // For Tupper and Tcrit, alert is triggered when Ta:
  //  1. rises above threshold
  //  2. falls back below (threshold - hysteresis)
  // For Tlower, alert is triggered when Ta:
  //  1. falls below (threshold - hysteresis)
  //  2. rises back above threshold
  
  uint16_t temp = (uint8_t)val << 4;
  temp |= (val < 0 ? 0x1000 : 0x0000);
  _writeRegister16(reg, temp);
}

	
//
// Public methods
//

void Hackscribble_MCP9804::begin()
{
	Wire.begin();
	
	// Enable alert for upper and lower thresholds, active low, interrupt mode
	// uint16_t current = _readRegister16(REG_CONFIG) & 0xFFF0;
	// current |= 0x0009; 
	// _writeRegister16(REG_CONFIG, current);
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


float Hackscribble_MCP9804::getTAFloat()
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


void Hackscribble_MCP9804::setHysteresis(MCP9804_Hysteresis val)
{
  // For Tupper and Tcrit, alert is triggered when Ta:
  //  1. rises above threshold
  //  2. falls back below (threshold - hysteresis)
  // For Tlower, alert is triggered when Ta:
  //  1. falls below (threshold - hysteresis)
  //  2. rises back above threshold

  uint16_t current = _readRegister16(REG_CONFIG);
  current &= 0xF9FF;
  current |= ((uint16_t)val << 9);
  _writeRegister16(REG_CONFIG, current);
}


int8_t Hackscribble_MCP9804::getTUPPER()
{
	return _getThresholdInteger(REG_TUPPER);
}


void Hackscribble_MCP9804::setTUPPER(int8_t val)
{
	_setThresholdInteger(REG_TUPPER, val);
}


int8_t Hackscribble_MCP9804::getTLOWER()
{
	return _getThresholdInteger(REG_TLOWER);
}


void Hackscribble_MCP9804::setTLOWER(int8_t val)
{
	_setThresholdInteger(REG_TLOWER, val);
}


int8_t Hackscribble_MCP9804::getTCRIT()
{
	return _getThresholdInteger(REG_TCRIT);
}


void Hackscribble_MCP9804::setTCRIT(int8_t val)
{
	_setThresholdInteger(REG_TCRIT, val);
}


void Hackscribble_MCP9804::configureAlert()
{
	uint16_t current = _readRegister16(REG_CONFIG) & 0xFFF0;
	current |= 0x0009; // Alert enabled for upper and lower, active low, interrupt mode
	_writeRegister16(REG_CONFIG, current);
}


boolean Hackscribble_MCP9804::alertTCRIT()
{
  return (_readRegister16(REG_TA) & 0x8000) > 0;
}


boolean Hackscribble_MCP9804::alertTUPPER()
{
	return (_readRegister16(REG_TA) & 0x4000) > 0;
}


boolean Hackscribble_MCP9804::alertTLOWER()
{
	return (_readRegister16(REG_TA) & 0x2000) > 0;
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

