function Validator(converted, port) {
  // Return false if the decoded, converted
  // message is invalid and should be dropped.
    
  if (port === 1 && typeof converted.led !== 'boolean') {
    return false;
  }
    
  return true;
}
