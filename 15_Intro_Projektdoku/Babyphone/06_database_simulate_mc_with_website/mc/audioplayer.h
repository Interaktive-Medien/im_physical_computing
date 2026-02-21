/******************************************************************************************************
 * audioplayer.h
 * this code is included into mc.ino
 * keep mc.ino clean and hide the audio player functionality in this file
 ******************************************************************************************************/



// we use HardwareSerial 1 (Schnittstelle 1)
HardwareSerial myMP3(1);
#define RX_PIN 7
#define TX_PIN 6
#define MP3_GND_CONTROL_PIN 5
#define PLAYER_BAUD 9600


// command definitions (Open-Smart)
#define PLAY_W_INDEX   0x41
#define SET_VOLUME     0x31
#define SEL_DEV        0x35
#define DEV_TF         0x01





void controlAudioPlayer(int8_t command, int16_t dat) {
  uint8_t sendBuf[6];
  sendBuf[0] = 0x7e;                // Start byte
  sendBuf[1] = 0x04;                // number off following bytes
  sendBuf[2] = command;             // command
  sendBuf[3] = (int8_t)(dat >> 8);  // data digh
  sendBuf[4] = (int8_t)(dat);       // data low
  sendBuf[5] = 0xef;                // end byte
  
  for(uint8_t i=0; i < 6; i++) {
    myMP3.write(sendBuf[i]);
  }
  delay(100); // short break after each break for more stability
}


void initAudioPlayer(){
  pinMode(MP3_GND_CONTROL_PIN, OUTPUT);

  // initialise ESP32-C6 hardware serial: RX=7, TX=6, Baud=9600
  myMP3.begin(PLAYER_BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(1000); 
  Serial.println("audio player starting...");

  // select SD card
  controlAudioPlayer(SEL_DEV, DEV_TF);
  delay(200);

  // audio volume (range 0-30)
  controlAudioPlayer(SET_VOLUME, 30);
  delay(200);

  // short interrupt of the power to the audio player, otherwise we cannot hear the sound
  digitalWrite(MP3_GND_CONTROL_PIN, 0); // transistor locks -> GND interrupted
  delay(200);
  digitalWrite(MP3_GND_CONTROL_PIN, 1); // transistor not locked any more -> GND ocnnected

}

void playTrack(int i){
  controlAudioPlayer(PLAY_W_INDEX, i);
}