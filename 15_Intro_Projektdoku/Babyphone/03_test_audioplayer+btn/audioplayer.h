// Wir nutzen HardwareSerial 1 (Schnittstelle 1)
// Verkabelung: ESP Pin 6 (TX) -> MP3 RX | ESP Pin 7 (RX) -> MP3 TX
HardwareSerial myMP3(1);
#define RX_PIN 7
#define TX_PIN 6
#define MP3_GND_CONTROL_PIN 5
#define PLAYER_BAUD 9600


// Befehls-Definitionen laut Datenblatt (Open-Smart)
#define PLAY_W_INDEX   0x41
#define SET_VOLUME     0x31
#define SEL_DEV        0x35
#define DEV_TF         0x01





void controlAudioPlayer(int8_t command, int16_t dat) {
  uint8_t sendBuf[6];
  sendBuf[0] = 0x7e;                // Startbyte
  sendBuf[1] = 0x04;                // Anzahl der folgenden Bytes
  sendBuf[2] = command;             // Befehl
  sendBuf[3] = (int8_t)(dat >> 8);  // Data High
  sendBuf[4] = (int8_t)(dat);       // Data Low
  sendBuf[5] = 0xef;                // Endbyte
  
  for(uint8_t i=0; i < 6; i++) {
    myMP3.write(sendBuf[i]);
  }
  delay(100); // Kurze Pause nach jedem Befehl für die Stabilität
}


void initAudioPlayer(){
  pinMode(MP3_GND_CONTROL_PIN, OUTPUT);


  // ESP32-C6 Hardware Serial initialisieren
  // Pins: RX=7, TX=6, Baud=9600
  myMP3.begin(PLAYER_BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  
  delay(1000); 
  Serial.println("audio player starting...");

  // SD-Karte auswählen
  controlAudioPlayer(SEL_DEV, DEV_TF);
  delay(200);

  // Lautstärke (Bereich 0-30)
  controlAudioPlayer(SET_VOLUME, 30);
  delay(200);

  // Strom zum Audioplayer kurz kappen, weil Ton sonst nicht zu hören ist
  digitalWrite(MP3_GND_CONTROL_PIN, 0); // Transistor sperrt -> GND unterbrochen
  delay(200);
  digitalWrite(MP3_GND_CONTROL_PIN, 1); // Transistor leitet -> GND verbunden

}

void playTrack(int i){
  controlAudioPlayer(PLAY_W_INDEX, i);
}