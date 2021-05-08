
#include "state.h"

using namespace std;

State::State()
{
}

String State::Serialize()
{
    String state = "";
    // Photoresistor
    state += String(photoresistor) + ",";

    // Sound Sensor
    state += String(sound) + ",";

    // RGB Switches
    state += String(red) + ",";
    state += String(green) + ",";
    state += String(blue) + ",";

    // Encoder (delta)
    state += String(encoder) + ",";

    // Distance sensor
    state += String(distance) + ",";
    state += String(distanceButton) + ",";

    // 4x4 Key Pad
    state += String(keypad);
    
    return state;
}

#define BUFFER_SIZE 10;
void State::ReadState()
{   
    DeserializeColor((Color) Serial.parseInt());
    selected = Serial.parseInt();

    while(Serial.available() > 0)
    {
        Serial.read();
    }
}

void State::DeserializeColor(Color color)
{
    switch(color)
    {
        case RED:
            red_RGB = 235;
            green_RGB = 52;
            blue_RGB = 52;
        break;
        case GREEN:
            red_RGB = 58;
            green_RGB = 235;
            blue_RGB = 52;
        break;
        case BLUE:
            red_RGB = 52;
            green_RGB = 55;
            blue_RGB = 235;
        break;
        case YELLOW:
            red_RGB = 235;
            green_RGB = 229;
            blue_RGB = 52;
        break;
        case MAGENTA:
            red_RGB = 235;
            green_RGB = 52;
            blue_RGB = 140;
        break;
        case CYAN:
            red_RGB = 52;
            green_RGB = 235;
            blue_RGB = 229;
        break;
        case WHITE:
            red_RGB = 255;
            green_RGB = 255;
            blue_RGB = 255;
        break;
        case NONE:
            red_RGB = 0;
            green_RGB = 0;
            blue_RGB = 0;
        break;
        break;
    }
}
