// Switch_LED_vibration.ino

const int ledPin = 8;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  digitalWrite(ledPin, 0);
}

void loop() {
  // Überprüfe, ob Daten über die serielle Schnittstelle empfangen wurden
  if (Serial.available() > 0) {
    // Lese das empfangene Zeichen
    char receivedChar = Serial.read();
    /* Lese das empfangene Zeichen und konvertiere es in einen int
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
