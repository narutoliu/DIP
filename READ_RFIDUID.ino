/*RFID READER Code by group 6
 *BACKUP: https://github.com/narutoliu/DIP
 *Last editted 10/12/18 
 */



#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "RedMP3.h"
#include <Wire.h>

#define MP3_RX 7
#define MP3_TX 8
MP3 mp3(MP3_RX, MP3_TX);

int8_t index  = 0x01;//the first song in the SD card
int8_t volume = 0x1c;//Volum ctronol

#define SS_PIN 10
#define RS_PIN 9
#define buttonPIN 1

int x = 0;

bool state = 0;
//char incomingByte;

int lng = 100;  //long delay
int sht = 50;   //short delay


const int sampleWindow = 200; // Sample window width in mS
unsigned int sample;

MFRC522 rfid(SS_PIN, RS_PIN);

void setup() {

  // Start master
  Wire.begin();
  
  SPI.begin();
  Serial.begin(9600);
  rfid.PCD_Init();
  digitalWrite(SS_PIN, LOW);

  delay(500);//500ms to initialize
  mp3.playWithVolume(index, volume); //play sound
  delay(50);//wait before cmmad
}

void loop() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 200 mS
  while (millis() - startMillis < sampleWindow)
  {
    
    sample = analogRead(0);
    if (sample < 1024)  // toss out out of range data
    {

      if (sample > signalMax)
      {

        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max-min = peak-peak amplitude a.k.a deviation
  double multiplier = 3.3;

  
  if (peakToPeak < 512 ) {   //control multiplier for level
    multiplier = 3.3;
  } else {
    multiplier = 6.6;       //increase multiplier to get more drastic changes
  }

  
  double final_Amp = (peakToPeak * multiplier) / 1024; 
  int sound = (final_Amp * 10);

  int soundLevel = map(sound, 1, 10, 0, 11);  //map sound to 12 levels
  Serial.print("the sound Level is:");
  Serial.println(soundLevel);


  Wire.beginTransmission(9); // transmit to device #9

  Wire.write(soundLevel);


  Wire.endTransmission();    // stop transmitting



  
  //RFID reader starts here
  int is[10] = {0};
  int it = 0;

  for (int i = 0; i <= 9; i++) {                //detect new cards
    is[i] = rfid.PICC_IsNewCardPresent();
    if (is[i] == 1) {
      rfid.PICC_ReadCardSerial();
    }
    delay(10);
  }

  for (int i = 0; i <= 9; i++) {
    it += is[i];
  }
  //////////////////////////////////////////////////////////////////////////////////
  while (it) {                                       //process and play songs

    Serial.println("while it");

    rfid.PICC_ReadCardSerial();

    unsigned long UID_unsigned;
    UID_unsigned =  rfid.uid.uidByte[0] << 24;
    UID_unsigned += rfid.uid.uidByte[1] << 16;
    UID_unsigned += rfid.uid.uidByte[2] << 8;
    UID_unsigned += rfid.uid.uidByte[3];
    //Serial.println(UID_unsigned);
    Serial.println(UID_unsigned);
    if (UID_unsigned == 4294960036) { //Havanana

      index  = 0x02;
      //state = 0;

      if (!state) {
        delay(500);//Requires 500ms to wait for the MP3 module to initialize
        mp3.playWithVolume(index, volume);
        delay(50);
        Serial.print("CARD 1");
        state = 1;
      }
    }

    if (UID_unsigned == 4294954774) {
      Serial.flush();
      //index  = 0x03;
      if (!state) {
        delay(500);//Requires 500ms to wait for the MP3 module to initialize
        mp3.playWithVolume(0x03, volume);
        delay(50);
        Serial.print("CARD 2");
        state = 1;
        rfid.PICC_ReadCardSerial();
      }
    }

    if (UID_unsigned == 4294958614) {
      Serial.flush();
      // index  = 0x04;
      if (!state) {
        delay(500);//Requires 500ms to wait for the MP3 module to initialize
        mp3.playWithVolume(0x04, volume);
        delay(50);
        Serial.print("CARD 3");
        rfid.PICC_ReadCardSerial();
        state = 1;
      }
    }

    if (UID_unsigned == 4294954006) {
      Serial.flush();
      //index  = 0x05;
      if (!state) {
        delay(500);//Requires 500ms to wait for the MP3 module to initialize
        mp3.playWithVolume(0x05, volume);
        delay(50);
        Serial.print("CARD 4");
        state = 1;
        rfid.PICC_ReadCardSerial();
      }
    }

    if (UID_unsigned == 4294935460) {//bumblebee
      Serial.flush();
    
      if (!state) {
        delay(500);//Requires 500ms to wait for the MP3 module to initialize
        mp3.playWithVolume(0x05, volume);
        delay(50);
        Serial.print("CARD 5");
        rfid.PICC_ReadCardSerial();
        state = 1;
      }
    }


    it = 0;

    for (int i = 0; i <= 9; i++) {
      is[i] = rfid.PICC_IsNewCardPresent();
      delay(10);
    }
    for (int i = 0; i <= 9; i++) {
      it += is[i];
    }
  }



  state = 0;
}

//while loop
