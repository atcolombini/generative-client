
#include "state.h"

using namespace std;

State::State()
{
}

void State::WriteState()
{
    Serial.print(photoresistor);
    Serial.print(",");
    Serial.print(sound);
    Serial.print(",");
    Serial.print(red);
    Serial.print(",");
    Serial.print(green);
    Serial.print(",");
    Serial.print(blue);
    Serial.print(",");
    Serial.print(encoder);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(",");
    Serial.print(distanceButton);
    Serial.print(",");
    Serial.print(keypad);
    Serial.println();
}

void State::ReadState()
{   
    DeserializeColor((Color) Serial.parseInt());
    selected = Serial.parseInt() == 0 ?  false : true;
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
