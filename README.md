# The Things Network Arduino Library
[![Build Status](https://travis-ci.org/TheThingsNetwork/arduino-device-lib.svg?branch=master)](https://travis-ci.org/TheThingsNetwork/arduino-device-lib)

This is an [Arduino Library](https://www.arduino.cc/en/Guide/Libraries) for Arduino devices like [The Things Uno](https://www.thethingsnetwork.org/docs/devices/uno/) and [Node](https://www.thethingsnetwork.org/docs/devices/node/) to communicate via [The Things Network](https://www.thethingsnetwork.org).

> At the moment this library requires devices to feature a [Microchip RN2xx3 module](http://www.microchip.com/design-centers/wireless-connectivity/embedded-wireless/lora-technology). You may also use a `SAMR34`-based board, for more information on that see [SAM34 Usage](#user-content-samr34-usage).

## Installation

* Install the library by [Using the Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3)
* **OR** by [Importing the .zip library](https://www.arduino.cc/en/Guide/Libraries#toc4) using either the [master](https://github.com/TheThingsNetwork/arduino-device-lib/archive/master.zip) or one of the [releases](https://github.com/TheThingsNetwork/arduino-device-lib/releases) ZIP files.

## Documentation

* [The Things Network Documentation](https://www.thethingsnetwork.org/docs/devices/arduino/)
* API References:
    * [TheThingsNetwork](docs/TheThingsNetwork.md)
    * [TheThingsMessage](docs/TheThingsMessage.md)

## SAM34 Usage

Compatibility between this library and the `SAMR34`-based boards is, at the moment, experimental. Boards that can be used are the [SAMR34 Xplained Pro](https://www.microchip.com/en-us/development-tool/dm320111), the [WLR089 Xplained Pro](https://www.microchip.com/en-us/development-tool/EV23M25A) or a proprietary board containing the `SAMR34`.

Before usage, please note the following:

1. If using a `SAMR34`-based board, you must first program your board with the [RN Parser](https://github.com/MicrochipTech/atsamr34_lorawan_rn_parser) firmware. This firmware emulates the behaviour of the `RN2xx3` devices on the `SAMR34`. Specifically, you **must** use the `MLS 1_0_P_6 (Parser_ECC608)` firmware contained [here](https://github.com/MicrochipTech/atsamr34_lorawan_rn_parser/tree/master/software/MLS_1_0_P_6/Parser_ECC608), since several bugs that made the firmware unusable where fixed for this release.
2. Some commands available in the `RN2xx3` modems are not implemented in the RN Parser firmware for `SAMR34`. For a complete list of the implemented commands and their possible differences, see the [Command User Guide](https://github.com/MicrochipTech/atsamr34_lorawan_rn_parser/blob/master/02_command_guide/README.md#top) for the RN Parser firmware.
3. The `autoBaud` feature is not available in the RN parser firmware, but this is easily circumvented by manually modifying the default baud rate in the `conf_sio2host.h` file within the firmware's source code. Default baud rate is `115200`.

## Examples

The library comes with [examples](examples). After installing the library you need to restart the Arduino IDE before they can be found under **File > Examples > TheThingsNetwork**.
