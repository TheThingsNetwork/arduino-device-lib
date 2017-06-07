/**
 * Use as payload decoder function.
 *
 * 12 8E 00 21 09 5A
 * 
 * {
 *   "event": "interval",
 *   "device": "node",
 *   "battery": 4750,
 *   "light": 33,
 *   "temperature": 23.94
 * }
 */

function Decoder(bytes, port) {
  var decoded = {};

  var events = {
    1: 'setup',
    2: 'interval',
    3: 'motion',
    4: 'button'
  };

  if (port > 10) {
    port = port - 10;
    decoded.device = 'uno';
  } else {
    decoded.device = 'node';
    decoded.battery = (bytes[0] << 8) + bytes[1];
    decoded.light = (bytes[2] << 8) + bytes[3];
    decoded.temperature = ((bytes[4] << 8) + bytes[5]) / 100;
  }

  decoded.event = events[port];

  return decoded;
}
