# The Things Network Arduino Library

[![Slack Status](https://slack.thethingsnetwork.org/badge.svg)](https://slack.thethingsnetwork.org/)

## Introduction

This is an [Arduino Library](https://www.arduino.cc/en/Guide/Libraries) for Arduino-compatible devices like [The Things Uno](https://shop.thethingsnetwork.com/index.php/product/the-things-uno/) and [Node](https://shop.thethingsnetwork.com/index.php/product/the-things-node/) to communicate with [The Things Network](https://www.thethingsnetwork.org).

At the moment this library requires devices to feature a [Microchip RN2xx3 module](http://www.microchip.com/design-centers/wireless-connectivity/embedded-wireless/lora-technology).

## Installation

1. Download the ZIP file of the latest [release on GitHub](https://github.com/TheThingsNetwork/arduino-library/releases).
2. Follow [Importing a .zip Library](https://www.arduino.cc/en/Guide/Libraries#toc4) to install the library using the ZIP file.
3. Restart the Arduino IDE.

**NOTE:** Examples for the library will not be exposed in the *File > Examples > TheThingsNetwork* until after the IDE has restarted.

## Examples
The library comes with several [examples](examples) that can be loaded via *File > Eamples > TheThingsNetwork* once you've installed the library and restarted the IDE.

## API

TheThingsNetwork class features the following [public methods](https://github.com/TheThingsNetwork/arduino-library/blob/master/src/TheThingsNetwork.h#L39):

```C++
  public:
    int downlinkPort;
    byte downlink[64];
    void init(Stream& modemStream, Stream& debugStream);
    void reset(bool adr = true, int sf = DEFAULT_SF, int fsb = DEFAULT_FSB);
    bool personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]);
    bool join(const byte appEui[8], const byte appKey[16]);
    int sendBytes(const byte* buffer, int length, int port = 1, bool confirm = false);
    int sendString(String message, int port = 1, bool confirm = false);
    void showStatus();
```

## Contributing

Source code for The Things Network is MIT licensed. We encourage users to make contributions on [Github](https://github.com/TheThingsNetwork/arduino-library) and to participate in discussions on [Slack](https://slack.thethingsnetwork.org).

If you find bugs or documentation mistakes, please check [open issues](https://github.com/TheThingsNetwork/arduino-library/issues) before [creating a new issue](https://github.com/TheThingsNetwork/arduino-library/issues/new). Please be specific and give a detailed description of the issue. Explain the steps to reproduce the problem. If you're able to fix the issue yourself, please help the community by forking the repository and submitting a pull request with your fix.

For contributing a feature, please open an issue that explains what you're working on. Work in your own fork of the repository and submit a pull request when you're done.

If you want to contribute, but don't know where to start, you could have a look at issues with the label [*help wanted*](https://github.com/TheThingsNetwork/arduino-library/labels/help%20wanted) or [*difficulty/easy*](https://github.com/TheThingsNetwork/arduino-library/labels/difficulty%2Feasy).

## License

Source code for The Things Network is released under the MIT License, which can be found in the [LICENSE](LICENSE) file. A list of authors can be found in the [AUTHORS](AUTHORS) file.