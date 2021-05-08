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
 *  - 22, 23, 24: always On at the start of the Photoresistor
 *  - 25, 26, 27: sequentially On when photoresistor is activated
 *  - 28, 29: on when the sound goes over a threshold
 *  - 30: always on over the encoder
 *  - 31, 32, 33, 34, 35, 36, 37: encoder value indicators (from 0 to 7)
 *  - 38, 39, 40, 41: on when the distance sensor button is pressed
 * 
 * The sketch gathers the input values  in a State class, which is then
 * serialized and sent over the serial port to the electron p5 application.
 * 
 */

#include "./state.h"
#include "movingAvg.h"

State state = State();

// Serial timing
const int sendPeriod = 15;
int sendTime = 0;

#define SOUND_SPEED 0.0343

// General time management
unsigned long deltaTime = 0;
unsigned long currentMillis = 0;
unsigned long preMillis = 0;
//const unsigned long interval = 50;

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

#pragma endregion PIN SETUP IN

#pragma region PIN SETUP OUT

#define RED_RGB 44
#define GREEN_RGB 45
#define BLUE_RGB 46

#define PHOTO_LED_COUNT 3
const byte PHOTO_LED[PHOTO_LED_COUNT] = { 22, 23, 24 };
#define SOUND_LED_COUNT 3
const byte SOUND_LED[SOUND_LED_COUNT] = { 25, 26, 27 };
#define SOUND_END_LED_COUNT 2
const byte SOUND_END_LED[SOUND_END_LED_COUNT] = { 28, 29 };

#define ENCODER_LED_COUNT 7
const byte ENCODER_ON_LED = 30;
const byte ENCODER_LED[ENCODER_LED_COUNT] = { 31, 32, 33, 34, 35, 36, 37 };

#define DISTANCE_LED_COUNT 4
const byte DISTANCE_LED[DISTANCE_LED_COUNT] = { 38, 39, 40, 41 };


#pragma endregion PIN SETUP OUT

#pragma region SENSOR VARIABLES

// Photoresistor/Sound variables
const float lightThreshold = 0.6f;
int initialPhotoValue;
const float soundThreshold = 0.95f;
int initialSoundValue;
movingAvg soundSensor(5);
int soundLEDsOn = 0;
unsigned long soundOnTime = 0;

// Encoder variables
int encoderCurrentCLK;
int encoderPrevCLK;

// Distance sensor variables
unsigned long lastDistanceSwitchPress = 0;
movingAvg distanceSensor(10);
int distanceLEDsOn = 0;
unsigned long distanceOnTime = 0;

// Keypad variables
unsigned int keysDebounceTime = 10;
unsigned long keysStartTime = 0;

#pragma endregion SENSORS VARIABLES

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
    // Initial read for "environment calibration"
    initialSoundValue = analogRead(SOUND_PIN);
    soundSensor.begin();
    soundSensor.reading(initialSoundValue);

    // RGB Switches
    pinMode(RED_SWITCH_PIN, INPUT);
    pinMode(GREEN_SWITCH_PIN, INPUT);
    pinMode(BLUE_SWITCH_PIN, INPUT);
    
    // Encoder
    pinMode(ENCODER_CLK_PIN, INPUT);
    pinMode(ENCODER_DT_PIN, INPUT);
    encoderPrevCLK = digitalRead(ENCODER_CLK_PIN);

    // Distance Sensor
    pinMode(DISTANCE_SWITCH_PIN, INPUT);
    pinMode(DISTANCE_TRIG_PIN, OUTPUT);
    pinMode(DISTANCE_ECHO_PIN, INPUT);
    distanceSensor.begin();
    
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
    for (int i = 0; i < PHOTO_LED_COUNT; i++)
    {
        pinMode(PHOTO_LED[i], OUTPUT);
        digitalWrite(PHOTO_LED[i], HIGH);
    }

    // Sound sensor LEDs (on when the photo is covered)
    for (int i = 0; i < SOUND_LED_COUNT; i++)
    {
        pinMode(SOUND_LED[i], OUTPUT);
    }

    // Sound sensor LEDs at the end
    for (int i = 0; i < SOUND_END_LED_COUNT; i++)
    {
        pinMode(SOUND_END_LED[i], OUTPUT);
    }

    // Encoder single LED (always on)
    pinMode(ENCODER_ON_LED, OUTPUT);
    digitalWrite(ENCODER_ON_LED, HIGH);

    // Encoder LEDs
    for (int i = 0; i < ENCODER_LED_COUNT; i++)
    {
        pinMode(ENCODER_LED[i], OUTPUT);
    }

    // Distance LEDs
    for (int i = 0; i < DISTANCE_LED_COUNT; i++)
    {
        pinMode(DISTANCE_LED[i], OUTPUT);
    }
}

void loop()
{
    currentMillis = millis();
    deltaTime = currentMillis - preMillis;

    // Read Inputs

    ReadPhotoresistor();
    ReadSoundSensor();

    ReadRGBSwitches();
    ReadEncoder();
    ReadDistanceSensor();
    ReadKeyPad();

    // Update outputs

    UpdateOutputs();

    // Read Serial
    
    if(Serial.available() > 0)
    {
        ReceiveState();
        SetRGBColor();
    }
    
    // Send state update 

    if(currentMillis - sendTime > sendPeriod)
    {
        SendState();
        sendTime = currentMillis;
    }

    preMillis = currentMillis;
}

#pragma region Inputs

void ReadPhotoresistor()
{
    state.photoresistor = analogRead(PHOTO_PIN) / (float) initialPhotoValue;
    state.photoOn = state.photoresistor < lightThreshold;
}

void ReadSoundSensor()
{
    if(state.photoresistor < lightThreshold)
    {
        int soundRead = analogRead(SOUND_PIN);
        soundSensor.reading(soundRead);
        int avgRead = soundSensor.getAvg();

        state.sound = avgRead / (float) initialSoundValue, 0, 1;
        state.soundOn = state.sound < soundThreshold;
    }
    else
    {
        state.sound = 0;
        state.soundOn = false;
    }
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
        
        if (encoderDT != encoderCurrentCLK)
        {
            state.encoder--;
        }
        else
        {
            state.encoder++;
        }

    // Serial.print(encoderCurrentCLK);
    // Serial.print(", ");
    // Serial.print(encoderDT);
    // Serial.print(": ");
    // Serial.print(state.encoder);
    // Serial.println("");

    }

    encoderPrevCLK = encoderCurrentCLK;
}

void ReadDistanceSensor()
{
    // Debounced button press to activate teh distance sensor
    int button = digitalRead(DISTANCE_SWITCH_PIN);

    if (button == HIGH)
    {
		if (currentMillis - lastDistanceSwitchPress > 50)
        {
			state.distanceButton = true;
		}

        lastDistanceSwitchPress = currentMillis;
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

        // Store for average smoothing
        soundSensor.reading(duration);

        // Calculate distance based on speed of sound
        state.distance = (soundSensor.getAvg() * SOUND_SPEED) / 2;
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
    if ((currentMillis - keysStartTime) > keysDebounceTime)
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

        keysStartTime = currentMillis;
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

void UpdateOutputs()
{
    UpdateSoundOutput();
    UpdateDistanceOutput();
    UpdateEncoderOutput();
}

void UpdateSoundOutput()
{
    UpdateLEDSequence(SOUND_LED, SOUND_LED_COUNT, 50, state.photoOn, &soundLEDsOn, &soundOnTime);
    SetLEDGroup(SOUND_END_LED, SOUND_END_LED_COUNT, state.soundOn);
}

void UpdateDistanceOutput()
{
    UpdateLEDSequence(DISTANCE_LED, DISTANCE_LED_COUNT, 50, state.distanceButton, &distanceLEDsOn, &distanceOnTime);
}

void UpdateEncoderOutput()
{
    int encoderLEDs = state.encoder;
    SetLEDGroupToValue(ENCODER_LED, ENCODER_LED_COUNT, encoderLEDs);
}

// General method to turn a sequence of LEDs on sequentially with configurable intervals
bool UpdateLEDSequence(const byte* LEDGroup, const int LEDCount, const int interval, bool trigger, int* LEDsOnCount, unsigned long* onTime)
{
     if(trigger)
    {
        *onTime += deltaTime;

        if(*onTime >= interval)
        {
            *onTime = 0;
            *LEDsOnCount = constrain(*LEDsOnCount + 1, 0, LEDCount);
            SetLEDGroup(LEDGroup, *LEDsOnCount, trigger);
        }
    }
    else
    {
        *LEDsOnCount = 0;
        *onTime = 0;
        
        SetLEDGroup(LEDGroup, LEDCount, LOW);
    }

    return LEDCount == *LEDsOnCount;
}

void SetLEDGroup(const byte* array, int count, byte state)
{
    for(int i = 0; i < count; i++)
    {
        digitalWrite(array[i], state);
    }
}

// Sets a group of LEDs ON depending on the input value:
//
void SetLEDGroupToValue(const byte* array, int count, int value)
{
    if(value > 0)
    {
        for(int i = 0; i < count; i++)
        {
            if(i + 1 <= value)
            {
                digitalWrite(array[i], HIGH);
            }
            else
            {
                digitalWrite(array[i], LOW);
            }
        }
    }
    else if (value < 0)
    {
        for(int i = 0; i < count; i++)
        {
            if(i + 1 <= count + value)
            {
                digitalWrite(array[i], LOW);
            }
            else
            {
                digitalWrite(array[i], HIGH);
            }
        }
    }
    else // value == 0
    {
        SetLEDGroup(array, count, LOW);
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
    //Serial.println(state.Serialize());
}