
#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

class State
{
public:

    #pragma region Inputs
    // Photoresistor
    // Normalized positive value [0, 1]
    float photoresistor;
    byte photoOut;

    // Sound Sensor
    // Normalized positive value [0, 1]
    int sound;

    // RGB Switches
    // Booleans (on/off)
    bool red;
    bool green;
    bool blue;

    // Encoder accumulated value, can be positive or negative
    int encoder = 0;

    // Distance sensor
    // Positive distance value (in centimeters?) may need regulation
    float distance;
    // Activate distance sensor reading
    bool distanceButton;
    byte distanceOut;

    // 4x4 Key Pad
    // Flags of active buttons
    unsigned int keypad;
    
    #pragma endregion Inputs

    #pragma region Outputs

    int red_RGB;
    int green_RGB;
    int blue_RGB;

    #pragma endregion Outputs

    State();
    String Serialize(void);
    void Deserialize(String);
};

enum Color
{
    RED,
    GREEN,
    BLUE,
    MAGENTA,
    CYAN,
    YELLOW,
    WHITE,
    NONE
};

#endif
