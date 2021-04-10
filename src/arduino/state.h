
#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

class State
{
public:
    // Photoresistor
    // Normalized positive value [0, 1]
    float photoresistor;

    // Sound Sensor
    // Normalized positive value [0, 1]
    int sound;

    // RGB Switches
    // Booleans (on/off)
    bool red;
    bool green;
    bool blue;

    // Encoder (change from previous state)
    // Positive or negative value
    int encoder = 0;
    bool encoderButton;

    // Distance sensor
    // Positive distance value (in centimeters?)
    float distance;

    // 4x4 Key Pad
    // Flags of active buttons
    unsigned int keypad;

    State();
    String Serialize(void);

private:
    String Field(String, String);
};

#endif