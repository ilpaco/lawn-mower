


/**
 * Main Lawn_Mower
**/
/**
 * 
 * #ifndef DEBUG;
 * #else
 * #endif
**/
#define DEBUG; //enable debug mode

// volatiles are subject to modification by IRQs
/** 
 * volatile unsigned long raintime;
  volatile unsigned long rainlast;
  volatile unsigned long raininterval;
  volatile float rainCurrentHour[60]; // mm di piogga nell'ora corrente durante ciclo di interupt, spezzato in 60 minuti. 
**/
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


void setup() {
  Serial.begin(9600);
  int serRetry = 0;
  while (!Serial || serRetry < 5) { // wait for serial
    serRetry++;
    delay(500); //riciclo 5 volte con 500ms di attesa dopo di che forza andare avanti
  }
  delay(1500);

  Serial.println("Robot startup...");

//Below all the healty check of sub components PCB


  Serial.println("Robot online!");
}

void loop() {
  Serial.println("Robot is running...");


}
