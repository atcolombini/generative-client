/*
 * Android Input sketch
 * 
 * Input for the application, pins and breadboard positions (if any):
 *  - A0: Photoresistor, 50 - 5V, 52 - R - Gn
 *  - A1: Sound Sensor, 60+ e 
 *  - 14: Distance activation button
 *  - 15: Red switch
 *  - 16: Green switch
 *  - 17: Blue switch
 *  - 2 - 9: 4x4 button pad
 *  - 10, 11: Rotatry Encoder
 *  - 12, 13: Distance Sensor
 *  
 * Outputs (LEDs):
 *  - 44, 45, 46: PWM outputs respectively for the RGB LED
 *  - 
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

#pragma region PIN SETUP IN

// Photoresistor - A0
#define PHOTO_PIN A0

// Sound Sensor - A1
#define SOUND_PIN A1

// Button - 14 (switch)
#define DISTANCE_SWITCH_PIN 14

// Switches (as digital inputs) - 15, 16, 17
#define RED_SWITCH_PIN 15
#define GREEN_SWITCH_PIN 16
#define BLUE_SWITCH_PIN 17

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

#pragma endregion PIN SETUP IN

#pragma region PIN SETUP OUT

#define RED_RGB 44
#define GREEN_RGB 45
#define BLUE_RGB 46

const byte PHOTO_LED[3] = { 22, 23, 24 };
const byte SOUND_LED[3] = { 25, 26, 27};
const byte SOUND_END_LED[2] = { 28, 29 };

const byte ENCODER_LED[8] = { 30, 31, 32, 33, 34, 35, 36, 37 };

const byte DISTANCE_LED[5] = { 38, 39, 40, 41, 42 };

#pragma endregion PIN SETUP OUT

void setup()
{
    Serial.begin(9600);

    // Input Pins Setup

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

    // Output Pins Setup

    pinMode(RED_RGB, OUTPUT);
    pinMode(GREEN_RGB, OUTPUT);
    pinMode(BLUE_RGB, OUTPUT);

    // Photoresistor LEDs (always on?)
    for (int i = 0; i < 3; i++)
    {
        pinMode(PHOTO_LED[i], OUTPUT);
    }

    // Sound sensor LEDs (on when the photo is covered)
    for (int i = 0; i < 3; i++)
    {
        pinMode(SOUND_LED[i], OUTPUT);
    }

    // Sound sensor LEDs at the end
    for (int i = 0; i < 2; i++)
    {
        pinMode(SOUND_END_LED[i], OUTPUT);
    }

    // Encoder LEDs (started sequentially)
    for (int i = 0; i < 8; i++)
    {
        pinMode(ENCODER_LED[i], OUTPUT);
    }

    // Distance LEDs
    for (int i = 0; i < 5; i++)
    {
        pinMode(DISTANCE_LED[i], OUTPUT);
    }
}

void loop()
{

    // Read Inputs

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

    // Update outputs

    DistanceLEDs();

    // Read Serial
    
    if(Serial.available() > 0)
    {
        ReceiveState();
        SetRGBColor();
    }
    
    // Send state update 

    if(millis() - sendTime > sendPeriod)
    {
        SendState();
        sendTime = millis();
    }
}

#pragma region Inputs

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
    int encoderDT = 0;

    encoderCurrentCLK = digitalRead(ENCODER_CLK_PIN);

    // React to only 1 state change to avoid double count
    if (encoderCurrentCLK != encoderPrevCLK && encoderCurrentCLK == 1)
    {
        encoderDT = digitalRead(ENCODER_DT_PIN);

        if (digitalRead(ENCODER_DT_PIN) != encoderCurrentCLK )
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

#pragma endregion Inputs

#pragma region Outputs

void SetRGBColor()
{
    analogWrite(RED_RGB, state.red_RGB);
    analogWrite(GREEN_RGB, state.green_RGB);
    analogWrite(BLUE_RGB, state.blue_RGB);
}

void DistanceLEDs()
{
    
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(DISTANCE_LED[i], state.distanceButton ? HIGH : LOW);
    }
}

#pragma endregion Outputs

void ReceiveState()
{
    String data = Serial.readStringUntil('\n');
    state.Deserialize(data);
}

void SendState()
{
    Serial.println(state.Serialize());
}