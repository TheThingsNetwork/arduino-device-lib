#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define debugSerial Serial
#define loraSerial Serial1

TheThingsNetwork ttn;

void setup() {
  debugSerial.begin(9600);
  loraSerial.begin(57600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  ttn.init(loraSerial, debugSerial);

  debugSerial.println("-- RESET");
  ttn.reset();

  // Wait 10s between each attempt to get a confirmed activation
  debugSerial.println("-- JOIN");
  while(!ttn.join(appEui, appKey)){
    delay(10000);
  }

  debugSerial.println("-- STATUS");
  ttn.showStatus();
}

void loop() {
  debugSerial.println("-- LOOP");

  // Prepare array of 1 byte to indicate LED status
  byte data[1];
  data[0] = (digitalRead(LED_BUILTIN) == HIGH) ? 1 : 0;

  // Send it off and see if we get bytes in response
  int downlinkSize = ttn.sendBytes(data, sizeof(data));
  
  // Only handle messages of a single byte
  if (downlinkSize == 1) {
  
    if (ttn.downlink[0] == 0) {
      debugSerial.println("LED: off");
      digitalWrite(LED_BUILTIN, LOW);
        
    } else if (ttn.downlink[0] == 1) {
      debugSerial.println("LED: on");
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  
  delay(10000);
}
