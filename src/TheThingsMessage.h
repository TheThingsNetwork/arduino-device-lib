#ifndef _THETHINGSMESSAGE_H_
#define _THETHINGSMESSAGE_H_

#include <TheThingsNetwork.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "sensorData.pb.h"
#include "appData.pb.h"

#define TTN_PIN_LED A2

typedef api_SensorData sensordata_t;
typedef api_AppData appdata_t;

class TheThingsMessage
{
  public:
    static void encodeSensorData(sensordata_t *data, byte **buffer, size_t *size);
    static bool decodeAppData(appdata_t *receiveData, const byte *payload, size_t length);
};

#endif
