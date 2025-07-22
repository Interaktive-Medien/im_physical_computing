/*********************************************
 * AT-Commands an SIM7020e versenden ohne FTDI Converter, stattdessen ESP32-c6
 * Verbinde:
 * ESP32-C6: 5V         <->   SIM7020e: Vin
 * ESP32-C6: GND        <->   SIM7020e: GND
 * ESP32-C6: D10 (RX)   <->   SIM7020e: TX
 * ESP32-C6: D11 (TX)   <->   SIM7020e: RX
 * Usage:
 * AT-Command eingeben in Serial Console. Dieser wird vom ESP32 dann an den sim7020 geschickt.
 * Der SIM7020 antwortet daraufhin. Starte mit "AT" und erwarte ein "OK"
 * Beispiel-AT-Commands, um eine MQTT-Nachricht zu versenden:
 * -   AT+CMQNEW="broker.emqx.io","1883",12000,100
 * -   AT+CMQCON=0,3,"myclient",600,0,0
 * -   AT+CREVHEX=0
 * -   AT+CMQPUB=0,"jan/test",1,0,0,14,"{\"msg\": \"hoi\"}"
 * -   AT+CMQDISCON=0
**********************************************/


#include <HardwareSerial.h>

HardwareSerial sim(1);               // UART1
constexpr int PIN_SIM_RX = 10;      
constexpr int PIN_SIM_TX = 11;       



void setup() {
  Serial.begin(115200);              // USB‑Debug
  sim.begin(115200, SERIAL_8N1, PIN_SIM_RX, PIN_SIM_TX); // Modem
}

void loop() {
  if (Serial.available())            // PC → Modem
    sim.write(Serial.read());

  if (sim.available())               // Modem → PC
    Serial.write(sim.read());
}
