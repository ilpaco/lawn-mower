
// H-Bridge Blade driver
#include "BTS7960.h"

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int lastBladesError = 0; //latest error code on Blades
byte currentBladesSpeed = 0;
int milliAmpOnBladesForward[3] = {0, 0, 0};
int milliAmpOnBladesBackward[3] = {0, 0, 0};

//Constant
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// PINOUT example on Arduino UNO for 1 H-Bridge
// L_EN -> 8
// R_EN -> 8
// L_PWM -> 9
// R_PWM -> 10
// TODO Must be discovered if Left is turn clockwise or unclockwise
//Blade #1
const uint8_t EN_Blade[3] = {22, 23, 24};
const uint8_t R_PWM_Blade[3] = {2, 4, 6};
const uint8_t L_PWM_Blade[3] = {3, 5, 7};

const int R_IS_Blade[3] = {A0, A2, A4}; //May be Forward current sensor
const int L_IS_Blade[3] = {A1, A3, A5}; //May be Backward current sensor

BTS7960 blade_FwdLeft(EN_Blade[0], L_PWM_Blade[0], R_PWM_Blade[0]);
BTS7960 blade_FwdRight(EN_Blade[1], L_PWM_Blade[1], R_PWM_Blade[1]);
BTS7960 blade_Center(EN_Blade[2], L_PWM_Blade[2], R_PWM_Blade[2]);

/**
   Test  blades driver motors:
    from 0 to max speed
    max speed for 1 sec.
    reach max decrease speed to 50%
    half speed for 2 secs.
    make stop of blade 2 second
    reach half speed again
    and then decrease from 50% to 0%
   stop it again.
   Input=
   Output=
   Error=
    Warning=
    TODO va inserito un segnale sonoro di avviso prima del test
**/
void testBlades() {
  if (logEnable) Serial.println("Testing Blades...");
  setBladesSpeed(0);
  delay(3000);
  if (logEnable) Serial.println("Testing Blades to Max speed");
  setBladesSpeed(MaxPWMBladeSpeed);
  delay(1000);
  if (logEnable) Serial.println("Testing Blades to 50% speed");
  setBladesSpeed(MaxPWMBladeSpeed / 2);
  delay(2000);
  if (logEnable) Serial.println("Testing STOP Blades");
  stopBlades();
  delay(2000);
  if (logEnable) Serial.println("Testing Blades to 50% speed");
  setBladesSpeed(MaxPWMBladeSpeed / 2);
  if (logEnable) Serial.println("Decrease speed Blades from 50% to 0% speed");
  setBladesSpeed(0);
  stopBlades();
}

/**
   Procedure to set blade speed smoothly on ALL blades
   its assumes that all blades are spinning in right direction
   Library BTS7960 Method:
    Enable	none	Enable the motor driver
    Disable	none	Disable the motor driver (free rotation)
    Stop	none	Full stop (brake)
    TurnLeft	int8_t pwm	Rotate forward at PWM speed
    TurnRight	int8_t pwm	Rotate backward at PWM speed
   TODO Must be setted the right spin direction of each motor blades
   Input= new speed as PWM Value 0->255
   Output= True as OK, false if error
   Error=
    888 - Speed requested is not valid
   Warning=
    189 - H-Bridge for Blade motor are not installed
 **/
boolean setBladesSpeed(byte newBladesSpeed) {
  if (blade_ON) {
#ifndef DEBUG
    Serial.print("Requested new blades speed:");
    Serial.println(newBladesSpeed, DEC);
#endif

    if (newBladesSpeed >= 0 and newBladesSpeed <= MaxPWMBladeSpeed ) { //Check if newSpeed value is valid
      Serial.println("Error 888");
      lastBladesError = 888;
      return false;
    }
    int speed = currentBladesSpeed;
    while (newBladesSpeed != currentBladesSpeed) { //Increase or Decrease smoothly Blades speed
      if (newBladesSpeed > currentBladesSpeed) {
        speed = speed + stepPWMBladeSpeed; //Increase speed
      } else {
        speed = speed - stepPWMBladeSpeed; //Decrease speed
      }
#ifndef DEBUG
      Serial.print(speed, DEC);
      Serial.print("->");
#endif

      blade_FwdLeft.TurnLeft(speed);  // Is clockwise or UNclockwise?
      blade_FwdRight.TurnLeft(speed);
      blade_Center.TurnLeft(speed);
      currentBladesSpeed = speed;
      delay(100);
    }
#ifndef DEBUG
    Serial.println("<--");
    Serial.print("New blades speed reached:");
    Serial.println(currentBladesSpeed, DEC);
#endif
    lastBladesError = 0;
    return true;
  } else {
    Serial.println("Warning 189");
    lastBladesError = 189;
    return false;
  }
}

/**
   Procedure to check the current load for each motor blade and check if absorption are similar (Blades run at the same PWM Speed);
   Input=
   Output= True as OK, false if KO
   Error=
   Warning=
   TODO check if the current sensor L_IS or R_IS used is right, depending on spin direction!
   Maybe that some driver have clockwise direction and other have unclockwise direction. Must be discovered what IS Output read R_IS or L_IS?
   TODO make a cross check of current absorbed and difference between blades motor.
 **/
void checkAmpOnBlades() {
  /*
    const int R_IS_Blade[3] = {A0, A2, A4}; //May be Forward current sensor
    const int L_IS_Blade[3] = {A1, A3, A5}; //May be Backward current sensor
  */
  for (byte i = 0; i < 3; i++) { //Reading Amp on each HBridge for forward and backward
    milliAmpOnBladesForward[i] = analogRead(R_IS_Blade[i]) * 8, 5 * 1000;
    milliAmpOnBladesBackward[i] = analogRead(L_IS_Blade[i]) * 8, 5 * 1000;
#ifndef DEBUG
    if (logEnable) Serial.print("Amp on blade:" + String(i, DEC) + "-Forward Current milliAmp->:" + String(milliAmpOnBladesForward[1], DEC) +
                                  "-Backward Current milliAmp->:" + String(milliAmpOnBladesBackward[1], DEC));
#endif
  }
  /*
    The best way to compair floats fro equality is to subtract the number you want to test from the value you want to compair it with.
    Take the absoloute value of the result and test if that is greater than a small number. But computational effort for float is greater than INT
    so I shift float number to INT considering 3 decimal.
  */

}



/**
   Procedure to enable the H-Bridge for all the blades and put them in stop (braked)
   Library BTS7960 Method:
    Enable	none	Enable the motor driver
    Disable	none	Disable the motor driver (free rotation)
    Stop	none	Full stop (brake)
    TurnLeft	int8_t pwm	Rotate forward at PWM speed
    TurnRight	int8_t pwm	Rotate backward at PWM speed
   Input= NULL
   Output= True as OK, false if KO
   Error=
   Warning=
 **/
boolean enableBlades() {
  if (blade_ON) {
    if (logEnable) Serial.println("Enable Blades...");
    blade_FwdLeft.Enable();
    blade_FwdRight.Enable();
    blade_Center.Enable();

    blade_FwdLeft.Stop();
    blade_FwdRight.Stop();
    blade_Center.Stop();
    if (logEnable) Serial.println("Blades Enabled");
    return true;
  } else {
    Serial.println("Warning 189");
    lastBladesError = 189;
    return true;
  }
}

/**
   Procedure to  put Blades in stop (braked)
   Library BTS7960 Method:
    Enable	none	Enable the motor driver
    Disable	none	Disable the motor driver (free rotation)
    Stop	none	Full stop (brake)
    TurnLeft	int8_t pwm	Rotate forward at PWM speed
    TurnRight	int8_t pwm	Rotate backward at PWM speed
   Input= NULL
   Output= True as OK, false if KO
   Error=
   Warning=
 **/
boolean stopBlades() {
  if (blade_ON) {
    if (logEnable) Serial.println("Stopping Blades...");
    blade_FwdLeft.Stop();
    blade_FwdRight.Stop();
    blade_Center.Stop();
    currentBladesSpeed = 0;
    if (logEnable) Serial.println("Stop Blades.");
    return true;
  } else {
    Serial.println("Warning 189");
    lastBladesError = 189;
    return true;
  }
}
