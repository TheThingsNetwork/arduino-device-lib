// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSMESSAGE_H_
#define _THETHINGSMESSAGE_H_

#include "TheThingsNetwork.h"
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "deviceData.pb.h"
#include "appData.pb.h"

typedef api_DeviceData devicedata_t;
typedef api_AppData appdata_t;

class TheThingsMessage
{
public:
  static void encodeDeviceData(devicedata_t *data, uint8_t **buffer, size_t *size);
  static bool decodeAppData(appdata_t *receiveData, const uint8_t *payload, size_t size);
};

#endif
