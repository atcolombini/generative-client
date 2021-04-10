
#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

class State
{
public:
    // Photoresistor
    // Normalized positive value [0, 1]
    int photoresistor;

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
    int encoderDelta;

    // Distance sensor
    // Positive distance value (in centimeters?)
    int distance;

    enum KeyPadState
    {
        KEY_1 = 1 << 0,
        KEY_2 = 1 << 1,
        KEY_3 = 1 << 2,
        KEY_4 = 1 << 3,
        KEY_5 = 1 << 4,
        KEY_6 = 1 << 5,
        KEY_7 = 1 << 6,
        KEY_8 = 1 << 7,
        KEY_9 = 1 << 8,
        KEY_10 = 1 << 9,
        KEY_11 = 1 << 10,
        KEY_12 = 1 << 11,
        KEY_13 = 1 << 12,
        KEY_14 = 1 << 13,
        KEY_15 = 1 << 14,
        KEY_16 = 1 << 15
    };

    // 4x4 Key Pad
    // Flags of active buttons
    unsigned int keypad;

    State();
    String Serialize(void);
};

#endif