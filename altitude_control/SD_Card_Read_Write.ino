/*
  SD card read/write
 This example shows how to read and write data to and from an SD card file   
 The circuit:
 */
 
#include <SD.h>
#include <SPI.h>
 
String dataString =""; // holds the data to be written to the SD card
float sensorReading1 = 1.00; // value read from your first sensor
float sensorReading2 = 0.00; // value read from your second sensor
float sensorReading3 = 0.00; // value read from your third sensor
float sensorReading4 = 0.00; // value read from your third sensor

File sensorData;

File myFile;
const int chipSelect= 15;
 
//const int chipSelect = BUILTIN_SDCARD; //for Teensy3.5


void setup(){
  Serial.begin(115200);
  sd_setup();
  
}


  

void loop()
{
  prepare_data();
  saveData(); // save to SD card
}




//////////////////////////////////////////////////////////////////////////////////////////////
//                              Functions                                                  ///
//////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
//SD Initialize and creates a file               ///
////////////////////////////////////////////////////
void sd_setup()
{
  
  Serial.print("Initializing SD card...");
  // IF FAILURE OCCURS: 
    if (!SD.begin(chipSelect)) {
      Serial.println("initialization failed!");
      return;
    }
  
  Serial.println("initialization done.");
  sensorData = SD.open("data.csv", FILE_WRITE);
  sensorData.close();
  //read_SD_Directory(); 
}

////////////////////////////////////////////////////
//Prepares the Data                              ///
////////////////////////////////////////////////////
void prepare_data(){
    // build the data string
  dataString = String(sensorReading1) + "," + String(sensorReading2) + "," + String(sensorReading3) +  "," + String(sensorReading4) +  "," ;
  Serial.println(dataString); //Only to show on the Serial Monitor.
  
}

////////////////////////////////////////////////////
//Saves the Data                                 ///
////////////////////////////////////////////////////
void saveData(){
    if(SD.exists("data.csv")){ // check the card is still there
      // now append new data file
      Serial.println("data.csv exists");
      sensorData = SD.open("data.csv", FILE_WRITE);
      if (sensorData){
      sensorData.println(dataString);
      sensorData.close(); // close the file
    }
  }
  else{
    Serial.println("Error writing to file !");
  }
}

////////////////////////////////////////////////////
//Reads the SD Directory                         ///
////////////////////////////////////////////////////
//void read_SD_Directory(){
//    // Read the contents of the SD card
//    File root = SD.open("/");
//    root.rewindDirectory();
//    listFiles(root);
//    root.close();
//  
//}

//////////////////////////////////////////////////////////
//Reads the files and prints onto Serial Monitors        ///
//////////////////////////////////////////////////////////
//String listFiles(File dir) 
//{
//  while(true) {
//     File entry = dir.openNextFile();
//     if (! entry) {
//      Serial.println("NO MORE FILES!");
//       // no more files
//       break;
//     }
//     // convert to string to make it easier to work with...
//     String entryName = (String)entry.name();
//     Serial.println("FOUND: " + entryName);
//     entry.close();
//     Serial.println("closed: ");
//   }
//}
