
// -----
// SimplePollRotator.ino - Example for the RotaryEncoder library.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// -----

// This example checks the state of the rotary encoder in the loop() function.
// The current position is printed on output when changed.

// Hardware setup:
// Attach a rotary encoder with output pins to A2 and A3.
// The common contact should be attached to ground.

#include <RotaryEncoder.h>

int poleEncoderSteps = 90;
int messageLength = 3;
char message[3] = "";
int bytesIndex = 0;

unsigned long old_message_time;
unsigned long new_message_time;
unsigned long message_period_time = 1000;

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A0, A2);

void setup()
{
  Serial.begin(9600);
//  Serial.println("SimplePollRotator example for the RotaryEncoder library.");
} // setup()


// Read the current position of the encoder and print out when changed.
void loop()
{
  
  static int actualPolePos = 0;
  
  //  Handling messages
  if (Serial.available() > 0) {
          new_message_time = millis();
          if (new_message_time - old_message_time > message_period_time){
            bytesIndex = 0;
          }
          old_message_time = new_message_time;
          // read the incoming byte:
          char incomingByte = Serial.read();
          message[bytesIndex] = incomingByte;
          bytesIndex++;

          // Check if message was complete         
          if (bytesIndex > 2){
            bytesIndex = 0;
            
//            Serial.println(message[0]);
            if (message[0] == 'p'){
              if (message[1] == '0'){
                actualPolePos = 0;
              }
//              Serial.println("pee");
            }
            if (message[0] == 'm'){
              Serial.println("emm");
            }
          }
  }
  // Pole
  encoder.tick();
  static int polePos = 0;
  int deltaPolePos = encoder.getPosition() - polePos;
  if (deltaPolePos != 0) {
    actualPolePos = actualPolePos + deltaPolePos;
    if (actualPolePos > poleEncoderSteps - 1)
    {
      actualPolePos = 0;
    }
    if ( actualPolePos < 0){
      actualPolePos = poleEncoderSteps - 1;
    }
    Serial.println("[" + String(actualPolePos * (360 / poleEncoderSteps)) + "]" );
//    Serial.println();
    polePos = polePos + deltaPolePos;
  } // if
} // loop ()

// The End
