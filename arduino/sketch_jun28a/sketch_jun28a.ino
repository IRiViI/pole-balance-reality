
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
#include <AccelStepper.h>

int poleEncoderSteps = 90;
int messageLength = 3;
char message[3] = "";
int bytesIndex = 0;

unsigned long current_time;

unsigned long old_message_time;
unsigned long message_period_time = 1000;

unsigned long update_time = millis();
unsigned long update_period_time = 1000;

unsigned long last_step_time = millis();

unsigned long last_jerk_time = millis();
unsigned long delta_jerk_time = 10;

unsigned long last_acceleration_time = millis();
unsigned long delta_acceleration_time = 10;

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder poleEncoder(A0, A1);

// Stepper motor
int dir_pin = 8;
int step_pin = 9;
int sleep_pin = 7;
int reset_pin = 4;
int motorAccel = 0;
long stepperMaxFrequency = 5000;

// bounaries
int boundary_left_pin = 10;
int boundary_right_pin = 11;
bool boundary_left_value = false;
bool boundary_right_value = false;

AccelStepper stepperMotor(1, step_pin, dir_pin);

bool stepperGoRight = true;
long stepperFrequency = 0;
long stepperFrequencyGoal = 0;
bool jerkMode = false;
int jerk = 0;
bool calibrating = true;
int calibration_stage = 'right';
int max_steps = 0;

void setup()
{
  
  pinMode(sleep_pin, OUTPUT); 
  digitalWrite(sleep_pin, LOW);
  
  pinMode(reset_pin, OUTPUT);
  digitalWrite(reset_pin, HIGH);
  
  Serial.begin(9600);
  stepperMotor.setMaxSpeed(stepperMaxFrequency);
  stepperMotor.setSpeed(stepperFrequency);
  stepperMotor.setAcceleration(100);
  
  pinMode(boundary_left_pin, INPUT); 
  pinMode(boundary_right_pin, INPUT); 

//  pinMode(dir_pin, OUTPUT);   
//  digitalWrite(dir_pin, LOW);
//  
//  pinMode(step_pin, OUTPUT);   
////  digitalWrite(step_pin, LOW);
//  analogWrite(step_pin, 128);
//  setPwmFrequency(step_pin, 100000);

} // setup()

void calibration()
{

  //  Boundaries
  boundary_left_value = digitalRead(boundary_left_pin);
  boundary_right_value = digitalRead(boundary_right_pin);

  // Calibration 
  if (calibrating == true){
    digitalWrite(sleep_pin, HIGH);
    if (calibration_stage == 'right'){
      if (boundary_right_value){
        stepperMotor.setSpeed(-600);
        stepperMotor.runSpeed();
        return;
      } else {
        calibration_stage = 'left';
        stepperMotor.setCurrentPosition(0);
      }
    }
    
    if (calibration_stage == 'left'){
        if (boundary_left_value){
          stepperMotor.setSpeed(600);
          stepperMotor.runSpeed();
          return;
        } else {
        calibration_stage = 'reset';
          max_steps = stepperMotor.currentPosition();
        }
    }
    
    if (calibration_stage == 'reset'){
        if (stepperMotor.currentPosition() > max_steps/2){
          stepperMotor.setSpeed(-600);
          stepperMotor.runSpeed();
          return;
        } else {
          calibrating = false;
        }
    }
    return;
  }
}

// Read the current position of the encoder and print out when changed.
void loop()
{


  // Calibration 
  if (calibrating == true){
    calibration();
    return;
  }

  //  Boundaries
  boundary_left_value = digitalRead(boundary_left_pin);
  boundary_right_value = digitalRead(boundary_right_pin);
  
  static int actualPolePos = 0;
  current_time = millis();
  
  //  Handling messages
  if (Serial.available() > 0) {
          if (current_time - old_message_time > message_period_time){
            bytesIndex = 0;
          }
          old_message_time = current_time;
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
            }
            // Motor
            if (message[0] == 'm'){
              // Change direction              
              if (message[1] == 'd'){
                stepperGoRight = !stepperGoRight;
              }
              // Break              
              if (message[1] == 'b'){
                stepperFrequency = 0;
                stepperFrequencyGoal = 0;
                motorAccel = 0;
                jerk = 0;
              }
              // Set frequency
              if (message[1] == 'f'){
                jerkMode = false;
                // convert the value (char value 0 to 127) to an interger
                stepperFrequency = String(int(message[2])).toInt() * stepperMaxFrequency /127;
              }
              // Set speed
              if (message[1] == 's'){
                jerkMode = false;
                // convert the value (char value 0 to 127) to an interger
                stepperFrequency = ((String(int(message[2])).toInt()) -63) * stepperMaxFrequency /126 * 2;
              }
              // Set jerk      
              if (message[1] == 'j'){
                // convert the value (char value 0 to 127) to an interger
                jerkMode = true;
                jerk = ((String(int(message[2])).toInt()) -63);
                motorAccel += jerk;
              }
              // Update motor
            }
          }
  }
  
  // Pole
  poleEncoder.tick();
  static int polePos = 0;
  int deltaPolePos = poleEncoder.getPosition() - polePos;
  if (deltaPolePos != 0) {
    actualPolePos = actualPolePos + deltaPolePos;
    if (actualPolePos > poleEncoderSteps - 1)
    {
      actualPolePos = 0;
    }
    if ( actualPolePos < 0){
      actualPolePos = poleEncoderSteps - 1;
    }
    polePos = polePos + deltaPolePos;
  } 


  // Stepper motor
//  int max_acc = 100;
//  int deltaFrequency = stepperFrequencyGoal - stepperFrequency;
//  if (deltaFrequency>0){
//    deltaFrequency = max_acc;
//    stepperFrequency += 1;
//  }
//  if (deltaFrequency < 0){
//    deltaFrequency = max_acc;
//    stepperFrequency -= 1;
//  }
//  stepperFrequency += deltaFrequency;
//  stepperFrequency = stepperFrequencyGoal;
  if (jerkMode == true){
    if (current_time - last_jerk_time > delta_jerk_time){
//      motorAccel += jerk;
      last_jerk_time = current_time;
    }
    if (current_time - last_acceleration_time > delta_acceleration_time){
      stepperFrequency += motorAccel;
      stepperFrequencyGoal = stepperFrequency;
      last_acceleration_time = current_time;
    }
  }
  bool resetting = false;
  if (boundary_right_value and boundary_left_value){
//    stepperFrequency/
    resetting = false;
  }
  else if(boundary_right_value){
    if (stepperFrequency > 0){
      stepperFrequency = 0;
      resetting = true;
    }
  }
  else if(boundary_left_value){
    if (stepperFrequency < 0){
      stepperFrequency = 0;
      resetting = true;
    }
    
  } else {
    stepperFrequency = 0;
    resetting = true;
  }
  
  if (stepperFrequency != 0){
    digitalWrite(sleep_pin, HIGH);
  }
  stepperMotor.setSpeed(stepperFrequency);
  stepperMotor.runSpeed();

  if (stepperFrequency == 0){
//    digitalWrite(sleep_pin, LOW);
  } 
  
  if (resetting){
    stepperFrequency = 0;
    stepperFrequencyGoal = 0;
    motorAccel = 0;
    jerk = 0;
    bool resetting = false;
  }
//  if (current_time - last_step_time > 0.01){
//    digitalWrite(step_pin, !digitalRead(step_pin));
//    last_step_time = current_time;
//  }   

  // Send status
  if(current_time - update_time > update_period_time){
    update_time = current_time;
    Serial.println("[" + 
      String(actualPolePos * (360 / poleEncoderSteps)) + "," + 
      String(stepperMotor.currentPosition ()) + "," +
      String(stepperFrequency) + "," + 
      String(motorAccel) + "," + 
      String(jerk) + "," + 
      String(boundary_left_value) + "," + 
      String(boundary_right_value)  + "," + 
      String(max_steps) + "]" );
  }
  
  
  // if
} // loop ()

