/*******************************************
 * 01_LED.ino
 * Empfange entweder eine 1 oder eine 0 am seriellen Port
 * und schalte die LED an GPIO8 entsorechend.
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
