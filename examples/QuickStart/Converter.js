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
