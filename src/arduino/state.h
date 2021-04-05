
#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

class State
{
public:
    // Photoresistor
    int photoresistor;

    // Sound Sensor
    int sound;

    // RGB Buttons
    bool red;
    bool green;
    bool blue;

    // Encoder (change from previous state)
    int encoderDelta;

    // Distance sensor
    int distance;

    // 4x4 Button Pad
    

    State();
    String to_string(void);
};

#endif