function Decoder(bytes) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};
    
  decoded.led = bytes[0];
    
  return decoded;
}