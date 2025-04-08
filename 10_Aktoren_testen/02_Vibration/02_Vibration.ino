/*******************************************
 * 02_Vibration.ino (Code gleich wie 01_LED.ino)
 * Empfange entweder eine 1 oder eine 0 am seriellen Port
 * und schalte die LED an GPIO8 entsprechend.
 * Verbinde:
 * Vibrationsmotor: + (langes Bein)  <->  ESP32-C6: GPIO 8 (per Vorwiederstand)
 * Vibrationsmotor: - (kurzes Bein)  <->  ESP32-C6: GND 
 * GitHub: https://github.com/Interaktive-Medien/im_physical_computing/blob/main/10_Aktoren_testen/02_Vibration/02_Vibration.ino
 *******************************************/

const int ledPin = 8;  

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  digitalWrite(ledPin, 0);
}

void loop() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read(); // empfangene Zeichen lesen
    /* konvertiere char zu int
    Da Zeichen wie '0' und '1' in ASCII gespeichert sind,
    entspricht '0' dem Wert 48 und '1' dem Wert 49.
    Durch das Subtrahieren von '0' wird der ASCII-Wert
    in den numerischen Wert umgewandelt */
    int receivedInt = receivedChar - '0';
    if (receivedInt == 0 || receivedInt == 1) {
      digitalWrite(ledPin, receivedInt);  // LED schalten
      Serial.println(receivedInt);
    }
  }
}
