/**
   References:
  https://www.arduino.cc/en/reference/SD
  https://www.arduino.cc/en/reference/SPI
  https://learn.sparkfun.com/tutorials/microsd-breakout-with-level-shifter-hookup-guide?_ga=2.102369153.1649077145.1614009245-446441880.1614009245
*/

// SD - Version: Latest
#include <SD.h>

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

String dataFileName="RobotLawnMower.log";
int lastSDCardError=0;  //latest error code on SD Card

//Constant
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Sparkfun SD shield: pin 8 on Arduino Uno, on Mega2560 is 53
// (SS) must be kept as an output or the SD library functions will not work.
const int chipSelect = 53; 


/**
 * Procedure to Init SD card to write log into
   Output= true OK, false KO
   Error Code
    992 - Card failed, or not present
    TODO va inserito la gestione degli eventi di allarme su cloud.
**/
boolean dataLogSDInit() {
   Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {  //Initializes the SD library and card. 
    Serial.println("Error 992 - Card failed, or not present");
    lastSDCardError=992;
    return false;
  }
  Serial.println("card initialized.");
  lastSDCardError=0;
  return true;
}



/**
 * Basic procedure to test the PCB SD Card Functionality.
   Output= 0 if OK
   Error=
    995 - SD Card initialization failed
    994 - Could not find FAT16/FAT32 partition
    Warning=
    193 - SD Card PCB not installed
    TODO va inserito la gestione degli eventi di allarme su cloud.
    TODO Check se la SD è piena o c'è spazio 
**/
int checkSDCardPCB() {
  // set up variables using the SD utility library functions
Sd2Card card;
SdVolume volume;
SdFile root;
  if (sdCard_ON) {
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
      Serial.println("Error 995");
      lastSDCardError = 995;
      return false;
    } else {
      if (logEnable) {
        Serial.print("Wiring is correct and a card is present.");
        Serial.print(" Card type:         ");
        switch (card.type()) {
          case SD_CARD_TYPE_SD1:
            Serial.println("SD1");
            break;
          case SD_CARD_TYPE_SD2:
            Serial.println("SD2");
            break;
          case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
          default:
            Serial.println("Unknown");
        }
      }
      // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
      if (!volume.init(card)) {
        Serial.println("Error 994");
        lastSDCardError=994;
        return false;
      } else {
        if (logEnable) {
          // print size of the first FAT-type volume
          uint32_t volumesize;
          Serial.print("Volume type is:    FAT");
          Serial.println(volume.fatType(), DEC);
          volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
          volumesize *= volume.clusterCount();       // we'll have a lot of clusters
          volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
          Serial.print("Volume size (Mb):  ");
          volumesize /= 1024;
          Serial.print(volumesize);
          Serial.print(" Volume size (Gb):  ");
          Serial.println((float)volumesize / 1024.0);
          Serial.println("\nFiles found on the card (name, date and size in bytes): ");
          root.openRoot(volume);
          // list all files in the card with date and size
          root.ls(LS_R | LS_DATE | LS_SIZE);
          root.close();
        }
        lastSDCardError=0;
        return true;
      }
    }
  } else {
    //SD Card not installed
    Serial.println("Warning 193");
    lastSDCardError=193;
    return true;
  }
}
