#include <ESC.h>

/**
   Pin di controllo su Mega 7,8,9
*/

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int lastBladesError = 0;

// 3 Electronic speed controller, for each blade.
ESC bladeFwdLeft(7, 1000, 2000, 500);
ESC bladeFwdRight(8, 1000, 2000, 500);
ESC bladeFwdCenterRear(9, 1000, 2000, 500);


/**
   It's important to calibrate the ESC with the values (uS / microseconds) he will expect to be for Min and Max speed.
   This one require a little procedure:
      1 - Load the sketch to your Arduino board
      2 - Connect your ESC to the Arduino board
      3 - Power your Arduino board
      4 - Once the LED (LED_PIN) is HIGH/ON connect the power to your ESC, you have 5sec to do so
      5 - Once the LED is LOW/OFF the calibration should be done
      6 - Should now be calibrated between 1000us and 2000us
   Input=
   Output=
   Error=
   Warning= 101 ESC not installed
   TODO Occorre rendere automatica la calibrazione del'ESC
**/
void ESCBladeCalibration() {
  if (blade_ON) {
    Serial.println("Starting ESC calibration for blades...");
    //pinMode(LED_PIN, OUTPUT);       // LED Visual Output (can be removed)
    //digitalWrite(LED_PIN, HIGH);    // LED High while signal is High (can be removed)
    bladeFwdLeft.calib();                  // Calibration of the Max and Min value the ESC is expecting
    bladeFwdLeft.stop();                   // Stop the ESC to avoid damage or injuries
    //digitalWrite(LED_PIN, LOW);     // LED Low when the calibration is done (can be removed)
    Serial.println("ESC calibration for blades DONE");
  } else {
    //ESC not installed
    Serial.println("Warning 101");
    lastBladesError = 101;
  }
}
