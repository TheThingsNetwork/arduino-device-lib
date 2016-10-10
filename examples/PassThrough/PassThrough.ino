/**
 * Use this sketch to talk directly to the LoRa module.
 * 
 * In Serial Monitor, select "Both NL & CR" and "9600 baud" in
 * the bottom right dropdowns and send command a command like:
 *
 * mac get deveui
 * sys reset
 */

#define loraSerial Serial1
#define debugSerial Serial

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);
}

void loop() {
  if (debugSerial.available()) {
    loraSerial.write(debugSerial.read());
  }
  if (loraSerial.available()) {
    debugSerial.write(loraSerial.read());
  }
}