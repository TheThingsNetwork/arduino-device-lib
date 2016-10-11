# API Reference

## Class: TheThingsNode
Include and instantiate the TheThingsNode class to read sensor values from The Things Node.

```c
#include <TheThingsNode.h>

TheThingsNode node;
```

## Method: showStatus
Writes information about the device and sensors to `Serial`.

```c
void showStatus();
```

Will write something like:

```bash
Light: 25
Temperature as int: 23 C
Temperature as float: 23.00 C
Temperature alert: No
Moving: No
Button pressed: No
Color: Yellow
USB connected: Yes
Battery voltage: 4704 MV
```

## Light
Use and control the light sensor.

### Method: configLight
Configure the light sensor.

```c
void configLight(int gain);
```

- `int gain`: **TODO**

### Method: getLight
Get the current value of the light sensor as an unsigned integer of 2 bytes.

```c
uint16_t getLight();
```

## Temperature
Use and control the temperature sensor.

### Method: configTemperature
Configure the temperature sensor.

```c
void configTemperature(MCP9804_Resolution resolution);
```

- `MCP9804_Resolution resolution = R_DEGREES_0_0625 `: Set the resolution (precision) of the sensor. One of:
    - `R_DEGREES_0_5000`
    - `R_DEGREES_0_2500`
    - `R_DEGREES_0_1250`
    - `R_DEGREES_0_0625` (default)

  See the sensor's [data sheet, section 5.2.4](http://ww1.microchip.com/downloads/en/DeviceDoc/22203C.pdf) for more details.

### Method: getTemperatureAsInt
Get the current value of the temperature sensor in Celsius, rounded to a signed integer of 1 byte.

```c
int8_t getTemperatureAsInt();
```

### Method: getTemperatureAsFloat
Get the current value of the temperature sensor in Celsius as signed float of 4 bytes.

```c
float getTemperatureAsFloat();
```

### Method: configureTemperatureAlert
Configure the temperature alert.

```c
void configTemperatureAlert(bool enabled);

void configTemperatureAlert(bool enabled, int8_t lower, int8_t upper, int8_t critical, MCP9804_Hysteresis hysteresis = H_DEGREES_0_0);
```
    
- `bool enabled `: Enable or disable temperature alerts. Enabled automatically by `onTemperature()`, but you can use this method to temporarily disable the alerts.
- `int8_t lower`: Alert when temperature in Celsius is lower.
- `int8_t upper`: Alert when temperature in Celsius is higher.
- `int8_t critical = 55`: Alert when temperature in Celsius is higher. Defaults to `55`, above which you run the risk of battery failure.
- `MCP9804_Hysteresis hysteresis = H_DEGREES_0_0`: Set the margin for the bounds, before an alert is triggered. One of:
    - `H_DEGREES_0_0` (default)
    - `H_DEGREES_1_5`
    - `H_DEGREES_3_0`
    - `H_DEGREES_6_0`

  See the sensor's [data sheet, section 5.2.2](http://ww1.microchip.com/downloads/en/DeviceDoc/22203C.pdf) for more details.

### Method: onTemperatureAlert
Register a function to be called when temperature exceeds the configured bounds. This will automatically enable the temperature alerts.

```c
void onTemperature(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: hasTemperatureAlert
Returns `true` if the current temperature exceeds the configured bounds for the alert. This also works if the alert itself is temporarily disabled.

## Motion
Use and control the motion sensor.

### Method: configMotion
Enable or disable the motion sensor. The sensor will be enabled automatically by `onMotionStart()` and `onMotionStop()`, but you can use this method to temporarily disable the sensor.

```c
void configMotion(bool enabled);
```

- `bool enabled`: Enable or disable the sensor.

### Method: onMotionStart
Register a function to be called when motion starts. This will automatically enable the motion sensor.

```c
void onMotionStart(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: onMotionStop
Register a function to be called when motions stops. This will automatically enable the motion sensor.

```c
void onMotionStop(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: isMoving
Returns `true` if the device is currently moving. Requires the sensor to be enabled via `onMotionStart()`, `onMotionStop()` or `setMotion()`.

## Button

### Method: onButtonPress
Register a function to be called when the button is pressed down.

```c
void onButtonPress(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: onButtonRelease
Register a function to be called when the button is released.

```c
void onButtonRelease(void(*callback)(void));
```

- `void(*callback)(void)`: Function to be called, with no arguments nor return value.

### Method: isButtonPressed
Returns `true` if the button is currently pressed.

## LED
Control the LED.

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

### Method: getColor
Returns the current color of the LED.

```c
TTN_COLOR getColor();
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
String colorToString(TTN_COLOR color);
```

- `TTN_COLOR color`: Color to return a string for.

Returns one of:

- `"Red"`: `TTN_RED`
- `"Green"`: `TTN_GREEN`
- `"Blue"`: `TTN_BLUE`
- `"Yellow"`: `TTN_YELLOW`
- `"Cyan"`: `TTN_CYAN`
- `"Magenta"`: `TTN_MAGENTA`
- `"White"`: `TTN_WHITE`
- `"Black"`: `TTN_BLACK`

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

### Method: setRGB
Turn one or more LEDs on or off.

```c
void setRGB(bool red = false, bool green = false, bool blue = false);
```

- `bool red = false`: Set to `true` to turn the red LED on. Defaults to `false`.
- `bool green = false`: Set to `true` to turn the green LED on. Defaults to `false`.
- `bool blue = false`: Set to `true` to turn the blue LED on. Defaults to `false`.

> You can combine colors to [create additives](https://en.wikipedia.org/wiki/RGB_color_model#Additive_primary_colors), e.g. red plus green makes yellow.

### Method: setColor
Combine one or more LEDs to get a primary or [additive color](https://en.wikipedia.org/wiki/RGB_color_model#Additive_primary_colors).

```c
setColor(TTN_COLOR color);
```

- `TTN_COLOR color`: Set to one of:
  - `TTN_RED`
  - `TTN_GREEN`
  - `TTN_BLUE`
  - `TTN_YELLOW`: Red and green
  - `TTN_CYAN`: Green and blue
  - `TTN_MAGENTA`: Red and blue
  - `TTN_WHITE`: Red, green and blue
  - `TTN_BLACK`: None

## Method: getUSB
Returns `true` if the device is connected via USB.

```c
bool getUSB();
```

## Method: getBattery
Returns the battery level in micro voltage as a unsigned integer of 2 bytes.

```c
uint16_t getBattery();
```
    
