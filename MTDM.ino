#include <Adafruit_VL53L1X.h>
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <vl53l1x_class.h>
#include <vl53l1x_error_codes.h>


#define IRQ_PIN 2
#define XSHUT_PIN 5
#define Breakout_Reset 9
#define BREAKOUT_CS 10
#define BREAKOUT_DCS 8

#define SHIELD_RESET -1 //rset pin is unused
#define SHIELD_CS 7 //output pin
#define SHIELD_DCS 6 //data/command input pin

#define CARDCS 4
#define DREQ 3

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <stdio.h>



//char toPlay[11]; //string array for file to play

//makeName(randNumber,0); // generate file name in global array toPlay



Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


//create a shield object

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

void setup() {

  //*********MP3 Shield***********
  Serial.begin(9600);
  Serial.println("BOOGY WOOGY");

  //initialise the music player
  if( !musicPlayer.begin()) { 
    Serial.println(F("Couldn't find MP3 Shield, verify correct pins defined"));
    while (1);
  }


  //found MP3 shield
  Serial.println(F("MP3 FOUND"));

  if (!SD.begin(CARDCS)){
    Serial.println(F("NO SD"));
    while (1); //dont do anything
  }

// list the files
   printDirectory(SD.open("/MP3/"), 0);
// count the files in the directoryu


//set volume for left and right, lower number = Louder Volume
musicPlayer.setVolume(40,40);


  //*********TOF Sensor***********
  Serial.begin(9600); //set the frequency of the device
  while (!Serial) delay(10);

  Serial.println(F("Meme Theme Dream Machine"));

  Wire.begin();
  if (! vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on sensor init: ")); //in case the sensor doesnt initialize well
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }


  //statement for  things working as intended
  Serial.println(F("MTDM ON!"));
  
  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(),HEX);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't return a range"));
    Serial.println(vl53.vl_status);
    while (1) delay(10);
  }

  Serial.println(vl53.getTimingBudget());
  
  //optional set distance threshold
    /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */


}

void loop() {
  char MP3[15];
  File path = SD.open("/MP3/"); // root directory

  chooseRandomFile(path,MP3);6793;
  path.close();
  delay(200);
  
  //File results;
  //char* MP3 = selectRandomFileFrom( path, results);

  
  int16_t distance; //set distance variable

  if (vl53.dataReady()) {
    //get new measurements
    distance = vl53.distance(); //***very important variable, will be used for music if statement
    if (distance == -1) {
      //catch statement for bad measurements
      Serial.print(F("Couldn't get distance: "));
      Serial.println(vl53.vl_status);
      return;
    }
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(" mm");
    
    //Play music from distance measurement
    if (distance < 900){
      char fullName[20] = "/MP3/";
      strcat(fullName,MP3);
      Serial.print(fullName);
      Serial.println(F("Playing track: "));
      musicPlayer.playFullFile(fullName);
      delay(300000);
 
   /*   if (! musicPlayer.startPlayingFile("Track1")){
        Serial.println("Could not open file: ");
        Serial.println(MP3);
        delay(1000);
      }
    } */

    // time for another reading after data is read
    //vl53.clearInterrupt();
    }
  }
}


void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}


/*void makeName (int number, int depth){
  if(number > 0 ){
    makeName(number/10, ++depth);
    depth--;
    number = number %10;
  }
  toPlay[indexToWrite] = (number &0xf) |0x30;
  indexToWrite++;
  if (depth >0) return;
  else {
    toPlay[indexToWrite] = '.';
    toPlay[1+indexToWrite] = 'm';
    toPlay[2+indexToWrite] = 'p';
    toPlay[3+indexToWrite] = '3';
    toPlay[4+indexToWrite] = '\0'; //terminator
    indexToWrite = 0; //reset pointer for next time we enter
  }
} */
/*char* selectRandomFileFrom( File dir, File result) {
  File entry;
  int count = 0;

  dir.rewindDirectory();

  while (entry = dir.openNextFile() ) {
    if (random (count) == 0){
      result = entry;
    }
    entry.close();
    count ++;
  }
  return result.name();
}*/

/*void randomsong() {
  int i, rand_song;
  File folder = SD.open("/");
  File random_file;
  rand_song = (random(0, n_files)+1;
  Serial.print ("Random number: ");
  Serial.println(rand_song);
  folder.rewindDirectory();
  random_file = selectFileN(rand_song, folder);
  folder.close()
  if(!musicPlayer.stasrtPlayingFile(random_file.name()))
  {
    Serial.println("Could Not Open This File");
  }
  else Serial.println("Music Playing");
  random_file.close();
  Serial.print("playing Music = ");
  Serial.println(musicPlayer.playingMusic);
  delay (10000);
  musicplayer.stopPlaying;
  break
}*/

void chooseRandomFile( File dir, char* outFileName) {
  File entry, result;
  int count = 1;

  dir.rewindDirectory();

  while ( entry = dir.openNextFile() ) {
    if ( random( count ) == 0 ) {
      if (result ) {
        result.close();
      }
      result = entry;
    } else {
      entry.close();
    }
    count++;
  }
  strcpy(outFileName, result.name());
  result.close();
}
  
