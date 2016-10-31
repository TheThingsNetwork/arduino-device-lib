# API Reference

## Class: TheThingsNode
Include TheThingsNode class and create a pointer to the singleton instance like this:

```c
#include <TheThingsNode.h>

TheThingsNode *node;
```

## Method: setup
Use the static `setup()` method in your sketch's `setup()` function to create or get the class singleton.

```c
void setup() {
    node = TheThingsNode::setup();
}
```

## Method: loop
In your `loop()` function, call the singleton's `loop()` method.

```c
void loop() {
    node->loop();
}
```

This will:

1. Restore USB communication, if the Node has a USB data connection.
2. Call the function set via [`onWake()`](#method-onwake).
3. Find out why the device woke up and call the function set via the related `on*()` method.
4. Call the function set via [`onSleep()`](#method-onsleep).
5. Sleep the Node, unless the Node has a USB data connection and hasn't been configured to also sleep in that case.

> Don't add any other code in your `loop()` function, but use [`onWake()`](#method-onwake), [`onInterval()`](#method-oninterval) and [`onSleep()`](#method-onsleep) instead, to prevent unpredictable behavior.

## Method: onWake
Set a callback that will run first thing every time the Node wakes up, which is when an interrupt changes because of interaction with the button, motion sensor or temperature sensor. The device also wakes up every 8 seconds, which is the longest we can make it sleep.

```c
void onWake(void(*callback)(void));
```

Usage:

```c
void setup() {
    node = TheThingsNode::setup();
    
    node->onWake(wake);
}

void wake() {
    node->setColor(TTN_GREEN);
}
```

## Method: onSleep
Set a callback that will run last thing before the Node goes back to sleep for 8 seconds or until an interrupt changes because of interaction with the button, motion sensor or temperature sensor.

```c
void onSleep(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

Usage:

```c
void setup() {
    node = TheThingsNode::setup();
    
    node->onSleep(sleep);
}

void sleep() {
    node->setColor(TTN_BLACK);
}
```

## Interval
Instead of using your `loop()` function, use `configInterval()` and `onInterval()` to set a function to be called on a certain interval:

```c
void setup() {
    node = TheThingsNode::setup();
    
    // Call every 30s instead of the default 60
    node->configInterval(true, 30000);
    
    node->onInterval(interval);
}

void interval() {
    node->showStatus();
}
```

### Method: onInterval
Set a callback that will run on a certain interval. This will automatically enable the interval.

```c
void onInterval(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: configInterval

```c
void configInterval(bool enabled, uint32_t ms);
void configInterval(bool enabled);
```

- `bool enabled`: Enable or disable the interval callback. Enabled automatically by `onInterval()`, but you can use this method to temporarily disable it. Defaults to `false`.
- `uint32_t ms`: Minimal time between calling the interval callback. Defaults to `60000` or 60 seconds.

> If the Node has no USB data connection or is configured to also sleep in that case, it will only wake up every 8 seconds to check if the interval callback should run. This means setting `ms` to less than `8000` makes no sense. It also means that the maximum time between calls can be up to 8 seconds longer than `ms` if it wakes up to handle button or sensor interaction in between.

## Method: showStatus
Writes information about the device and sensors to `Serial`.

```c
void showStatus();
```

Will write something like:

```bash
Light: 25
Temperature: 23.16 C
Temperature alert: No
Moving: No
Button pressed: No
Color: Yellow
USB connected: Yes
Battery voltage: 4704 MV
```

## Light
Configure and read the light sensor.

### Method: configLight
Configure the light sensor.

```c
void configLight(bool enabled, uint8_t gain);
void configLight(bool enabled);
```

- `bool enabled`: Enable or disable the light sensor. Defaults to `false`.
- `uint8_t gain`: **TODO**. Value between `0` and `3`. Defaults to `1`.

### Method: getLight
Get the current value of the light sensor as an unsigned integer of 2 bytes.

```c
uint16_t getLight();
```

> Make sure you have enabled the light sensor or the value will be `0`.

## Temperature
Use and configure the temperature sensor.

### Method: getTemperatureAsInt
Get the current value of the temperature sensor in Celsius, rounded to a signed integer of 1 byte.

```c
int8_t getTemperatureAsInt();
```

> You can call this method even if the temperature alert has been disabled. The sensor will be woken up shortly to get the current temperature.

### Method: getTemperatureAsFloat
Get the current value of the temperature sensor in Celsius as signed float of 4 bytes.

```c
float getTemperatureAsFloat();
```

> You can call this method even if the temperature alert has been disabled. The sensor will be woken up shortly to get the current temperature.

### Method: configTemperature
Configure the temperature sensor.

```c
void configTemperature(bool enabled, MCP9804_Resolution resolution, int8_t lower, int8_t upper, int8_t critical, MCP9804_Hysteresis hysteresis);
void configTemperature(bool enabled, MCP9804_Resolution resolution);
void configTemperature(bool enabled);
```

- `bool enabled `: Enable or disable temperature alerts. Enabled automatically by `onTemperature()`, but you can use this method to temporarily disable the alerts. Defaults to `false`.
- `MCP9804_Resolution resolution = R_DEGREES_0_0625 `: Set the resolution (precision) of the sensor. One of:
    - `R_DEGREES_0_5000`: +0.5 C
    - `R_DEGREES_0_2500`: +0.25 C
    - `R_DEGREES_0_1250`: +0.125 C
    - `R_DEGREES_0_0625`: +0.0625 (default)

  See the sensor's [data sheet, section 5.2.4](http://ww1.microchip.com/downloads/en/DeviceDoc/22203C.pdf) for more details.
    
- `int8_t lower`: Alert when temperature in Celsius is lower. Defaults to `0`.
- `int8_t upper`: Alert when temperature in Celsius is higher. Defaults to `30`.
- `int8_t critical = 55`: Alert when temperature in Celsius is higher. Defaults to `55`, above which you run the risk of battery failure.
- `MCP9804_Hysteresis hysteresis = H_DEGREES_0_0`: Set the margin for the bounds, before an alert is triggered. One of:
    - `H_DEGREES_0_0`: 0 C (default)
    - `H_DEGREES_1_5`: +1.5 C
    - `H_DEGREES_3_0`: +3 C
    - `H_DEGREES_6_0`: +6 C

  See the sensor's [data sheet, section 5.2.2](http://ww1.microchip.com/downloads/en/DeviceDoc/22203C.pdf) for more details.

### Method: onTemperatureAlert
Register a function to be called when temperature exceeds the configured bounds. This will automatically enable the temperature alerts.

```c
void onTemperature(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: hasTemperatureAlert
Returns `true` if the current temperature exceeds the configured bounds for the alert. This also works if the alert itself is temporarily disabled.

```c
bool hasTemperatureAlert();
```

## Motion
Use and configure the motion sensor.

### Method: onMotionStart
Set a function to be called when motion starts. This will automatically enable the motion sensor.

```c
void onMotionStart(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: onMotionStop
Set a function to be called when motions stops. This will automatically enable the motion sensor.

```c
void onMotionStop(void(*callback)(unsigned long duration));
```

- `void(*callback)`: Function to be called.
    - `unsigned long duration`: Time since the motion began, in ms.

### Method: isMoving
Returns `true` if the device is currently moving. Requires the sensor to be enabled via `onMotionStart()`, `onMotionStop()` or `setMotion()`.

### Method: configMotion
Enable or disable the motion sensor. The sensor will be enabled automatically by `onMotionStart()` and `onMotionStop()`, but you can use this method to temporarily disable the sensor.

```c
void configMotion(bool enabled);
```

- `bool enabled`: Enable or disable the sensor.

## Button
Respond to interaction with the button.

### Method: onButtonPress
Set a function to be called when the button is pressed down.

```c
void onButtonPress(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: onButtonRelease
Set a function to be called when the button is released.

```c
void onButtonRelease(void(*callback)(unsigned long duration));
```

- `void(*callback)`: Function to be called.
    - `unsigned long duration`: Time since the button was pressed, in ms.

### Method: isButtonPressed
Returns `true` if the button is currently pressed.

```c
bool isButtonPressed();
```

### Method: configButton
Enable or disable the button callback. Will be enabled automatically by `onButtonPress()` and `onButtonRelease()`, but you can use this method to temporarily disable it.

```c
void configButton(bool enabled);
```

- `bool enabled`: Enable or disable the button callback.

## LED
Configure the LED.

### Method: setColor
Combine one or more LEDs to get a primary or [additive color](https://en.wikipedia.org/wiki/RGB_color_model#Additive_primary_colors).

```c
setColor(ttn_color color);
```

- `ttn_color color`: Set to one of:
  - `TTN_RED`
  - `TTN_GREEN`
  - `TTN_BLUE`
  - `TTN_YELLOW`: Red and green
  - `TTN_CYAN`: Green and blue
  - `TTN_MAGENTA`: Red and blue
  - `TTN_WHITE`: Red, green and blue
  - `TTN_BLACK`: None

### Method: setRGB
Turn one or more LEDs on or off.

```c
void setRGB(bool red = false, bool green = false, bool blue = false);
```

- `bool red = false`: Set to `true` to turn the red LED on. Defaults to `false`.
- `bool green = false`: Set to `true` to turn the green LED on. Defaults to `false`.
- `bool blue = false`: Set to `true` to turn the blue LED on. Defaults to `false`.

> Combine colors to [create additives](https://en.wikipedia.org/wiki/RGB_color_model#Additive_primary_colors), e.g. red plus green makes yellow.

### Method: setRed
Turn the red LED on or off.

```c
void setRed(bool on = true);
```

- `bool on = true`: Set to `true` to turn the red LED on. Defaults to `true`.

### Method: setGreen
Turn the red LED on or off.

```c
void setGreen(bool on = true);
```

- `bool on = true`: Set to `true` to turn the green LED on. Defaults to `true`.

### Method: setBlue
Turn the red LED on or off.

```c
void setBlue(bool on = true);
```

- `bool on = true`: Set to `true` to turn the blue LED on. Defaults to `true`.

### Method: getColor
Returns the current color of the LED.

```c
ttn_color getColor();
```

Returns one of:

- `TTN_RED`
- `TTN_GREEN`
- `TTN_BLUE`
- `TTN_YELLOW`: Red and green
- `TTN_CYAN`: Green and blue
- `TTN_MAGENTA`: Red and blue
- `TTN_WHITE`: Red, green and blue
- `TTN_BLACK`: None

### Method: colorToString
Returns a string for the color passed to it.

```c
String colorToString(ttn_color color);
```

- `ttn_color color`: Color to return a string for.

Returns one of:

- `"Red"`: `TTN_RED`
- `"Green"`: `TTN_GREEN`
- `"Blue"`: `TTN_BLUE`
- `"Yellow"`: `TTN_YELLOW`
- `"Cyan"`: `TTN_CYAN`
- `"Magenta"`: `TTN_MAGENTA`
- `"White"`: `TTN_WHITE`
- `"Black"`: `TTN_BLACK`

### Method: getRed
Returns `true` if the red LED is currently on.

```c
bool getRed();
```

### Method: getGreen
Returns `true` if the green LED is currently on.

```c
bool getGreen();
```

### Method: getBlue
Returns `true if the blue LED is currently on.

```c
bool getBlue();
```

## USB
Monitor the USB data connection.

### Method: isUSBConnected
Returns `true` if the Node has a data connection over USB.

```c
bool isUSBConnected();
```

### Method:: configUSB
Enable or disable deep sleep when a USB data connection is active.

```c
void configUSB(bool deepSleep);
```

- `bool deepSleep`: Set to `true` to sleep the Node even if a USB data connection is active. Defaults to `false`.

When disabled, the [`loop()`](#method-loop) method will delay 100ms until an interrupt has changed or configured interval has been reached. When enabled, it will sleep until woken up by an interrupt change or every 8s.

> When the Node goes into sleep, the Serial Monitor will loose its connection. The Node will try to reopen the connection when it wakes up, but Serial Monitor might not always be able to pick it up.

## Method: getBattery
Returns the battery level in micro voltage as a unsigned integer of 2 bytes.

```c
uint16_t getBattery();
```
