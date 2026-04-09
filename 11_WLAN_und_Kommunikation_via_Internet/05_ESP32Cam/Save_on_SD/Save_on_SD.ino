#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

// ---- Kamera-Pinbelegung für AI Thinker ----
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nInitializing...");

  // ---- Kamera-Konfiguration ----
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // hohe Auflösung (kann geändert werden)
  config.frame_size = FRAMESIZE_SVGA;  // Alternativen: FRAMESIZE_UXGA, XGA, SVGA, VGA
  config.jpeg_quality = 10;            // Bereich: 0-63 (niedriger = bessere Qualität)
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  // ---- SD Karte starten ----
  Serial.println("Mounting SD card...");
  if(!SD_MMC.begin("/sdcard", true)){
    Serial.println("SD Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card found");
    return;
  }

  Serial.println("SD card ready.");

  // ---- Foto aufnehmen ----
  camera_fb_t * fb = esp_camera_fb_get();
  if(!fb){
    Serial.println("Camera capture failed");
    return;
  }

  static int pictureNumber = 0;
  String path = "/picture_" + String(pictureNumber++) + ".jpg";

  Serial.printf("Saving picture: %s\n", path.c_str());

  File file = SD_MMC.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file");
  } else {
    file.write(fb->buf, fb->len);  // JPEG direkt speichern
    Serial.println("File saved successfully!");
  }
  file.close();

  esp_camera_fb_return(fb);
}

void loop() {
  // nichts — Foto wird nur beim Start gespeichert
}
