function Encoder(object, port) {
  // Encode downlink messages sent as
  // object to an array or buffer of bytes.
  var bytes = [];
    
  if (port === 1) {
    bytes[0] = object.led ? 1 : 0;
  }
    
  return bytes;
}
