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
