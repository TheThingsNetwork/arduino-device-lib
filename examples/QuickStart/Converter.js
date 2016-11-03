function Converter(decoded, port) {
  // Merge, split or otherwise
  // mutate decoded fields.
  var converted = decoded;
    
  if (port === 1 && (converted.led === 0 || converted.led === 1)) {
    converted.led = Boolean(converted.led);
  }
    
  return converted;
}
