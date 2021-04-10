/*
 * Android Input sketch
 * 
 * Input for the application, pins and breadboard positions (if any):
 *  - A0: Photoresistor, 50 - 5V, 52 - R - Gn
 *  - A1: Sound Sensor, 60+ e 
 *  - A3: Red switch
 *  - A4: Green switch
 *  - A5: Blue switch
 *  - 2 - 9: 4x4 button pad
 *  - A2, 10, 11: Rotatry Encoder, 
 *  - 12, 13: Distance Sensor
 *  
 * The sketch gathers the input values  in a State class, which is then
 * serialized and sent over the serial port to the electron p5 application.
 * 
 */

#include "./state.h"

State state = State();

#pragma region PIN SETUP

// Photoresistor - A0
#define PHOTO_PIN A0

// Sound Sensor - A1
#define SOUND_PIN A1

// Switches (as digital inputs) - A3, A4, A5
#define RED_SWITCH_PIN A3
#define GREEN_SWITCH_PIN A4
#define BLUE_SWITCH_PIN A5

// Encoder - A2 (switch), 10, 11
#define ENCODER_SW_PIN A2
#define ENCODER_DT_PIN 10
#define ENCODER_CLK_PIN 11

// Distance Sensor - 12, 13 
#define DISTANCE_TRIG_PIN 12
#define DISTANCE_ECHO_PIN 13

// 4x4 Button pad - 2 - 9
const byte KEYS_ROW_PIN[4] = {2, 3, 4, 5};
const byte KEYS_COL_PIN[4] = {6, 7, 8, 9};
unsigned int keysDebounceTime = 10;
unsigned long keysStartTime = 0;

/* OLD KEY PAD MANAGEMENT
#define KEYS_ROW1_PIN 2
#define KEYS_ROW2_PIN 3
#define KEYS_ROW3_PIN 4
#define KEYS_ROW4_PIN 5

#define KEYS_COL1_PIN 6
#define KEYS_COL2_PIN 7
#define KEYS_COL3_PIN 8
#define KEYS_COL4_PIN 9
*/

#pragma endregion PIN SETUP

void setup()
{
    Serial.begin(9600);

    // Pins setup

    // Photoresistor
    pinMode(PHOTO_PIN, INPUT);

    // Sound sensor
    pinMode(SOUND_PIN, INPUT);

    // RGB Switches
    pinMode(RED_SWITCH_PIN, INPUT);
    pinMode(GREEN_SWITCH_PIN, INPUT);
    pinMode(BLUE_SWITCH_PIN, INPUT);
    
    // Encoder
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT);
    pinMode(ENCODER_CLK_PIN, INPUT);

    // Distance Sensor
    pinMode(DISTANCE_TRIG_PIN, OUTPUT);
    pinMode(DISTANCE_ECHO_PIN, INPUT);
    
    // 4x4 Key pad
    for(int i = 0; i < 4; i++)
    {
        pinMode(KEYS_ROW_PIN[i], OUTPUT);
        pinMode(KEYS_COL_PIN[i], INPUT);
    }
    /* OLD KEY PAD MANAGEMENT
    pinMode(KEYS_ROW1_PIN, INPUT);
    pinMode(KEYS_ROW2_PIN, INPUT);
    pinMode(KEYS_ROW3_PIN, INPUT);
    pinMode(KEYS_ROW4_PIN, INPUT);

    pinMode(KEYS_COL1_PIN, INPUT);
    pinMode(KEYS_COL2_PIN, INPUT);
    pinMode(KEYS_COL3_PIN, INPUT);
    pinMode(KEYS_COL4_PIN, INPUT);
    */
}

void loop()
{
    ReadPhotoresistor();
    ReadSoundSensor();
    ReadRGBSwitches();
    //ReadEncoder();
    //ReadDistanceSensor();
    //ReadKeyPad();

    SendState();
}

void ReadPhotoresistor()
{
    /* If needed, this would go in setup() or its own function
    // Initial read for "environment calibration"
    // initialPhotoValue = analogRead(PHOTO_PIN);
    */
    state.photoresistor = analogRead(PHOTO_PIN);
}

void ReadSoundSensor()
{
    state.sound = analogRead(SOUND_PIN);
}

void ReadRGBSwitches()
{
    state.red = digitalRead(RED_SWITCH_PIN) == HIGH ? true : false;
    state.green = digitalRead(GREEN_SWITCH_PIN) == HIGH ? true : false;
    state.blue = digitalRead(BLUE_SWITCH_PIN) == HIGH ? true : false;
}

void ReadEncoder()
{
    static int previousValue;
    int currentValue;


}

void ReadDistanceSensor()
{
    
}

// Borrowed and adapted from Keypad library
// Comments are copied verbatim to allow understanding
void ReadKeyPad()
{
    // Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
    if ((millis() - keysStartTime) > keysDebounceTime)
    {
	    // Re-intialize the row pins. Allows sharing these pins with other hardware.
        for(byte row = 0; row < 4; row++)
        {
            pinMode(KEYS_ROW_PIN[row], INPUT_PULLUP);
        }

        for(byte col = 0; col < 4; col++)
        {
            pinMode(KEYS_COL_PIN[col], OUTPUT);
            
            // Begin column pulse output.
            digitalWrite(KEYS_COL_PIN[col], LOW);

            for(byte row = 0; row < 4; row++)
            {
                // keypress is active low so invert to high.
                int val = !digitalRead(KEYS_ROW_PIN[row]);
               
                if(val == HIGH)
                {
                    state.keypad |= (1 << col*row);
                }
                else
                {
                    state.keypad &= ~(1 << col*row);

                }
            }

		    // Set pin to high impedance input. Effectively ends column pulse.
            digitalWrite(KEYS_COL_PIN[col], HIGH);
            pinMode(KEYS_COL_PIN[col], INPUT);
        }

        keysStartTime = millis();
    }
}

void SendState()
{
    Serial.println(state.Serialize());
}