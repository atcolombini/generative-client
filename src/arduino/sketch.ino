/*
 * Android Input sketch
 * 
 * Input for the application, pins:
 *  - A0: Photoresistor
 *  - A1: Sound Sensor
 *  - A3: Red switch
 *  - A4: Blue switch
 *  - A5: Green switch
 *  - 0 - 7: 4x4 button pad
 *  - 8 - 10: Rotatry Encoder
 *  - 11, 12: Distance Sensor
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

// 4x4 Button pad - 0 - 7
#define BUTTONS_0_PIN 0
#define BUTTONS_1_PIN 1
#define BUTTONS_2_PIN 2
#define BUTTONS_3_PIN 3
#define BUTTONS_4_PIN 4
#define BUTTONS_5_PIN 5
#define BUTTONS_6_PIN 6
#define BUTTONS_7_PIN 7

// Encoder - 8, 9, 10
#define ENCODER_SW_PIN 8
#define ENCODER_DT_PIN 9
#define ENCODER_CLK_PIN 10

// Distance Sensor - 11, 12
#define DISTANCE_ECHO_PIN 11
#define DISTANCE_TRIG_PIN 12

#pragma endregion PIN SETUP

void setup()
{
    Serial.begin(9600);

    // Pins setup
    // Photoresistor
    pinMode(PHOTO_PIN, INPUT);
    // Sound sensor
    pinMode(SOUND_PIN, INPUT);
    // RGB Buttons

    pinMode(RED_SWITCH_PIN, INPUT_PULLUP);
    pinMode(GREEN_SWITCH_PIN, INPUT);
    pinMode(BLUE_SWITCH_PIN, INPUT);
    
    // 4x4 Button pad
    pinMode(BUTTONS_0_PIN, INPUT);
    pinMode(BUTTONS_1_PIN, INPUT);
    pinMode(BUTTONS_2_PIN, INPUT);
    pinMode(BUTTONS_3_PIN, INPUT);
    pinMode(BUTTONS_4_PIN, INPUT);
    pinMode(BUTTONS_5_PIN, INPUT);
    pinMode(BUTTONS_6_PIN, INPUT);
    pinMode(BUTTONS_7_PIN, INPUT);

    // Encoder - 8, 9, 10
    pinMode(ENCODER_SW_PIN, INPUT);
    pinMode(ENCODER_DT_PIN, INPUT);
    pinMode(ENCODER_CLK_PIN, INPUT);

    // Distance Sensor - 11, 12
    pinMode(DISTANCE_ECHO_PIN, INPUT);
    pinMode(DISTANCE_TRIG_PIN, OUTPUT);
}

void loop()
{
    ReadRGBSwitches();
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

void ReadButtonPad()
{

}

void ReadDistanceSensor()
{

}

void SendState()
{
    Serial.println(state.to_string());
}