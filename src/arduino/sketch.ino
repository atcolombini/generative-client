/*
 * Android Input sketch
 * 
 * Input for the application, pins and breadboard positions (if any):
 *  - A0: Photoresistor, 50 - 5V, 52 - R - Gn
 *  - A1: Sound Sensor, 60+ e 
 *  - A2: Distance activation button
 *  - A3: Red switch
 *  - A4: Green switch
 *  - A5: Blue switch
 *  - 2 - 9: 4x4 button pad
 *  - 10, 11: Rotatry Encoder
 *  - 12, 13: Distance Sensor
 *  
 * The sketch gathers the input values  in a State class, which is then
 * serialized and sent over the serial port to the electron p5 application.
 * 
 */

#include "./state.h"

State state = State();

const int sendPeriod = 15;
int sendTime = 0;

const float lightThreshold = 0.6;

#define SOUND_SPEED 0.0343

#pragma region PIN SETUP

// Photoresistor - A0
#define PHOTO_PIN A0

// Sound Sensor - A1
#define SOUND_PIN A1

// Button - A2 (switch)
#define DISTANCE_SWITCH_PIN A2

// Switches (as digital inputs) - A3, A4, A5
#define RED_SWITCH_PIN A3
#define GREEN_SWITCH_PIN A4
#define BLUE_SWITCH_PIN A5

// Encoder - 10, 11
#define ENCODER_DT_PIN 10
#define ENCODER_CLK_PIN 11

// Distance Sensor - 12, 13 
#define DISTANCE_TRIG_PIN 12
#define DISTANCE_ECHO_PIN 13

// 4x4 Key pad - 2 - 9
const byte KEYS_ROW_PIN[4] = { 2, 3, 4, 5 };
const byte KEYS_COL_PIN[4] = { 6, 7, 8, 9 };

// Photoresistor variables
int initialPhotoValue;

// Encoder variables
int encoderCurrentCLK;
int encoderPrevCLK;
unsigned long lastDistanceSwitchPress = 0;

// Keypad variables
unsigned int keysDebounceTime = 10;
unsigned long keysStartTime = 0;

#pragma endregion PIN SETUP

void setup()
{
    Serial.begin(9600);

    // Pins setup

    // Photoresistor
    pinMode(PHOTO_PIN, INPUT);
    // Initial read for "environment calibration"
    initialPhotoValue = analogRead(PHOTO_PIN);

    // Sound sensor
    pinMode(SOUND_PIN, INPUT);
    // TODO Initial read for "environment calibration"

    // RGB Switches
    pinMode(RED_SWITCH_PIN, INPUT);
    pinMode(GREEN_SWITCH_PIN, INPUT);
    pinMode(BLUE_SWITCH_PIN, INPUT);
    
    // Encoder
    pinMode(ENCODER_DT_PIN, INPUT);
    pinMode(ENCODER_CLK_PIN, INPUT);
    state.encoderDelta = 0;
    encoderPrevCLK = digitalRead(ENCODER_CLK_PIN);

    // Distance Sensor
    pinMode(DISTANCE_SWITCH_PIN, INPUT);
    pinMode(DISTANCE_TRIG_PIN, OUTPUT);
    pinMode(DISTANCE_ECHO_PIN, INPUT);
    
    // 4x4 Key pad
    for (int i = 0; i < 4; i++)
    {
        pinMode(KEYS_ROW_PIN[i], OUTPUT);
        pinMode(KEYS_COL_PIN[i], INPUT);
    }
}

void loop()
{
    ReadPhotoresistor();

    if(state.photoresistor < lightThreshold)
    {
        ReadSoundSensor();
    }
    else
    {
        state.sound = 0;
    }

    ReadRGBSwitches();
    ReadEncoder();
    ReadDistanceSensor();
    ReadKeyPad();

    if(millis() - sendTime > sendPeriod)
    {
        SendState();
        sendTime = millis();
    }
}

void ReadPhotoresistor()
{
    state.photoresistor = analogRead(PHOTO_PIN) / (float) initialPhotoValue;
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
    encoderCurrentCLK = digitalRead(ENCODER_CLK_PIN);

    if (encoderCurrentCLK != encoderPrevCLK)
    {
        if (digitalRead(ENCODER_DT_PIN) != encoderCurrentCLK)
        {
            state.encoder--;
        }
        else
        {
            state.encoder++;
        }
    }

    encoderPrevCLK = encoderCurrentCLK;
}

void ReadDistanceSensor()
{
    // Debounced button press to activate teh distance sensor
    int button = digitalRead(DISTANCE_SWITCH_PIN);

    if (button == HIGH)
    {
		if (millis() - lastDistanceSwitchPress > 50)
        {
			state.distanceButton = true;
		}

        lastDistanceSwitchPress = millis();
	}
    else
    {
        state.distanceButton = false;
    }

    if(state.distanceButton == true)
    {
        // Set to low to setup a pulse
        digitalWrite(DISTANCE_TRIG_PIN, LOW);
        delayMicroseconds(2);
        // Send 10 microsecond pulse
        digitalWrite(DISTANCE_TRIG_PIN, HIGH);
        delayMicroseconds(10);

        digitalWrite(DISTANCE_TRIG_PIN, LOW);

        // Read pulse on the receiver
        unsigned long duration = pulseIn(DISTANCE_ECHO_PIN, HIGH);

        // Calculate distance based on speed of sound
        state.distance = (duration * SOUND_SPEED) / 2;
    }
    else
    {
        state.distance = 0;
    }
}

// Borrowed and adapted from Keypad library
// Comments are copied verbatim to allow understanding
void ReadKeyPad()
{
    // Limit how often the keypad is scanned. This makes the loop() run 10 times as fast.
    if ((millis() - keysStartTime) > keysDebounceTime)
    {
	    // Re-intialize the row pins. Allows sharing these pins with other hardware.
        for (byte row = 0; row < 4; row++)
        {
            pinMode(KEYS_ROW_PIN[row], INPUT_PULLUP);
        }

        for (byte col = 0; col < 4; col++)
        {
            pinMode(KEYS_COL_PIN[col], OUTPUT);
            
            // Begin column pulse output.
            digitalWrite(KEYS_COL_PIN[col], LOW);

            for (byte row = 0; row < 4; row++)
            {
                // keypress is active low so invert to high.
                int val = !digitalRead(KEYS_ROW_PIN[row]);
                int selectedSwitch = col + row * 4;
                //Serial.println("switch: " + String(col + row * 4));

                // Set state using bit masks
                if (val == HIGH)
                {
                    state.keypad |= (1 << selectedSwitch);
                }
                else
                {
                    state.keypad &= ~(1 << selectedSwitch);
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