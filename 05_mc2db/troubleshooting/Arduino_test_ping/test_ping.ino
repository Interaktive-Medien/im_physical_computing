/******************************************************
  Test Ping
******************************************************/

#include <ESPping.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(100);
  const char* ssid = "dreammakers";
  const char* password = "dreammakers";

  // Verbindungsaufbau zu WiFi
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.printf("\nWiFi connected: SSID: %s, Password: %s, IP Address: %s, Gateway: %s", ssid, password, WiFi.localIP().toString().c_str(), WiFi.gatewayIP().toString().c_str());
}

void loop() {
  
  /****************************** Ping an remote IP-Adresse*/

  const IPAddress remote_ip(192, 168, 0, 98);
  Serial.print("Pinging remote IP address (");
  Serial.print(remote_ip);
   
  if (Ping.ping(remote_ip)) {
    Serial.printf(") was successful: Min / Avg / Max time: %d / %d / %d ms\n", Ping.minTime(), Ping.averageTime(), Ping.maxTime());
  } else {
    Serial.println(") failed!");
  }
  delay(1000);

  /******************************  Ping an einen Host (Domain, keine URL) */

  const char* remote_host = "358918-6.web.fhgr.ch";
  Serial.print("Pinging remote host (");
  Serial.print(remote_host);
  
  if (Ping.ping(remote_host)) {
    Serial.printf(") was successful: Min / Avg / Max time: %d / %d / %d ms\n", Ping.minTime(), Ping.averageTime(), Ping.maxTime());
  } else {
    Serial.println(") failed!");
  }
  delay(1000);

  /****************************** Ping an lokales Gateway */

  IPAddress gateway_ip = WiFi.gatewayIP();
  Serial.print("Pinging gateway (");
  Serial.print(gateway_ip);
  
  if (Ping.ping(gateway_ip)) {
    Serial.printf(") was successful: Min / Avg / Max time: %d / %d / %d ms\n", Ping.minTime(), Ping.averageTime(), Ping.maxTime());
  } else {
    Serial.println(") failed!");
  }
  delay(1000);

  Serial.println("----------------------------------------------------------------");
}
