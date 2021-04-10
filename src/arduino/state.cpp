
#include "state.h"

using namespace std;

State::State()
{
}

String State::Serialize()
{
    String state_string = "";
    // Photoresistor
    state_string += String(photoresistor) + ",";

    // Sound Sensor
    state_string += String(sound) + ",";

    // RGB Switches
    state_string += String(red) + ",";
    state_string += String(green) + ",";
    state_string += String(blue) + ",";

    // Encoder (delta)
    state_string += String(encoder) + ",";
    state_string += String(encoderButton) + ",";

    // Distance sensor
    state_string += String(distance) + ",";

    // 4x4 Key Pad
    
    return state_string;
/*
    String state_string = "{";
    // Photoresistor
    state_string += Field("photoresistor", String(photoresistor)) + ",";

    // Sound Sensor
    state_string += Field("sound", String(sound)) + ",";

    // RGB Switches
    state_string += Field("red", String(red)) + ",";
    state_string += Field("green", String(green)) + ",";
    state_string += Field("blue", String(blue)) + ",";

    // Encoder (delta)
    state_string += Field("encoder", String (encoder) + ",");
    state_string += Field("encoderButton", String (encoderButton)) + ",";

    // Distance sensor

    // 4x4 Key Pad
    
    state_string += "}";
    */
}

String State::Field(String name, String value)
{
    return "\"" + name + "\": " + "\"" + value + "\"";
}