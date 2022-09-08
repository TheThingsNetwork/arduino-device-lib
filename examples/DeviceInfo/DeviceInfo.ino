#include <TheThingsNetwork.h>

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

  // RN2XX3/SAMR34 reset pin connected to Arduino pin 12
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  // hard reset module and wait for startup
  debugSerial.println("-- CHECK COMM");
  ttn.resetHard(12);
  delay(1000);
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
      debugSerial.println("Module unsupported!");       // module must be RN2483, RN2483A, RN2903, RN2903AS, SAMR34
    }
    while(true)                                         // stop code execution
    {
      ;
    }
  }
  // init LoRaWAN stack (only necessary in SAMR34)
  if(ttn.getModemType() == TTN_MODEM_TYPE_SAMR34)
    ttn.macReset();
}

void loop()
{
  debugSerial.println("Device Information");
  debugSerial.println();
  ttn.showStatus();
  debugSerial.println();
  debugSerial.println("Use the EUI to register the device for OTAA");
  debugSerial.println("-------------------------------------------");
  debugSerial.println();

  delay(10000);
}
