#include <TheThingsNetwork.h>

// Before your start, make sure that in the Tools menu, your Board and your
// Port is set to Arduino Leonardo

// After you registered your ABP device, go to The Things Network Dashboard
// and copy the Device Addr, Network Session Key and App Session Key

// Set your Device Address, for example: { 0x02, 0xDE, 0xAE, 0x00 };
const byte devAddr[4] = { ... };

// Set your Network Session Key, for example: { 0x2B, 0x7E, 0x15, 0x16, ... };
// This is used by the network to identify your device
const byte nwkSKey[16] = { ... };

// Set your Application Session Key, for example: { 0x2B, 0x7E, 0x15, 0x16, ... };
// This is used by the network for encryption
const byte appSKey[16] = { ... };

#define debugSerial Serial
#define loraSerial Serial1

#define debugPrintLn(...) { if (debugSerial) debugSerial.println(__VA_ARGS__); }
#define debugPrint(...) { if (debugSerial) debugSerial.print(__VA_ARGS__); }

TheThingsNetwork ttn;

void setup() {
  // Set up the serial interfaces for the debugging serial monitor and LoRa module
  debugSerial.begin(115200);
  loraSerial.begin(57600);
  delay(1000);

  // Initialize and reset The Things Uno
  ttn.init(loraSerial, debugSerial);
  ttn.reset();

  // Here we activate the device with your address and keys
  ttn.personalize(devAddr, nwkSKey, appSKey);

  // Show the status on the debugging serial monitor
  ttn.showStatus();
  debugPrintLn("Setup for The Things Network complete");
}

void loop() {
  // Create a buffer with three bytes  
  byte data[3] = { 0x01, 0x02, 0x03 };

  // Send it to the network
  ttn.sendBytes(data, sizeof(data));

  // Wait 10 seconds
  delay(10000);
}
