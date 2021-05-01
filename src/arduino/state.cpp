
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
