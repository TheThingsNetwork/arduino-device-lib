#ifndef _THETHINGSMESSAGE_H_
#define _THETHINGSMESSAGE_H_

#include <TheThingsNetwork.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "sensorData.pb.h"

typedef api_SensorData sensordata_t;

class TheThingsMessage
{
  public:
    static void encodeSensorData(sensordata_t *data, byte **buffer, size_t *size);
};

#endif
