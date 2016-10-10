#include <TheThingsNetwork.h>
#include <TheThingsNode.h>

const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int loopDelay = 60000;

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, TTN_FP_EU868);
TheThingsNode node;

enum REASON : byte
{
  TEMPERATURE,
  MOTION,
  BUTTON,
  LOOP,
  REQUEST
};

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  ttn.onMessage(onMessage);

  debugSerial.println("-- TTN: STATUS");
  ttn.showStatus();

  debugSerial.println("-- TTN: JOIN");
  ttn.join(appEui, appKey);

  node.configTemperatureAlert(true, 10, 15);

  node.onTemperatureAlert(onTemperatureAlert);

  node.onMotionStart(onMotionStart);
  node.onMotionStop(onMotionStop);

  node.onButtonPress(onButtonPress);
  node.onButtonRelease(onButtonRelease);

  node.setColor(TTN_YELLOW);

  debugSerial.println("-- NODE: STATUS");
  node.showStatus();
}

void loop() {
  debugSerial.println("-- LOOP");

  debugSerial.println("-- NODE: STATUS");
  node.showStatus();

  sendMessage(LOOP);

  delay(loopDelay);
}

void onTemperatureAlert() {
  debugSerial.println("-- TEMPERATURE ALERT");

  sendMessage(TEMPERATURE);
}

void onMotionStart() {
  debugSerial.println("-- MOTION START");
}

void onMotionStop() {
  debugSerial.println("-- MOTION STOP");

  sendMessage(MOTION);
}

void onButtonPress() {
  debugSerial.println("-- BUTTON PRESS");
}

void onButtonRelease() {
  debugSerial.println("-- BUTTON RELEASE");

  sendMessage(BUTTON);
}

void onMessage(const byte* payload, int length, int port) {
  debugSerial.println("-- ON MESSAGE");

  switch (port) {
    case 1:
      debugSerial.println("Message requested");
      sendMessage(REQUEST);
      break;
    case 2:
      loopDelay = ((payload[1] << 8) + payload[2]) * 1000;
      debugSerial.println("Loop delay changed: " + String(loopDelay));
      loop();
      break;
    case 3:
      bool enabled = (payload[0] == 1);
      int lower = (payload[1] << 8) + payload[2];
      int upper = (payload[3] << 8) + payload[4];
      int critical = (payload[5] << 8) + payload[6];
      node.configTemperatureAlert(enabled, lower, upper, critical);
      debugSerial.print("Temperature alert changed, on: ");
      debugSerial.print(enabled ? "yes" : "no");
      debugSerial.println(", lower: " + String(lower) + ", upper: " + String(upper) + ", critical: " + String(critical));
      break;
  }
}

void sendMessage(REASON reason) {
  debugSerial.println("-- SEND MESSAGE");

  byte payload[9];

  payload[0] = reason;

  byte temperature = node.getTemperatureAsFloat();
  byte* b = (byte*) &temperature;
  payload[1] = b[0];
  payload[2] = b[1];
  payload[3] = b[2];
  payload[4] = b[3];

  uint16_t light = node.getLight();
  payload[5] = (light & 0xFF00) >> 8;
  payload[6] = (light & 0x00FF);

  uint16_t battery = node.getBattery();
  payload[7] = (battery & 0xFF00) >> 8;
  payload[8] = (battery & 0x00FF);

  ttn.sendBytes(payload, sizeof(payload));
}
