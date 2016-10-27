function Encoder(object) {
  // Encode downlink messages sent as
  // object to an array or buffer of bytes.
  var bytes = [];
    
  bytes[0] = object.led ? 1 : 0;
    
  return bytes;
}
