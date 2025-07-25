/****************************************************
 * 06_beliebige_MQTT-Commands_versenden.ino
 * ESP32‑C6  ‑‑>  SIM7020e (NB‑IoT)  
 * MQTT‑Sequencer,der alle 20s eine Serie an MQTT-Nachrichten versendet.
 * Bevor den nächste Command abgeschickt wird, wird auf  "OK" oder "ERROR" reagiert.
 * Mehr Stabilität durch Timeout, falls sich Antwort lange verzögert + Soft-Reset, allenfalls Hard-Reset, falls sich Timeout wiederholt.
 *
 * Verkabelung (wie bisher):
 *   ESP32‑C6 5 V     <‑‑>  SIM7020e Vin
 *   ESP32‑C6 GND     <‑‑>  SIM7020e GND
 *   ESP32‑C6 GPIO10  <‑‑>  SIM7020e TX   (ESP RX)
 *   ESP32‑C6 GPIO11  <‑‑>  SIM7020e RX   (ESP TX)
 *   ESP32‑C6 GPIO12  <‑‑>  SIM7020e PWR  (aktiv HIGH) 
 ****************************************************/

#include <HardwareSerial.h>

/************** UART‑Instanz *************************/
HardwareSerial sim(1);           // UART1 → SIM7020
constexpr int PIN_SIM_RX = 10;   // ESP RX  (SIM TX)
constexpr int PIN_SIM_TX = 11;   // ESP TX  (SIM RX)

/************** PWRKEY‑Definition ********************/
constexpr int PIN_PWRKEY = 12;   // GPIO12 ist jetzt an den PWR‑Pin des SIM7020 geführt   // <<< NEU

/************** MQTT‑Parameter (variabel) ***********/           // <<< NEU
String   brokerAddress  = "broker.emqx.io";
String   brokerPort     = "1883";
uint16_t keepAlive      = 12000;     // Sek.
uint16_t rxBufferSize   = 100;       // TCP‑Recv‑Puffer

String   clientId       = "myclient";
String   topic          = "jan/test";
uint8_t  qosLevel       = 1;         // 0,1,2
uint8_t  retainedFlag   = 0;         // 0/1
String   payload        = "{\"msg\": \"wolfi\"}";

/************** AT‑Kommandos (werden gebaut) *********/          // <<< NEU
constexpr uint8_t NUM_COMMANDS = 5;
String AT_COMMANDS[NUM_COMMANDS];     // Platz für 5 dynamische Strings   // <<< NEU

/************** Konstanten ***************************/
constexpr uint32_t TIME_BETWEEN_SEQUENCES = 20000;   // 20s zwischen zwei Sequenzen
constexpr uint32_t OK_TIMEOUT   = 30000;             // 30s auf End‑Antwort warten

/************** Ablauf‑Status ************************/
uint8_t   cmdIndex        = 0;                                 // Welcher AT‑Befehl im Array AT_COMMANDS ist als Nächstes dran?
bool      stillWaitingFor_AT_response_completed  = false;      // warten wir noch auf die End‑Antwort?
uint32_t  lastCommandTimestamp    = 0;                         // Timestamp, wann der letzte Befehl gesendet wurde
uint32_t  lastCommandSequenceStartTimestamp  = 0;              // Zeit, wann die aktuelle Befehls-Sequenz begann
String    currentlyReceivedLine;                               // Puffer für die gerade empfangene Zeile
uint8_t   numSoftResets = 0;                                   // zählt, wie viele Software Resets in Serie auftraten

/************** Hilfsfunktionen ***********************/
/**
 * Baut alle AT‑Strings aus den obigen Variablen neu zusammen.
 * Diese Funktion kannst du später erneut aufrufen, wenn sich Parameter ändern.
 */
void buildATStrings() {
  AT_COMMANDS[0] = "AT+CMQNEW=\"" + brokerAddress + "\",\"" + brokerPort + "\"," +
                   String(keepAlive) + "," + String(rxBufferSize);

  AT_COMMANDS[1] = "AT+CMQCON=0,3,\"" + clientId + "\",600,1,0";

  AT_COMMANDS[2] = "AT+CREVHEX=0";

  /* Payload‑Quotes escapen, damit das Modem den String korrekt erkennt */   // <<< FIX
  String escapedPayload = payload;                                           // <<< FIX
  escapedPayload.replace("\"", "\\\"");                                      // <<< FIX

  AT_COMMANDS[3] = "AT+CMQPUB=0,\"" + topic + "\"," +
                   String(qosLevel) + "," + String(retainedFlag) + ",0," +
                   String(payload.length()) + ",\"" + escapedPayload + "\""; // <<< FIX

  AT_COMMANDS[4] = "AT+CMQDISCON=0";
}

/**
 * AT-Commands geben eine Antwort, ob es geklappt hat. "OK" oder "ERROR" (markiert das Ende eines AT‑Commands)
 * In dieser Funktion wird geprüft, ob die aktuell angekommene Zeile "OK" oder "ERROR" lautet. 
 * Nur dann (true) darf der nächste Command abgefeuert werden.
 */
inline bool is_AT_response_completed(const String &s) {
  if (s == "OK" || s == "ERROR") return true;
  return s.startsWith("+CME ERROR") || s.startsWith("+CMS ERROR");
}

/* Puffer der SIM7020‑UART komplett leeren. Verhindert Antwort‑Stau („alte“ OK/ERROR von vorherigen Befehlen). */
void flushSimBuffer() { while (sim.available()) sim.read(); }

/* Soft‑Reset des SIM7020 – AT+CRESET (Stack‑Neustart) */
void sim7020SoftReset() {
  Serial.println(F("\n########### Soft-Reset des SIM7020 (AT+CRESET) ..."));
  flushSimBuffer();
  sim.println("AT+CRESET");
  delay(5000);                      // SIM7020 rebootet
  flushSimBuffer();
  numSoftResets++;
}

/* Hard‑Reset via PWR‑Pin (aktiv HIGH, ≧100 ms) */
void sim7020HardReset() {
  Serial.println(F("\n################################### Hard-Reset des SIM7020 (PWRKEY High-Pulse) ..."));
  flushSimBuffer();
  digitalWrite(PIN_PWRKEY, HIGH);   // High‑Impuls schaltet Modul aus
  delay(1000);                      
  digitalWrite(PIN_PWRKEY, LOW);    
  delay(1000);                      
  digitalWrite(PIN_PWRKEY, HIGH);   
  delay(1000);                      
  digitalWrite(PIN_PWRKEY, LOW);    // Modul startet neu
  delay(8000);                      // Boot‑Zeit abwarten
  flushSimBuffer();
}

/***************** SETUP ******************************/
void setup() {
  Serial.begin(115200);                                      // USB‑Debug
  sim.begin(115200, SERIAL_8N1, PIN_SIM_RX, PIN_SIM_TX);     // UART‑Link

  /* PWRKEY‑GPIO einrichten (Leerlauf‑Pegel = LOW) */        // <<< NEU
  pinMode(PIN_PWRKEY, OUTPUT);
  digitalWrite(PIN_PWRKEY, LOW);

  delay(500);

  sim.println("AT+CMQTSYNC=1");   // SIM7020e in den MQTT‑Synchron‑Modus schalten
  Serial.println(F("\nESP32-C6 SIM7020e MQTT Sequencer (sync‑mode) ready."));

  buildATStrings();                // AT‑Strings einmal zusammensetzen   // <<< NEU
  flushSimBuffer();                // alle Boot‑Meldungen verwerfen      // <<< NEU
  send_AT_command(String("AT+CMQDISCON=0"));   // erstmal disconnecten   // <<< NEU
}

/***************** LOOP *******************************/
void loop() {
  const uint32_t now = millis();

  /* 1) Daten transparent durchreichen -> so kann man über den seriellen Monitor live mitlesen oder manuell AT‑Befehle tippen */
  while (Serial.available()) sim.write(Serial.read());   // PC  ➜ SIM7020
  while (sim.available()) {
    char c = sim.read();
    Serial.write(c);                                     // SIM7020 ➜ PC

    /* Zeilenweise Antwort sammeln, solange wir noch auf das Ende‑Statement (OK/ERROR) des gerade gesendeten Befehls warten. */
    if (stillWaitingFor_AT_response_completed) {
      if (c == '\n') {                                   // Zeilenende ➜ auswerten
        currentlyReceivedLine.trim();                    // \r & Leerzeichen entfernen
        if (is_AT_response_completed(currentlyReceivedLine)) {
          stillWaitingFor_AT_response_completed = false; // End‑Antwort erkannt
          cmdIndex++;
          if (cmdIndex == NUM_COMMANDS) {                // Sequenz fertig
            cmdIndex = 0;
            lastCommandSequenceStartTimestamp = now;
          }
        }
        currentlyReceivedLine = "";                      // Zeilenpuffer leeren
      } else if (c != '\r') {
        currentlyReceivedLine += c;                      // Zeichen puffern
      }
    }
  }

  /* 2) Timeout‑Überwachung:
   * Falls der SIM7020 gar nicht antwortet (Funkloch, Absturz, …) geben wir nach OK_TIMEOUT ms auf und gehen zum nächsten Step.  
   * So kann sich der Automat nicht aufhängen. */
  if (stillWaitingFor_AT_response_completed && now - lastCommandTimestamp >= OK_TIMEOUT) {
    Serial.println(F("\n!! TIMEOUT – kein Ende der Antwort des SIM7020 (OK oder ERROR) angekommen"));
    stillWaitingFor_AT_response_completed = false;       // Zwangs‑Weiter
    cmdIndex++;
    if (cmdIndex == NUM_COMMANDS) {
      cmdIndex = 0;
      lastCommandSequenceStartTimestamp = now;
    }

    /* Reset‑Strategie */
    sim7020SoftReset();                                  // Soft‑Reset zuerst
    if (numSoftResets >= 3) {                            // nach 3 Soft‑Resets → Hard‑Reset
      sim7020HardReset(); 
      numSoftResets = 0;
    }
  }

  /* 3) Command-Sequenz neu starten? ➜ Erst wenn die 20 s Pause (CYCLE_TIME) verstrichen sind, beginnt eine neue Sequenz. */
  if (!stillWaitingFor_AT_response_completed && cmdIndex == 0 &&
      now - lastCommandSequenceStartTimestamp >= TIME_BETWEEN_SEQUENCES) {
    Serial.println("---------------------------------------------");
    send_AT_command(AT_COMMANDS[cmdIndex]);              // erstes Kommando der Sequenz versenden
    stillWaitingFor_AT_response_completed = true;
    lastCommandTimestamp   = now;
    return;
  }

  /* 4) nächsten AT‑Befehl (2. bis letzter Befehl) abschicken */
  if (!stillWaitingFor_AT_response_completed &&
      cmdIndex > 0 && cmdIndex < NUM_COMMANDS) {
    send_AT_command(AT_COMMANDS[cmdIndex]);
    stillWaitingFor_AT_response_completed = true;
    lastCommandTimestamp   = now;
  }
}

/************** AT‑Befehl senden ************************/
/* hier wird der Befehl auf den UART des SIM7020e _und_ auf die USB‑Konsole gedruckt */
void send_AT_command(const String &cmd) {                 // <<< NEU: nimmt jetzt String
  flushSimBuffer();                                      
  sim.println(cmd.c_str());                               // an Modem
  Serial.print(F(">> "));
  Serial.println(cmd);                                    // an Konsole
  numSoftResets = 0;                                      // SoftReset‑Zähler zurück
}






/*
So sieht eine erfolgreiche Sequenz aus:

---------------------------------------------
>> AT+CMQNEW="broker.emqx.io","1883",12000,100
AT+CMQNEW="broker.emqx.io","1883",12000,100
AT+CMQNEW="broker.emqx.io","1883",12000,100

+CMQNEW: 0

OK
>> AT+CMQCON=0,3,"myclient",600,1,0
AT+CMQCON=0,3,"myclient",600,1,0
AT+CMQCON=0,3,"myclient",600,1,0

OK
>> AT+CREVHEX=0
AT+CREVHEX=0
AT+CREVHEX=0

OK
>> AT+CMQPUB=0,"jan/test",1,0,0,16,"{\"msg\": \"wolfi\"}"
AT+CMQPUB=0,"jan/test",1,0,0,16,"{\"msg\": \"wolfi\"}"
AT+CMQPUB=0,"jan/test",1,0,0,16,"{\"msg\": \"wolfi\"}"

OK
>> AT+CMQDISCON=0
AT+CMQDISCON=0
AT+CMQDISCON=0

OK
*/