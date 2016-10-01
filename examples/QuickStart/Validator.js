function Validator(converted) {
  // Return false if the decoded, converted
  // message is invalid and should be dropped.
    
  if (converted.led !== true && converted.led !== false) {
    return false;
  }
    
  return true;
}