/**
 * These are the payload functions to use with the sketch.
 * Configure them via The Things Network Dashboard or CLI.
 */

function Decoder(bytes) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};
    
  decoded.led = bytes[0];
    
  return decoded;
}

function Converter(decoded) {
  // Merge, split or otherwise
  // mutate decoded fields.
  var converted = decoded;
    
  if (converted.led === 0) {
    converted.led = false;
  }
    
  if (converted.led === 1) {
    converted.led = true;
  }
    
  return converted;
}

function Validator(converted) {
  // Return false if the decoded, converted
  // message is invalid and should be dropped.
    
  if (converted.led !== true && converted.led !== false) {
    return false;
  }
    
  return true;
}

function Encoder(object) {
  // Encode downlink messages sent as
  // object to an array or buffer of bytes.
  var bytes = [];
    
  bytes[0] = object.led ? 1 : 0;
    
  return bytes;
}