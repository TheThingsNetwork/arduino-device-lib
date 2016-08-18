#include <TheThingsNetwork.h>

#define debugSerial Serial
#define loraSerial Serial1

TheThingsNetwork ttu;

void setup()
{
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  delay(3000);

  ttu.init(loraSerial, debugSerial);
}

void loop()
{
  debugSerial.println("Device Information");
  debugSerial.println();
  ttu.showStatus();
  debugSerial.println();
  debugSerial.println("Use the EUI to register the device for OTAA");
  debugSerial.println("-------------------------------------------");
  debugSerial.println();
  delay(10000);
}
