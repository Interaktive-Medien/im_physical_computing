/************************************************************************
 * API Request an die OpenAI API mit einem Prompt
 * Szenario: durchschnittliche Temperatur der letzten 50 Jahre schätzen und daraufhin auf der Konsole "colder than usual" oder "hotter than usual" ausgeben.
 * Code based on WLAN-Verbindung Boilerplate
 * Start und Ende der Modifikation gekennzeichnet mit //#####
 ***********************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>

const char *ssid = "FRITZ!Box 6690 TA";    // your ssid, eg. tinkergarden
const char *pass = "79854308499311013585"; // your PW, eg. strenggeheim

int led = LED_BUILTIN;

void setup()
{
    Serial.begin(115200);
    delay(3000);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial.println("Starte Verbindung...");
    connectWiFi();
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi-Verbindung verloren, reconnect...");
        connectWiFi();
        rgbLedWrite(led, 0, 255, 0); // GRB rot
    }
    else
    {
        rgbLedWrite(led, 255, 0, 0); // GRB grün, if WLAN connected

        // #####

        static bool requestDone = false;
        float currentTemperature = 10.0; // Beispielwert, in der Realität könnte dieser von einem Sensor kommen
        static float averageTemperature = NAN;

        if (!requestDone)
        {
            const char *apiKey = "sk-your_openai_api_key";
            const char *url = "https://api.openai.com/v1/chat/completions";

            /*
            Folgende JSON Struktur aufbauen:

            {
                "model": "gpt-4o-mini",
                "messages": [
                    {
                        "role": "user",
                        "content": "Prompt hier eingeben"
                    }
                ]
            }

            */
            // API Keys gehören nicht in den JSON String bzw.in den HTTP Body, sondern in den HTTP Header (Authorization: Bearer <API_KEY>)

            JSONVar payload;
            payload["model"] = "gpt-4o-mini";

            JSONVar msg;
            msg["role"] = "user";
            msg["content"] =
                "Gib eine realistische klimatologische Schätzung der durchschnittlichen Temperatur der letzten 50 Jahre für Chur (GR, Schweiz) am heutigen Datum zur aktuellen Uhrzeit in °C. Falls keine exakten Daten verfügbar sind, schätze den Wert plausibel. Antworte ausschließlich als JSON im Format {\"averageTemperature\": <float>}. Gib nur den JSON String zurück, ohne weitere Erklärungen, Zeichen oder Text.";

            JSONVar messages;
            messages[0] = msg;
            payload["messages"] = messages;

            WiFiClientSecure client;
            client.setInsecure();

            HTTPClient https;
            https.begin(client, url);
            https.addHeader("Content-Type", "application/json");
            https.addHeader("Authorization", String("Bearer ") + apiKey);

            Serial.println("sending request...");

            int httpCode = https.POST(JSON.stringify(payload));
            if (httpCode > 0)
            {
                Serial.println("response receiced");

                JSONVar root = JSON.parse(https.getString());
                if (JSON.typeof(root) != "undefined")
                {
                    String content = (const char *)root["choices"][0]["message"]["content"];
                    content.trim();

                    Serial.print("raw content = ");
                    Serial.println(content);

                    JSONVar value = JSON.parse(content);
                    if (JSON.typeof(value) == "object")
                    {
                        averageTemperature = (double)value["averageTemperature"];
                    }
                    else
                    {
                        averageTemperature = content.toFloat();
                    }

                    Serial.print("averageTemperature = ");
                    Serial.println(averageTemperature, 2);

                    if (averageTemperature > currentTemperature)
                    {
                        Serial.println("Hotter than usual");
                    }
                    else
                    {
                        Serial.println("Colder than usual");
                    }

                    requestDone = true;
                }
            }
            else
            {
                Serial.printf("HTTP Fehler: %d\n", httpCode);
            }

            https.end();

            // #####
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

/*
mach einen HTTP REquest an die OpenAI API (API Kay ist vorhanden). Ich habe folgenden Prompt: "Berechne mir die Durchschnittstemperatur der vergangenen 50 Jahre in Chur GR in °C am heutigen Tag zur aktuellen Uhrzeit. Nur einen Float-Wert zurückgeben, ohne Einheit °C und ohne weiteren Text."
Verchicke diesen Prompt formatiert in JSON. Nutze dazu die Library Arduino_JSON.
Als Antwort erwarte ich ebenfalls einen JSON String, der allerdings nur die Zahl beinhalten sollte. Dieser Wert muss in einen Float-Wert umgewandelt werden.
Dieser Wert soll in der Variable float averageTemperature; gespeichert werden.
Erstelle mir den entsprechenden Arduino-Code.
*/