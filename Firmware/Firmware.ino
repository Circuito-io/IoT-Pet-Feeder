// Include Libraries
#include "Arduino.h"
#include "ESP8266.h"
#include "dweet.h"
#include "PIR.h"
#include "Servo.h"
#include "PiezoSpeaker.h"


// Pin Definitions
#define ESP8266_PIN_RX	10
#define ESP8266_PIN_TX	11
#define PIR_PIN_SIG	4
#define SERVOSM_PIN_SIG	3
#define THINSPEAKER_PIN_POS	2



const int ServoRestPosition   = 20;  //Starting position
const int ServoTargetPosition = 150; //Position when event is detected
unsigned int HoorayLength          = 6;                                                      // amount of notes in melody
unsigned int HoorayMelody[]        = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5}; // list of notes. List length must match HoorayLength!
unsigned int HoorayNoteDurations[] = {8      , 8      , 8      , 4      , 8      , 4      }; // note durations; 4 = quarter note, 8 = eighth note, etc. List length must match HoorayLength!

unsigned int comeLength          = 3;
unsigned int comeMelody[]        = {NOTE_C5, NOTE_G5, NOTE_C5};
unsigned int comeNoteDurations[] = {8      , 8      , 8};

ESP8266 wifi(ESP8266_PIN_RX,ESP8266_PIN_TX);
Servo servo;
PIR pir(PIR_PIN_SIG);
PiezoSpeaker PiezoSpeaker(THINSPEAKER_PIN_POS);

int pirCounter = 0;
// ====================================================================
// vvvvvvvvvvvvvvvvvvvv ENTER YOUR WI-FI SETTINGS  vvvvvvvvvvvvvvvvvvvv
//
const char *SSID     = "WIFI-SSID"; // Enter your Wi-Fi name 
const char *PASSWORD = "PASSWORD" ; // Enter your Wi-Fi password
//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// ====================================================================


// These Dweet tokens have been auto generated for you.
char* const inputToken  = "YOURTOKEN_input";
char* const outputToken = "YOURTOKEN_output";

Dweet dweet( &wifi, inputToken, outputToken);

/* This code sets up the essentials for your circuit to work. It runs first every time your circuit is powered with electricity. */
void setup() {
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    Serial.println("start");
    
    if (!wifi.init(SSID, PASSWORD))
    {
        Serial.println(F("Wifi Init failed. Check configuration."));
        while (true) ; // loop eternally
    }
    servo.attach(SERVOSM_PIN_SIG);
    servo.write(ServoRestPosition);
    delay(100);
    servo.detach();
}

/* This code is the main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop. */
void loop() {

    bool pirVal = pir.read();
    
    //SET DWEETS
    dweet.setDweet("pir", pirVal );
    dweet.setDweet("pirCounter",  pirCounter);
    
    
    dweet.sendDweetKeys();
    
    
    if (pirVal)
    {
        pirCounter++;
        // The servo will rotate to target position and back to resting position with an interval of 500 milliseconds (0.5 seconds) 
        servo.attach(SERVOSM_PIN_SIG);         // 1. attach the servo to correct pin to control it.
        servo.write(ServoTargetPosition);  // 2. turns servo to target position. Modify target position by modifying the 'ServoTargetPosition' definition above.
        delay(500);                              // 3. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        servo.write(ServoRestPosition);    // 4. turns servo back to rest position. Modify initial position by modifying the 'ServoRestPosition' definition above.
        delay(500);                              // 5. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        servo.detach();                    // 6. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
        
        // The Piezo Speaker light will play a beep for half a second, wait for another 500ms(half a second) and then play a tune
        PiezoSpeaker.tone(400,500);                                                    // 1. plays a 400Hz beep. Change the value in the brackets (400) for a higher or lower beep.
        delay(500);                                                               // 4. waits 500ms                   
        PiezoSpeaker.playMelody(HoorayLength, HoorayMelody, HoorayNoteDurations);  // 5. plays the Hurray melody. to play a different melody, modify HoorayLength, HoorayMelody and HoorayNoteDuration above.                    
        delay(500);                                                               // 4. waits 500ms
    }
    
    
    //GET DWEETS  
    dweet.receiveDweetEvents();
    
    
    if(strcmp(dweet.getValue() , "servo") == 0)
    {
        Serial.println(F("servo was pressed!"));
        // The servo will rotate to target position and back to resting position with an interval of 500 milliseconds (0.5 seconds) 
        servo.attach(SERVOSM_PIN_SIG);         // 1. attach the servo to correct pin to control it.
        servo.write(ServoTargetPosition);  // 2. turns servo to target position. Modify target position by modifying the 'ServoTargetPosition' definition above.
        delay(500);                              // 3. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        servo.write(ServoRestPosition);    // 4. turns servo back to rest position. Modify initial position by modifying the 'ServoRestPosition' definition above.
        delay(500);                              // 5. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
        servo.detach();                    // 6. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
    }
    else if(strcmp(dweet.getValue() , "PiezoSpeaker") == 0)
    {
        Serial.println(F("PiezoSpeaker was pressed!"));
        // The Piezo Speaker light will play a beep for half a second, wait for another 500ms(half a second) and then play a tune
        PiezoSpeaker.tone(400,500);                                                    // 1. plays a 400Hz beep. Change the value in the brackets (400) for a higher or lower beep.
        delay(500);                                                               // 4. waits 500ms                   
        PiezoSpeaker.playMelody(HoorayLength, HoorayMelody, HoorayNoteDurations);  // 5. plays the Hurray melody. to play a different melody, modify HoorayLength, HoorayMelody and HoorayNoteDuration above.                    
        delay(500);                                                               // 4. waits 500ms
    }
    else if(strcmp(dweet.getValue() , "playGame") == 0)
    {
        Serial.println(F("Playing Game!"));
        while (!pir.read())
        {
          PiezoSpeaker.playMelody(comeLength, comeMelody, comeNoteDurations);
          delay(500);
        }
        
        servo.attach(SERVOSM_PIN_SIG);
        servo.write(ServoTargetPosition);
        delay(1000);
        servo.write(ServoRestPosition);
        delay(1000);
        servo.detach();
        
        PiezoSpeaker.playMelody(HoorayLength, HoorayMelody, HoorayNoteDurations);  // 5. plays the Hurray melody. to play a different melody, modify HoorayLength, HoorayMelody and HoorayNoteDuration above.                    
        
        delay(100);
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/
