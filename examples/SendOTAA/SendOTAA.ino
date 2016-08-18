#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const byte appEui[8] = { <insert AppEui> }; //for example: {0x70, 0xB3, 0xD5, 0x7E, 0xE0, 0xE0, 0x01, 0x4A1};
const byte appKey[16] = { <insert AppKey> }; //for example: {0x73, 0x6D, 0x24, 0xD2, 0x69, 0xBE, 0xE3, 0xAE, 0x0E, 0xCE, 0xF0, 0xBB, 0x6C, 0xA4, 0xBA, 0xFE};

#define debugSerial Serial
#define loraSerial Serial1

// Set your message to send
String message = "Hello world"; //sending a string of chars "Hello world"

#define debugPrintLn(...) { if (debugSerial) debugSerial.println(__VA_ARGS__); }
#define debugPrint(...) { if (debugSerial) debugSerial.print(__VA_ARGS__); }

TheThingsNetwork ttn;

void setup() {
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  delay(1000);
  ttn.init(loraSerial, debugSerial);
  ttn.reset();

  //the device will attempt a join every second till the join is successfull
  while(!ttn.join(appEui, appKey)){
      delay(6000);
  }

  digitalWrite(13, HIGH); //turn on LED to confirm join

  delay(6000);
  ttn.showStatus();
  debugPrintLn("Setup for The Things Network complete");

  delay(1000);
}

void loop() {
  ttn.sendString(message);
  delay(20000);
}
