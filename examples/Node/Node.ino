#include <TheThingsNode.h>

#define debugSerial Serial

TheThingsNode node;

void setup() {
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- SETUP");
  
  node.onButtonPress(onPress);
  node.onButtonRelease(onRelease);

  node.showStatus();
}

void loop() {
  debugSerial.println("-- LOOP");

  node.showStatus();
  
  delay(5000);
}

void onPress() {
  debugSerial.println("-- PRESS");
}

void onRelease() {
  debugSerial.println("-- RELEASE");
}