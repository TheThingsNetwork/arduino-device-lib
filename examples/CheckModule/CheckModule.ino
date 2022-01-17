#include <TheThingsNetwork.h>

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  // RN2XX3 reset pin connected to Arduino pin 12
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  // hard reset module and wait for startup
  debugSerial.println("-- CHECK COMM");
  ttn.resetHard(12);
  delay(100);
  // check if a valid module responded
  // (if no module is connected or wiring is bad, checkValidModuleConnected() will
  //  take about ~30s to return (another ~30s if autobaud_first is true))
  if(!ttn.checkValidModuleConnected(true))
  {
    if(ttn.needsHardReset)
    {
      debugSerial.println("Module unresponsive, please power cycle or hard reset board!");
    }
    else
    {
      debugSerial.println("Module unsupported!");       // module must be RN2483, RN2483A, RN2903, RN2903AS
    }
    while(true)                                         // stop code execution
    {
      ;
    }
  }

  // do an ABP join
  debugSerial.println("-- PERSONALIZE");
  // false is added as fourth argument to the personalize() call so that it
  // does not perform a soft reset, because the module was already hard reset before via pin 12.
  ttn.personalize(devAddr, nwkSKey, appSKey, false);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Prepare payload of 1 byte to indicate LED status
  byte payload[1];
  payload[0] = (digitalRead(LED_BUILTIN) == HIGH) ? 1 : 0;

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}
