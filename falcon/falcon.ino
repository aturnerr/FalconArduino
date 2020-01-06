// Lights & Sound for UCS Millenium Falcon 75192
// Written by Adam Turner

// execute_CMD() function sourced from http://educ8s.tv/arduino-mp3-player/

#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"
#include "FastLED.h"

SoftwareSerial softwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini dfPlayer;

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 // returns info with command 0x41 [0x01: info, 0x00: no info]

#define VOLUME 28
#define NUM_LEDS 25
#define DATA_PIN 6
#define ACTIVATED LOW

// button and LED pins
int buttonNext = 3;
int buttonPlay = 4;
int blasterPin = 13;

// misc constants
int soundIndex = 0;
int maxTrackNo = 2;
boolean isPlaying = false;

CRGB leds[NUM_LEDS];

void setup () {
  softwareSerial.begin(9600);
  Serial.begin(115200);

  // set pin mode
  pinMode(buttonNext, INPUT);
  digitalWrite(buttonNext,HIGH);
  pinMode(buttonPlay, INPUT);
  digitalWrite(buttonPlay,HIGH);
  pinMode(blasterPin, OUTPUT);

  // initialise LEDs
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // set LEDs to OFF (black)
  fill_solid(leds,25,CRGB::Black);
  FastLED.show();

  delay(500);
  
  // initialise DFPlayer
  execute_CMD(0x3F, 0, 0);
  delay(100);
  // set volume 
  execute_CMD(0x06, 0, VOLUME);
  delay(100);
  // specify initial track number
  execute_CMD(0x03, 0, 0001);
  delay(100);
  // disable repeat play
  // execute_CMD(0x11, 0, 0);
  delay(100);

  // begin track playback
  execute_CMD(0x0D,0,1);

  delay(500);
  
  engineStart();
}

void loop () {
  // play start up sequence on boot
  if(!isPlaying && digitalRead(buttonPlay) == ACTIVATED) {
    engineStart();
  }

  // next track
  if(!isPlaying && digitalRead(buttonNext) == ACTIVATED) {
    // if we haven't reached the end of the track count
    if (soundIndex < maxTrackNo) {
      soundIndex ++;
      isPlaying = true;
      
      // play next track
      execute_CMD(0x01, 0, 0);

      // case specific behaviour for tracks
      if (soundIndex == 1) {
        esbTrack();
      } else if (soundIndex == 2) {
        tljTrack();
      }
    } else {
      // play the start up sequence again if we have gone through all tracks
      execute_CMD(0x01, 0, 0);
      soundIndex = 0;
      engineStart();
    }
  }
}

void blasterFire () {
  digitalWrite(blasterPin, HIGH);
  delay(50);
  digitalWrite(blasterPin, LOW);
  delay(50);
  digitalWrite(blasterPin, HIGH);
  delay(50);
  digitalWrite(blasterPin, LOW);
  return;
}

void esbTrack () {
  int i,k;
  delay(7500);
  for(i=0;i<8;i++) {
    for(k=255;k>150;k--){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
    }
    for(k=150;k<255;k++){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
    }
  }
  delay(5500);
  execute_CMD(0x0E,0,0);
  isPlaying = false;
  return;
}

void tljTrack () {
  int i;
  delay(7100);
  
  for(i=0;i<5;i++) {
    delay(150);
    blasterFire();
  }
  
  delay(340);

  for(i=0;i<2;i++) {
    delay(150);
    blasterFire();;
  }
  
  delay(3650);
  
  blasterFire();
  
  delay(1050);
  
  for(i=0;i<2;i++) {
    delay(300);
    blasterFire();
  }

  delay(1700);
  
  blasterFire();

  delay(200);
  
  for(i=0;i<2;i++) {
    delay(150);
    blasterFire();
  }
  
  execute_CMD(0x0E,0,0);
  isPlaying = false;
  return;
}

void engineStart () {
  // turn off LEDs
  fill_solid(leds,25,CRGB::Black);
  FastLED.show();
  
  isPlaying = true;
  
  int i,k;
  // LED behaviour timed to sync with track audio
  delay(6600);
  // the initial dimly lit light
  for(i=0;i<60;i++) {
    fill_solid(leds,25,CHSV(150,120,i));
    FastLED.show();
    delay(2);
  }
  delay(2600);
  // brightening the right from edge to center (LED 25 to 11)
  for(i=25;i>11;i--) {
    leds[i].setHSV(150,120,120);
    FastLED.show();
    delay(50);
  }
  delay(400);
  // then the remaining from the left
  for(i=0;i<13;i++) {
    leds[i].setHSV(150,120,120);
    FastLED.show();
    delay(50);
  }
  delay(1000);
  // slow pulsing
  for(i=0;i<5;i++) {
    for(k=120;k<160;k++){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
      delay(5);
    }
    for(k=160;k>120;k--){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
      delay(5);
    }
  }
  // faster pulsing
  for(i=0;i<10;i++) {
    for(k=120;k<160;k++){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
      delay(2);
    }
    for(k=160;k>120;k--){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
      delay(2);
    }
  }
  // faster with no delay
  for(i=0;i<8;i++) {
    for(k=120;k<160;k++){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
    }
    for(k=160;k>120;k--){
      fill_solid(leds,25,CHSV(150,120,k));
      FastLED.show();
    }
  }
  // fully lit
  fill_solid(leds,25,CHSV(150,120,255));
  FastLED.show();
  // let track play out then pause the player
  delay(13200);
  isPlaying = false;
  execute_CMD(0x0E,0,0);
  return;
}

// build and send command
void execute_CMD(byte CMD, byte Par1, byte Par2) {
  
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  
  // Send the command line to the module
  for (byte k=0; k<10; k++)
  {
    softwareSerial.write( Command_line[k]);
  }
}
