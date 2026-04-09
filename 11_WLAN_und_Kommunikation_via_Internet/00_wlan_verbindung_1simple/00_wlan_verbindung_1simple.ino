/************************************************************************
 * WLAN-Verbindung Boilerplate
 * Bei bestehender WLAN-Verbindung leuchtet die LED_BUILTIN auf dem Mikrocontrollerboard
 ***********************************************************************/

#include <WiFi.h>

const char *ssid = "tinkergarden";  // your ssid
const char *pass = "strenggeheim";  // your PW
bool isWlanConnected = 0;

int led = LED_BUILTIN;

void setup()
{
    Serial.begin(115200);
    delay(3000);
    pinMode(led, OUTPUT);
    rgbLedWrite(led, 0, 255, 0);  // GRB rot
    Serial.println("Starte Verbindung...");
    connectWiFi();
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi-Verbindung verloren, reconnect...");
        connectWiFi();
        rgbLedWrite(led, 0, 255, 0);  // GRB rot
        isWlanConnected = 0;
    }
    else
    {
        if(isWlanConnected == 0){
            rgbLedWrite(led, 255, 0, 0); // GRB grün
            isWlanConnected = 1;
        }
    }
}

void connectWiFi()
{
    Serial.printf("\nVerbinde mit WLAN %s", ssid); // ssid ist const char*, kein String(ssid) nötig
    WiFi.begin(ssid, pass);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40)
    { // Max 20 Versuche (10 Sekunden)
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.printf("\nWiFi verbunden: SSID: %s, IP-Adresse: %s\n", ssid, WiFi.localIP().toString().c_str());
    }
    else
    {
        Serial.println("\n❌ WiFi Verbindung fehlgeschlagen!");
    }
}