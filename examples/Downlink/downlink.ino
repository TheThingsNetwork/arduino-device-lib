#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const byte appEui[8] = { <insert AppEui> }; //for example: {0x70, 0xB3, 0xD5, 0x7E, 0xE0, 0xE0, 0x01, 0x4A1};
const byte appKey[16] = { <insert AppKey> }; //for example: {0x73, 0x6D, 0x24, 0xD2, 0x69, 0xBE, 0xE3, 0xAE, 0x0E, 0xCE, 0xF0, 0xBB, 0x6C, 0xA4, 0xBA, 0xFE};

#define debugSerial Serial
#define loraSerial Serial1

#define debugPrintLn(...) { if (debugSerial) debugSerial.println(__VA_ARGS__); }
#define debugPrint(...) { if (debugSerial) debugSerial.print(__VA_ARGS__); }

TheThingsNetwork ttu;

void setup()
{
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  delay(1000);
  ttu.init(loraSerial, debugSerial); //Initializing...
  ttu.reset();
  if (!ttu.join(appEui, appKey)) {
    delay(6000);
  }

  delay(6000);
  ttu.showStatus();
  debugPrintLn("Setup for The Things Network complete");

  delay(1000);
}

void loop() {
  // Send a byte
  byte buf[1];
  buf[0] = 20;
  int downlinkBytes = ttu.sendBytes(buf, 1);

  if (downlinkBytes > 0) {
    debugPrintLn("Received " + String(downlinkBytes) + " bytes")
    // Print the received bytes
    for (int i = 0; i < downlinkBytes; i++) {
      debugPrint(String(ttu.downlink[i]) + " ");
    }
    debugPrintLn();
  }

  delay(20000);
}
