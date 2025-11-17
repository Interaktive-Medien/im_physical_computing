/********************************************************************
 * stream ESP32 Cam to Node-Red
 * install Library PubSubClient by Nick O'Leary
 ********************************************************************/

#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include <PubSubClient.h>
// #include "credentials.h"

#define CAMERA_MODEL_AI_THINKER
// #include "camera_pins.h"

// Flash
#define LED_BUILTIN 4

//WIFI config -> credentials.h
const char* mqttServer = "broker.emqx.io";
const char* HostName = "pixelEDi_Node-RED";
const char* mqttUser = "admin";
const char* mqttPassword = "strenggeheim";
bool useMQTT = true;
const char* topic_PHOTO = "takePicture";
const char* topic_PUBLISH = "sendPicture";
const char* topic_FLASH = "setFlash";
const int MAX_PAYLOAD = 60000;

bool flash;

WiFiClient espClient;
PubSubClient client(espClient);

void startCameraServer();

void sendMQTT(const uint8_t * buf, uint32_t len) {
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
  if ((!flash)) {
    for (int i = 0; i < 6; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }

  if (flash) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
}

void take_picture() {
  camera_fb_t * fb = NULL;
  if (flash) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  Serial.println("Taking picture");

  fb = esp_camera_fb_get(); // used to get a single picture.

  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  Serial.println("Picture taken");
  digitalWrite(LED_BUILTIN, LOW);

  esp_camera_fb_return(fb);
  delay(100);
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  delay(100);
  fb = esp_camera_fb_get();
  sendMQTT(fb->buf, fb->len);
delay(300);
esp_camera_fb_return(fb); // must be used to free the memory allocated by esp_camera_fb_get().

void callback(String topic, byte* message, unsigned int length) {
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

  // Define Flash as an output
  pinMode(LED_BUILTIN, OUTPUT);

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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;


  // init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10; //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12; //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  WiFi.mode(WiFi_STA);
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
  client.setBufferSize(MAX_PAYLOAD); //This is the maximum payload length
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HostName, mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe(topic_PHOTO);
      client.subscribe(topic_FLASH);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}