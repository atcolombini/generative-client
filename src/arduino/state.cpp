
#include "state.h"

using namespace std;

State::State()
{
}

String State::Serialize()
{
    String state_string = "{";

    // Photoresistor
    state_string += "photoresistor: " + String(photoresistor) + ",";

    // Sound Sensor
    state_string += "sound: " + String(sound) + ",";

    // RGB Switches
    state_string += "red " + String(red) + ",";
    state_string += "green " + String(green) + ",";
    state_string += "blue " + String(blue) + ",";

    // Encoder (delta)

    // Distance sensor

    // 4x4 Key Pad
    
    state_string += "}";
    return state_string;
}
