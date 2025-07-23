/****************************************************
 * 04_AT_via_ESP32_automatisiert.ino
 * ESP32‑C6  ‑‑>  SIM7020e (NB‑IoT)  
 * MQTT‑Sequencer,der alle 20s eine Serie an MQTT-Nachrichten versendet
 * Bevor den nächste Command abgeschickt wird, wird auf  OK **oder ERROR**  reagiert
 *
 * Verkabelung (wie bisher):
 *   ESP32‑C6 5 V     <‑‑>  SIM7020e Vin
 *   ESP32‑C6 GND     <‑‑>  SIM7020e GND
 *   ESP32‑C6 GPIO10  <‑‑>  SIM7020e TX   (ESP RX)
 *   ESP32‑C6 GPIO11  <‑‑>  SIM7020e RX   (ESP TX)
 *
 * Sequenz:
 * AT+CMQNEW="broker.emqx.io","1883",12000,100            // Verbindung zum MQTT-Broker aufbauen (TCP + MQTT‑Context)
 * AT+CMQCON=0,3,"myclient",600,0,0                       // Festlegung Client-ID: MQTT‑CONNECT (Client‑ID …)
 * AT+CREVHEX=0                                           // Deaktivierung der Hexadezimal-Kodierung für empfangene Daten, stattdessen im Rohformat (ASCII/Binär)
 * AT+CMQPUB=0,"jan/test",1,0,0,14,"{\"msg\": \"hoi\"}"   // MQTT-Topic und Payload versenden (QoS1, retained=0)
 * AT+CMQDISCON=0                                         // Verbindung zum MQTT-Broker beenden
 ****************************************************/

#include <HardwareSerial.h>

/************** UART‑Instanz *************************/
HardwareSerial sim(1);           // UART1 → Modem
constexpr int PIN_SIM_RX = 10;   // ESP RX  (SIM TX)
constexpr int PIN_SIM_TX = 11;   // ESP TX  (SIM RX)

/************** AT‑Kommandos *************************/
const char *const AT_COMMANDS[] = {
  "AT+CMQNEW=\"broker.emqx.io\",\"1883\",12000,100",
  "AT+CMQCON=0,3,\"myclient\",600,1,0",        // cleansession = 1
  "AT+CREVHEX=0",
  "AT+CMQPUB=0,\"jan/test\",1,0,0,14,\"{\\\"msg\\\": \\\"hoi\\\"}\"",
  "AT+CMQDISCON=0"
};
constexpr uint8_t  NUM_COMMANDS     = sizeof(AT_COMMANDS) / sizeof(AT_COMMANDS[0]);
constexpr uint32_t TIME_BETWEEN_SEQUENCES = 20000;   // 20s zwischen zwei Sequenzen
constexpr uint32_t OK_TIMEOUT   = 12000;             // 12s auf End‑Antwort warten

/************** Ablauf‑Status ************************/
uint8_t   cmdIndex        = 0;                                 // Welcher AT‑Befehl im Array AT_COMMANDSist als Nächstes dran?
bool      stillWaitingFor_AT_response_completed  = false;      // warten wir noch auf die End‑Antwort?
uint32_t  lastCommandTimestamp    = 0;                         // Timestamp, wann der letzte Befehl gesendet wurde
uint32_t  lastCommandSequenceStartTimestamp  = 0;              // Zeit, wann die aktuelle Befehls-Sequenz begann
String    currntlyReceivedLine;                                // Puffer für die gerade empfangene Zeile

/************** Hilfsfunktion ************************/
/**
 * AT-Commands geben eine Antwort, ob es geklappt hat. "OK" oder "ERROR" (markiert das Ende eines AT‑Commands)
 * In dieser Funktion wird geprüft, ob die aktuell angekommene Zeile "OK" oder "ERROR" lautet. 
 * Nur dann (true) darf der nächste Command abgefeuert werden.
 */
inline bool is_AT_response_completed(const String &s) {
  if (s == "OK" || s == "ERROR") return true;
  return s.startsWith("+CME ERROR") || s.startsWith("+CMS ERROR");
}


void setup() {
  Serial.begin(115200);                                      // USB‑Debug
  sim.begin(115200, SERIAL_8N1, PIN_SIM_RX, PIN_SIM_TX);     // UART‑Link
  delay(500);

  sim.println("AT+CMQTSYNC=1");   // SIM7020e in den MQTT‑Synchron‑Modus schalten: "OK" für CMQPUB/CMQCON/etc. erscheint erst, wenn der Broker das jeweilige Paket quittiert hat
  Serial.println(F("\nESP32-C6 SIM7020e MQTT Sequencer (sync-mode) ready."));

  send_AT_command("AT+CMQDISCON=0");             // erstmal disconnecten
}


void loop() {
  const uint32_t now = millis();

  /* 1) Daten transparent durchreichen -> so kann man über den seriellen Monitor live mitlesen oder manuell AT‑Befehle tippen */

  while (Serial.available()) sim.write(Serial.read());   // PC  ➜ SIM7020
  while (sim.available()) {
    char c = sim.read();
    Serial.write(c);                                     // SIM7020 ➜ PC

    /* Zeilenweise Antwort sammeln, solange wir noch auf das Ende‑Statement (OK/ERROR) des gerade gesendeten Befehls warten. */

    if (stillWaitingFor_AT_response_completed) {
      if (c == '\n') {                                          // Es werden characters einzeln übertragen. '\n' heisst, dass die Zeile beendet wird → auswerten
        currntlyReceivedLine.trim();                            // \r (= zurück zum Zeilenanfang) & Leerzeichen entfernen
        if (is_AT_response_completed(currntlyReceivedLine)) {   // OK oder ERROR usw.
          // End‑Antwort erkannt ➜ Nächster Befehl darf folgen ➜ boolean wird auf false gesetzt
          stillWaitingFor_AT_response_completed = false;
          cmdIndex++;
          if (cmdIndex == NUM_COMMANDS) {                           // aktuelle Command-Sequenz fertig
            cmdIndex = 0;
            lastCommandSequenceStartTimestamp = now;            // Sequenz‑Timer zurücksetzen
          }
        }
        currntlyReceivedLine = "";       // Zeilenpuffer leeren
      } else if (c != '\r') {
        currntlyReceivedLine += c;       // Zeichen puffern
      }
    }
  }

  /* 2) Timeout‑Überwachung:
   * Falls der SIM7020 gar nicht antwortet (Funkloch, Absturz, …) geben wir nach OK_TIMEOUT ms auf und gehen zum nächsten Step.  
   * So kann sich der Automat nicht aufhängen. */

  if (stillWaitingFor_AT_response_completed && now - lastCommandTimestamp >= OK_TIMEOUT) {
    Serial.println("!! TIMEOUT – kein Ende der Antwort des SIM7020 (OK oder ERROR) angekommen \n");
    stillWaitingFor_AT_response_completed = false;              // Zwangs-Weiter im Ablauf
    cmdIndex++;
    if (cmdIndex == NUM_COMMANDS) {
      cmdIndex = 0;
      lastCommandSequenceStartTimestamp = now;
    }
  }

  /* 3) Command-Sequenz neu starten? ➜ Erst wenn die 20 s Pause (CYCLE_TIME) verstrichen sind, beginnt eine neue Sequenz. */

  if (!stillWaitingFor_AT_response_completed && cmdIndex == 0 && now - lastCommandSequenceStartTimestamp >= TIME_BETWEEN_SEQUENCES) {
    Serial.println("---------------------------------------------");
    send_AT_command(AT_COMMANDS[cmdIndex]);                  // erstes Kommando der Sequenz versenden
    stillWaitingFor_AT_response_completed = true;
    lastCommandTimestamp   = now;
    return;                                                  // springe in den nächsten loop‑Durchlauf
  }

  /* 4) nächsten AT‑Befehl (2. bis letzter Befehl (cmdIndex > 0)) abschicken  ➜ Sobald die vorherige Antwort (OK oder ERROR) da ist, wird der nächste AT‑String an den SIM7020 geschickt.*/

  if (!stillWaitingFor_AT_response_completed && cmdIndex > 0 && cmdIndex < NUM_COMMANDS) {
    send_AT_command(AT_COMMANDS[cmdIndex]);
    stillWaitingFor_AT_response_completed = true;
    lastCommandTimestamp   = now;
  }
}

/************** AT‑Befehl senden ************************/
/* hier wird der Befehl auf die Modem‑UART _und_ auf die USB‑Konsole gedruckt, damit man im Log klar sieht, was gerade passiert.*/

void send_AT_command(const char *cmd) {
  sim.println(cmd);
  Serial.print(F(">> "));
  Serial.println(cmd);
}
