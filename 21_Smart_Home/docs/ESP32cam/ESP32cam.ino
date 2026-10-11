

#include "credentials.h"
#include "driver/ledc.h"
#include "esp_camera.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
#include <PubSubClient.h>
#include <WiFi.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// Flash
#define LED_BUILTIN 4

// WIFI config -> credentials.h

const char *mqttServer = "192.168.0.51";
const char *HostName = "kp Node-RED";
const char *mqttUser = "admin";
const char *mqttPassword = "strenggeheim";
bool useMQTT = true;
const char *topic_PHOTO = "takePicture";
const char *topic_PUBLISH = "sendPicture";
const char *topic_FLASH = "setFlash";
const int MAX_PAYLOAD = 60000;

bool flash;

// LED Status-Konfiguration via Hardware-PWM (LEDC)
#define FLASH_GPIO 4
#define FLASH_LEDC_CHANNEL                                                     \
  7 // Channel 7 (Channel 0 wird von Kamera XCLK genutzt)

// Helligkeit für "schwach weiß" (Wertebereich 0 bis 255):
// Wert 1 entspricht ca. 0.4% Pulsweite bei 1000 Hz.
const int LED_DIM_BRIGHTNESS = 1;
const unsigned long BLINK_INTERVAL =
    150; // Schnelles Blinken (150 ms an / 150 ms aus) wenn nur WLAN verbunden

unsigned long lastBlinkTime = 0;
bool blinkState = false;
bool isFlashing = false;
unsigned long lastReconnectAttempt = 0;
int currentBrightness = -1;

void initLedPWM() {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(FLASH_GPIO, 1000, 8); // 1000 Hz Frequenz, 8-Bit Aufloesung
  ledcWrite(FLASH_GPIO, 0);
#else
  ledcSetup(FLASH_LEDC_CHANNEL, 1000, 8); // 1000 Hz Frequenz, 8-Bit Aufloesung
  ledcAttachPin(FLASH_GPIO, FLASH_LEDC_CHANNEL);
  ledcWrite(FLASH_LEDC_CHANNEL, 0);
#endif
  currentBrightness = 0;
}

void setLedBrightness(int duty) {
  if (duty < 0)
    duty = 0;
  if (duty > 255)
    duty = 255;
  if (duty == currentBrightness)
    return; // Nur aktualisieren wenn Wert sich aendert
  currentBrightness = duty;
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(FLASH_GPIO, duty);
#else
  ledcWrite(FLASH_LEDC_CHANNEL, duty);
#endif
}

WiFiClient espClient;
PubSubClient client(espClient);

void startCameraServer();

void update_status_led() {
  if (isFlashing) {
    return; // Blitz bzw. Testblinken hat Vorrang
  }

  if (WiFi.status() != WL_CONNECTED) {
    // 1. Kein WLAN: LED aus (schwarz)
    setLedBrightness(0);
  } else if (!client.connected()) {
    // 2. Nur WLAN verbunden (sucht MQTT): schnell schwach blinken (10 von 255)
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = currentMillis;
      blinkState = !blinkState;
      setLedBrightness(blinkState ? LED_DIM_BRIGHTNESS : 0);
    }
  } else {
    // 3. WLAN und MQTT verbunden: konstant schwach weiß (10 von 255)
    setLedBrightness(LED_DIM_BRIGHTNESS);
  }
}

void sendMQTT(const uint8_t *buf, uint32_t len) {
  Serial.println("Sending picture...");
  if (len > MAX_PAYLOAD) {
    Serial.print("Picture too large, increase the MAX_PAYLOAD value");
  } else {
    Serial.print("Picture sent? : ");
    Serial.println(client.publish(topic_PUBLISH, buf, len, false));
  }
}

void set_flash() {
  flash = !flash;
  Serial.print("Setting flash to ");
  Serial.println(flash);
  isFlashing = true;
  if (!flash) {
    for (int i = 0; i < 6; i++) {
      setLedBrightness(255);
      delay(100);
      setLedBrightness(0);
      delay(100);
    }
  }
  if (flash) {
    for (int i = 0; i < 3; i++) {
      setLedBrightness(255);
      delay(500);
      setLedBrightness(0);
      delay(100);
    }
  }
  isFlashing = false;
  update_status_led(); // Nach dem Testblinken wieder Status herstellen
}

void take_picture() {
  camera_fb_t *fb = NULL;
  if (flash) {
    isFlashing = true;
    setLedBrightness(255);
    delay(50);
  }
  Serial.println("Taking picture");

  fb = esp_camera_fb_get(); // used to get a single picture.

  if (!fb) {
    Serial.println("Camera capture failed");
    if (flash) {
      isFlashing = false;
      update_status_led();
    }
    return;
  }
  Serial.println("Picture taken");

  esp_camera_fb_return(fb);
  delay(100);
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  delay(100);
  fb = esp_camera_fb_get();

  if (flash) {
    isFlashing = false;
    update_status_led(); // Nach dem Blitz sofort wieder zum vorherigen Status
                         // zurückkehren
  }

  sendMQTT(fb->buf, fb->len);
  delay(300);
  esp_camera_fb_return(
      fb); // must be used to free the memory allocated by esp_camera_fb_get().
}

void callback(String topic, byte *message, unsigned int length) {
  String messageTemp;
  Serial.println(topic);
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  if (topic == topic_PHOTO) {
    take_picture();
  }
  if (topic == topic_FLASH) {
    set_flash();
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Initialise the Serial Communication
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // Config Camera Settings
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10; // 0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12; // 0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  // WICHTIG: initLedPWM() NACH esp_camera_init() aufrufen!
  // esp_camera_init() initialisiert das LEDC-Modul fuer XCLK neu.
  // Hier wird GPIO 4 nach dem Kamera-Start sauber an PWM gebunden.
  initLedPWM();
  setLedBrightness(0); // Anfangs schwarz (aus)

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  // Set MQTT Connection
  client.setServer(mqttServer, 1883);
  client.setBufferSize(MAX_PAYLOAD); // This is the maximum payload length
  client.setCallback(callback);
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (client.connect(HostName, mqttUser, mqttPassword)) {
    Serial.println("connected");
    client.subscribe(topic_PHOTO);
    client.subscribe(topic_FLASH);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
  }
}

void loop() {
  update_status_led();

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      unsigned long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        reconnect();
      }
    } else {
      client.loop();
    }
  }
}