function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};
    
  if (port === 1) {
    decoded.led = bytes[0];
  }
    
  return decoded;
}
