
#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

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

class State
{
public:

    #pragma region Inputs
    // Photoresistor
    // Normalized positive value [0, 1]
    float photoresistor;
    bool photoOn;

    // Sound Sensor
    // Positive raw value [0, 1024]
    int sound;
    bool soundOn;

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

    // 4x4 Key Pad
    // Flags of active buttons
    unsigned int keypad;
    
    #pragma endregion Inputs

    #pragma region Outputs

    // Values for the RGB LED
    int red_RGB;
    int green_RGB;
    int blue_RGB;

    // A shape is selected (received from app)
    bool selected;

    #pragma endregion Outputs

    State();
    String Serialize(void);

    void ReadState();
    void DeserializeColor(Color);
};

#endif
