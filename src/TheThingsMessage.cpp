#include <TheThingsMessage.h>

bool TheThingsMessage::decodeAppData(appdata_t *receiveData, const byte *payload, size_t length) {
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
  if (!pb_decode(&stream, api_AppData_fields, receiveData)) {
    return false;
  }
  receiveData->light ? digitalWrite(TTN_PIN_LED, HIGH) : digitalWrite(TTN_PIN_LED, LOW);
  // if (receiveData->flush) {
  //   flushWater();
  // }
  return true;
}

void TheThingsMessage::encodeSensorData(sensordata_t *data, byte **buffer, size_t *size) {
  byte message[TTN_BUFFER_SIZE];
  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, api_SensorData_fields, data);
  *(buffer) = message;
  *(size) = sendStream.bytes_written;
}

unsigned long  TheThingsMessage::getWaterLevel(int signal) {
  unsigned long echo;

  pinMode(signal, OUTPUT);
  digitalWrite(signal, LOW);
  delayMicroseconds(2);
  digitalWrite(signal, HIGH);
  delayMicroseconds(5);
  digitalWrite(signal, LOW);
  pinMode(signal, INPUT);
  digitalWrite(signal, LOW);
  echo = pulseIn(signal, HIGH);
  echo = (echo / 58.138);
  return echo;
}

void TheThingsMessage::flushWater() {
  return ;
}
