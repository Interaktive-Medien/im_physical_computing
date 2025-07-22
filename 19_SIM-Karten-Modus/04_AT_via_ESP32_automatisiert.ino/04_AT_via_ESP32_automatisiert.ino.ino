/****************************************************
 * ESP32‑C6  ->  SIM7020e (NB‑IoT)
 * MQTT‑Sequencer, der auf  OK **oder ERROR**  reagiert
 * und mit  AT+CMQTSYNC=1  im synchronen MQTT‑Modus läuft
 ****************************************************/
#include <HardwareSerial.h>

HardwareSerial sim(1);
constexpr int PIN_SIM_RX = 10;   // ESP RX  (SIM TX)
constexpr int PIN_SIM_TX = 11;   // ESP TX  (SIM RX)

/* ---------- AT‑Kommandos ---------- */
const char *const AT_CMDS[] = {
  "AT+CMQNEW=\"broker.emqx.io\",\"1883\",12000,100",
  "AT+CMQCON=0,3,\"myclient\",600,1,0",        // cleansession = 1
  "AT+CREVHEX=0",
  "AT+CMQPUB=0,\"jan/test\",1,0,0,14,\"{\\\"msg\\\": \\\"hoi\\\"}\"",
  "AT+CMQDISCON=0"
};
constexpr uint8_t  NUM_CMDS     = sizeof(AT_CMDS) / sizeof(AT_CMDS[0]);
constexpr uint32_t CYCLE_TIME   = 20000;   // 20 s zwischen zwei Sequenzen
constexpr uint32_t OK_TIMEOUT   = 8000;    // 8 s auf End‑Antwort warten

/* ---------- Ablauf‑Variablen ---------- */
uint8_t   cmdIndex        = 0;
bool      awaitingAnswer  = false;
uint32_t  lastCmdStamp    = 0;
uint32_t  lastCycleStart  = 0;
String    respLine;

/* ---------- Hilfs‑Funktion ---------- */
inline bool isFinalResp(const String &s) {
  if (s == "OK" || s == "ERROR") return true;
  return s.startsWith("+CME ERROR") || s.startsWith("+CMS ERROR");
}

/* -------------------------------------------------------------- */
void setup() {
  Serial.begin(115200);
  sim.begin(115200, SERIAL_8N1, PIN_SIM_RX, PIN_SIM_TX);
  delay(500);

  /*  👉  MQTT‑Synchronisationsmodus einschalten  */
  sim.println("AT+CMQTSYNC=1");
  Serial.println(F("\nESP32‑C6 SIM7020e MQTT Sequencer (sync‑mode) ready."));
}

/* ---------- Haupt‑Loop ---------- */
void loop() {
  const uint32_t now = millis();

  /* 1) Daten transparent durchreichen  */
  while (Serial.available()) sim.write(Serial.read());
  while (sim.available()) {
    char c = sim.read();
    Serial.write(c);

    /* Zeichen puffern, bis Zeilenende */
    if (awaitingAnswer) {
      if (c == '\n') {                   // Zeile komplett
        respLine.trim();
        if (isFinalResp(respLine)) {     // OK oder ERROR usw.
          awaitingAnswer = false;
          cmdIndex++;
          if (cmdIndex == NUM_CMDS) {    // Zyklus fertig
            cmdIndex = 0;
            lastCycleStart = now;
          }
        }
        respLine = "";
      } else if (c != '\r') {
        respLine += c;
      }
    }
  }

  /* 2) Timeout‑Überwachung */
  if (awaitingAnswer && now - lastCmdStamp >= OK_TIMEOUT) {
    Serial.println(F("!! TIMEOUT – keine End‑Antwort"));
    awaitingAnswer = false;              // weiter im Ablauf
    cmdIndex++;
    if (cmdIndex == NUM_CMDS) {
      cmdIndex = 0;
      lastCycleStart = now;
    }
  }

  /* 3) Zyklus neu starten? */
  if (!awaitingAnswer && cmdIndex == 0 &&
      now - lastCycleStart >= CYCLE_TIME) {
    sendAT(AT_CMDS[cmdIndex]);
    awaitingAnswer = true;
    lastCmdStamp   = now;
    return;
  }

  /* 4) nächsten AT‑Befehl abschicken */
  if (!awaitingAnswer && cmdIndex < NUM_CMDS) {
    sendAT(AT_CMDS[cmdIndex]);
    awaitingAnswer = true;
    lastCmdStamp   = now;
  }
}

/* ---------- sendAT: an Modem & Debug‑Echo ---------- */
void sendAT(const char *cmd) {
  sim.println(cmd);
  Serial.print(F(">> "));
  Serial.println(cmd);
}
