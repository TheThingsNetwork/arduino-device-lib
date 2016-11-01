/**
 * Use as payload decoder function.
 *
 * 01 12 8E 00 21 09 5A 03 04
 * 
 * {
 *   "battery": 4750,
 *   "event": "interval",
 *   "isButtonPressed": false,
 *   "isMoving": false,
 *   "isUSBConnected": true,
 *   "light": 33,
 *   "temperature": 23.94
 * }
 *
 * 02 08 99
 * 
 * {
 *   "event": "temperature",
 *   "temperature": 22.01
 * }
 *
 * 03 00 00 01 FE
 * 
 * {
 *   "event": "motion",
 *   "duration": 0.51
 * }
 *
 * 04 00 00 01 FE
 * 
 * {
 *   "event": "button",
 *   "duration": 0.51
 * }
 */

function Decoder(bytes, port) {
  var decoded = {};

  var ports = {
    PORT_INTERVAL: 1,
    PORT_TEMPERATURE: 2,
    PORT_MOTION: 3,
    PORT_BUTTON: 4
  };

  switch (port) {
    case ports.PORT_INTERVAL:
      decoded.event = 'interval';
      decoded.battery = (bytes[0] << 8) + bytes[1];
      decoded.light = (bytes[2] << 8) + bytes[3];
      decoded.temperature = ((bytes[4] << 8) + bytes[5]) / 100;
      decoded.isMoving = (bytes[6] === 1);
      var flags = bytes[7];
      decoded.isMoving = (flags & 1) !== 0;
      decoded.isButtonPressed = (flags & 2) !== 0;
      decoded.isUSBConnected = (flags & 4) !== 0;
      break;
    case ports.PORT_TEMPERATURE:
      decoded.event = 'temperature';
      decoded.temperature = ((bytes[0] << 8) + bytes[1]) / 100;
      break;
    case ports.PORT_MOTION:
      decoded.event = 'motion';
      decoded.duration = ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3]) / 1000;
      break;
    case ports.PORT_BUTTON:
      decoded.event = 'button';
      decoded.duration = ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3]) / 1000;
      break;
  }

  return decoded;
}
