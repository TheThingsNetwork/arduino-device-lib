#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  // For SAMR34, configure baudrate to the speed set in RN parser firmware (default is 115200)
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  pinMode(12, OUTPUT);                              // RN2XX3/SAMR34 reset pin connected to Arduino pin 12
  digitalWrite(12, HIGH);
  debugSerial.println("-- CHECK COMM");
  ttn.resetHard(12);                                // hard-reset the module
  delay(1000);                                      // wait for module startup
  ttn.checkValidModuleConnected(true);              // check a valid module is connected (RN2xx3 or SAMR34)
  if(ttn.getModemType() == TTN_MODEM_TYPE_SAMR34)   // if SAMR34, init LoRaWAN stack
    ttn.macReset();

  // Set callback for incoming messages
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Send single byte to poll for incoming messages
  ttn.poll();

  delay(10000);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < size; i++)
  {
    debugSerial.print(" " + String(payload[i]));
  }

  debugSerial.println();
}
