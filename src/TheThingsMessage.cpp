#include <TheThingsMessage.h>

bool TheThingsMessage::decodeAppData(appdata_t *receiveData, const byte *payload, size_t length) {
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
  if (!pb_decode(&stream, api_AppData_fields, receiveData)) {
    return false;
  }
  receiveData->light ? digitalWrite(TTN_PIN_LED, HIGH) : digitalWrite(TTN_PIN_LED, LOW);
  return true;
}

void TheThingsMessage::encodeSensorData(sensordata_t *data, byte **buffer, size_t *size) {
  byte message[TTN_BUFFER_SIZE];

  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, api_SensorData_fields, data);
  *buffer = message;
  *size = sendStream.bytes_written;
}
