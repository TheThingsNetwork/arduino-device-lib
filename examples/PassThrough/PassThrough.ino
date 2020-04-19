/**
 * Use this sketch to talk directly to the LoRa module.
 * 
 * In Serial Monitor, select "Both NL & CR" and "115200 baud" in
 * the bottom right dropdowns and send command a command like:
 *
 * mac get deveui
 * sys reset
 */

#define loraSerial Serial1
#define debugSerial Serial

void setup()
{
  while (!debugSerial || !loraSerial)
    ;

  debugSerial.begin(115200);
  delay(1000);

  loraSerial.begin(57600);
}

void loop()
{
  while (debugSerial.available())
  {
    loraSerial.write(debugSerial.read());
  }
  while (loraSerial.available())
  {
    debugSerial.write(loraSerial.read());
  }
}
