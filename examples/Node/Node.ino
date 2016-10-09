#include <TheThingsNode.h>

#define debugSerial Serial

TheThingsNode node;

void setup() {
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- SETUP");

  node.configTemperatureAlert(true, 10, 15);
  
  node.onTemperatureAlert(onTemperatureAlert);
  
  node.onMotionStart(onMotionStart);
  node.onMotionStop(onMotionStop);
  
  node.onButtonPress(onButtonPress);
  node.onButtonRelease(onButtonRelease);

  node.setColor(TTN_YELLOW);

  node.showStatus();
}

void loop() {
  debugSerial.println("-- LOOP");

  node.showStatus();
  
  delay(5000);
}

void onTemperatureAlert() {
  debugSerial.println("-- TEMPERATURE ALERT");
}

void onMotionStart() {
  debugSerial.println("-- MOTION START");
}

void onMotionStop() {
  debugSerial.println("-- MOTION STOP");
}

void onButtonPress() {
  debugSerial.println("-- BUTTON PRESS");
}

void onButtonRelease() {
  debugSerial.println("-- BUTTON RELEASE");
}
