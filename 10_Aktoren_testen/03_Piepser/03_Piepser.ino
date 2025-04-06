/*******************************************
 * 03_Piepser.ino (Code gleich wie 01_LED.ino)
 * Bauteil: KY-012
 * Empfange entweder eine 1 oder eine 0 am seriellen Port
 * und schalte die LED an GPIO8 entsprechend.
 * Verbinde:
 * Piepser: - (linkes Bein)     <->  ESP32-C6: GND
 * Piepser: + (mittleres Bein)  <->  ESP32-C6: 5V (lauter) oder 3.3V
 * Piepser: S (rechtes Bein)    <->  ESP32-C6: GPIO 8
 *******************************************/

const int piepserPin = 8;  

void setup() {
  pinMode(piepserPin, OUTPUT);
  Serial.begin(115200);
  digitalWrite(piepserPin, 0);
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
      digitalWrite(piepserPin, receivedInt);  // LED schalten
      Serial.println(receivedInt);
    }
  }
}
