#include <TheThingsNetwork.h>
#include <CayenneLPP.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop()
{
  debugSerial.println("-- LOOP");

  lpp.reset();
  lpp.addTemperature(1, 22.5);
  lpp.addBarometricPressure(2, 1073.21);
  lpp.addGPS(3, 52.37365, 4.88650, 2);

  // Send it off
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());

  delay(10000);
}
