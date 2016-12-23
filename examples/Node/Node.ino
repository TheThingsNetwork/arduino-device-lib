#include <TheThingsNetwork.h>
#include <TheThingsNode.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
TheThingsNode *node;

enum port : byte
{
  PORT_SETUP = 1,
  PORT_INTERVAL,
  PORT_MOTION,
  PORT_BUTTON
};

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- TTN: STATUS");
  ttn.showStatus();

  debugSerial.println("-- TTN: JOIN");
  ttn.join(appEui, appKey);

  node = TheThingsNode::setup();
  node->setColor(TTN_GREEN);

  node->configLight(true);
  node->configInterval(true, 60000);

  node->onWake(wake);
  node->onInterval(interval);
  node->onSleep(sleep);
  
  node->onMotionStart(onMotionStart);
  node->onButtonRelease(onButtonRelease);

  debugSerial.println("-- NODE: STATUS");
  node->showStatus();

  sendData(PORT_SETUP);
}

void loop() {
  node->loop();
}

void interval() {
  node->setColor(TTN_BLUE);
  
  debugSerial.println("-- INTERVAL");
  node->showStatus();

  sendData(PORT_INTERVAL);
}

void wake() {
  node->setColor(TTN_GREEN);
}

void sleep() {
  node->setColor(TTN_BLACK);
}

void onMotionStart() {
  node->setColor(TTN_RED);
  debugSerial.print("-- MOTION STOP");

  sendData(PORT_MOTION);
}

void onButtonRelease(unsigned long duration) {
  node->setColor(TTN_RED);
  debugSerial.print("-- BUTTON RELEASE: ");
  debugSerial.println(duration);

  sendData(PORT_BUTTON);
}

void sendData(port) {
  byte* bytes;
  byte payload[9];

  uint16_t battery = node->getBattery();
  bytes = (byte*) &battery;
  payload[0] = bytes[1];
  payload[1] = bytes[0];

  uint16_t light = node->getLight();
  bytes = (byte*) &light;
  payload[2] = bytes[1];
  payload[3] = bytes[0];

  int16_t temperature = round(node->getTemperatureAsFloat() * 100);
  bytes = (byte*) &temperature;
  payload[4] = bytes[1];
  payload[5] = bytes[0];

  ttn.sendBytes(payload, sizeof(payload), PORT_INTERVAL);
}
