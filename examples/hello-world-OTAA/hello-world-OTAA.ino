#include <TheThingsNetwork.h>

// Set your app Credentials
const byte appEui[8] = { <insert appEui> }; //for example: {0x70, 0xB3, 0xD5, 0x7E, 0xE0, 0xE0, 0x01, 0x4A1}
const byte appKey[16] = { <insert aapKey> }; //for example: {0x73, 0x6D, 0x24, 0xD2, 0x69, 0xBE, 0xE3, 0xAE, 0x0E, 0xCE, 0xF0, 0xBB, 0x6C, 0xA4, 0xBA, 0xFE}

#define debugSerial Serial
#define loraSerial Serial1

TheThingsNetwork ttn;

void setup()
{
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  delay(1000);
  ttn.init(loraSerial, debugSerial); //Initializing...
  ttn.reset();
  ttn.join(appEui, appKey);

  delay(6000);
  ttn.showStatus();
  debugSerial.println("Setup for The Things Network complete");

  delay(1000);
}

void loop() {
  ttn.sendString("Hello world!");

  delay(20000);
}
