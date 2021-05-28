/*
 * Generative Artifact
 * Copyright (C) 2021 Vicent Ramirez
 * Copyright (C) 2021 Alex T. Colombini
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include "string.h"

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
    void WriteState();
    void ReadState();
    void DeserializeColor(Color);
};

#endif
