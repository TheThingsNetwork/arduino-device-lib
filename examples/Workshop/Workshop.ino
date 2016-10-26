#include <TheThingsNetwork.h>

// Before your start, make sure that in the Tools menu, your Board and your
// Port is set to Arduino Leonardo

// After you registered your ABP device, go to The Things Network Dashboard
// and copy the Device Addr, Network Session Key and App Session Key

// Set your Device Address, for example: "1C55B133";
const String devAddr = " ... ";

// Set your Network Session Key, for example: "0C11F84C5E4315CCDE50AB31C430BBE5";
// This is used by the network to identify your device
const String nwkSKey = " ... ";

// Set your Application Session Key, for example: "6C250A6B559B1BA0A51696ED20E3CF56";
// This is used by the network for encryption
const String appSKey = " ... ";

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, /* TTN_FP_EU868 or TTN_FP_US915 */);

void setup() {
  // Set up the serial interfaces for the debugging serial monitor and LoRa module
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  // Here we activate the device with your address and keys
  ttn.personalize(devAddr, nwkSKey, appSKey);

  // Show the status on the debugging serial monitor
  ttn.showStatus();
}

void loop() {
  // Create a buffer with three bytes
  byte payload[3] = { 0x01, 0x02, 0x03 };

  // Send it to the network
  ttn.sendBytes(payload, sizeof(payload));

  // Wait 10 seconds
  delay(10000);
}
