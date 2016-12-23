#include <TheThingsNode.h>

TheThingsNode *node;

void setup() {
    node = TheThingsNode::setup();
    node->onWake(voidFn);
    node->onSleep(voidFn);
    node->onInterval(onInterval);
    node->configInterval(false, 60000);
    node->configInterval(true);
    node->configLight(false, 2);
    node->configLight(true);
    node->configTemperature(false, R_DEGREES_0_2500, 0, 30, 50, H_DEGREES_1_5);
    node->configTemperature(false, R_DEGREES_0_2500);
    node->configTemperature(true);
    node->onTemperature(voidFn);
    node->onMotionStart(voidFn);
    node->onMotionStop(durationFn);
    node->configMotion(true);
    node->onButtonPress(voidFn);
    node->onButtonRelease(durationFn);
    node->configButton(true);
    node->setColor(TTN_RED);
    node->setColor(TTN_GREEN);
    node->setColor(TTN_BLUE);
    node->setColor(TTN_YELLOW);
    node->setColor(TTN_CYAN);
    node->setColor(TTN_MAGENTA);
    node->setColor(TTN_WHITE);
    node->setColor(TTN_BLACK);
    node->setRGB(true, false, true);
    node->setRed(true);
    node->setGreen(false);
    node->setBlue(true);
    node->configUSB(true);
}

void voidFn() {}

void durationFn(unsigned long duration) {}

void loop() {
    node->loop();
}

void onInterval() {
    node->showStatus();
    uint16_t light = node->getLight();
    int8_t tempFloat = node->getTemperatureAsFloat();
    bool alert = node->hasTemperatureAlert();
    bool moving = node->isMoving();
    bool pressed = node->isButtonPressed();
    ttn_color color = node->getColor();
    String colorStr = node->colorToString(color);
    bool red = node->getRed();
    bool green = node->getGreen();
    bool blue = node->getBlue();
    bool connected = node->isUSBConnected();
    uint16_t battery = node->getBattery();
}
