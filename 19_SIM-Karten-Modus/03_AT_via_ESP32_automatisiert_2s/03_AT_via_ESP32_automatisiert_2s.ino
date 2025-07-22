/****************************************************
 * 03_AT_via_ESP32_automatisiert_2s.ino
 * ESP32‑C6  ‑‑>  SIM7020e (NB‑IoT)  
 * Automatisches Senden einer MQTT‑Sequenz
 *
 * Verkabelung (wie bisher):
 *   ESP32‑C6 5 V     <‑‑>  SIM7020e Vin
 *   ESP32‑C6 GND     <‑‑>  SIM7020e GND
 *   ESP32‑C6 GPIO10  <‑‑>  SIM7020e TX   (ESP RX)
 *   ESP32‑C6 GPIO11  <‑‑>  SIM7020e RX   (ESP TX)
 *
 * Sequenz alle 20 s:
 *   0 s   AT+CMQNEW="broker.emqx.io","1883",12000,100
 *   +2 s  AT+CMQCON=0,3,"myclient",600,0,0
 *   +4 s  AT+CREVHEX=0
 *   +6 s  AT+CMQPUB=0,"jan/test",1,0,0,14,"{\"msg\": \"hoi\"}"
 *   +8 s  AT+CMQDISCON=0
 ****************************************************/

#include <HardwareSerial.h>

HardwareSerial sim(1);                 // UART1 fürs Modem
constexpr int PIN_SIM_RX = 10;         // ESP32‑C6 RX  (mit SIM7020e TX)
constexpr int PIN_SIM_TX = 11;         // ESP32‑C6 TX  (mit SIM7020e RX)

/* ------------------- Konstante AT‑Befehle ------------------- */
const char *const AT_CMDS[] = {
  "AT+CMQNEW=\"broker.emqx.io\",\"1883\",12000,100",
  "AT+CMQCON=0,3,\"myclient\",600,0,0",
  "AT+CREVHEX=0",
  "AT+CMQPUB=0,\"jan/test\",1,0,0,14,\"{\\\"msg\\\": \\\"hoi\\\"}\"",
  "AT+CMQDISCON=0"
};
constexpr uint8_t NUM_CMDS     = sizeof(AT_CMDS) / sizeof(AT_CMDS[0]);
constexpr uint32_t CMD_SPACING = 2000;   // 1 s zwischen zwei Befehlen
constexpr uint32_t CYCLE_TIME  = 20000;  // 15 s bis neue Sequenz startet

/* ------------------- Steuer‑Variablen ------------------- */
uint8_t   cmdIndex          = 0;         // Index des nächsten Befehls
uint32_t  lastCmdTimestamp  = 0;         // Zeitstempel des letzten Tx
uint32_t  lastCycleStart    = 0;         // Beginn der letzten Sequenz

/* -------------------------------------------------------- */
void setup() {
  Serial.begin(115200);                               // USB‑Debug
  sim.begin(115200, SERIAL_8N1, PIN_SIM_RX, PIN_SIM_TX); // Modem‑UART

  delay(500);                                         // kleine Pause
  Serial.println(F("\nESP32‑C6 SIM7020e MQTT‑Sequencer ready."));
}

void loop() {
  const uint32_t now = millis();

  /******** 1) Modem‑ und PC‑Daten durchreichen (Debug) ********/
  while (Serial.available()) sim.write(Serial.read());  // PC  -> Modem
  while (sim.available())    Serial.write(sim.read());  // Modem -> PC

  /******** 2) Automatische AT‑Sequenz ********/
  /* Start einer neuen 15‑s‑Sequenz? */
  if (cmdIndex == 0 && (now - lastCycleStart >= CYCLE_TIME)) {
    sendAT(AT_CMDS[cmdIndex]);         // ersten Befehl feuern
    lastCmdTimestamp = now;
    cmdIndex = 1;                      // nächster Index vorbereiten
    return;                            // auf nächsten Loop‑Tick warten
  }

  /* Zwischenbefehle im 1‑s‑Abstand senden */
  if (cmdIndex > 0 && cmdIndex < NUM_CMDS &&
      now - lastCmdTimestamp >= CMD_SPACING) {

    sendAT(AT_CMDS[cmdIndex]);
    lastCmdTimestamp = now;
    cmdIndex++;

    /* Sequenz fertig? -> für nächsten Zyklus zurücksetzen */
    if (cmdIndex == NUM_CMDS) {
      cmdIndex = 0;
      lastCycleStart = now;            // neue 15‑s‑Periode starten
    }
  }
}

/* ---------- Hilfsroutine: Befehl an Modem & Console ---------- */
void sendAT(const char *cmd) {
  sim.println(cmd);        // CR+LF für das Modem
  Serial.print(F(">> "));  // Echo zur PC‑Konsole
  Serial.println(cmd);
}
