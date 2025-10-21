/**************************************************************
* 01_I2S_Microphone.ino (based on Espressif I2S example)
*  Reads sound data from I2S microphone using ESP32
*  Computes mean signal strength and prints to serial port
*
*  Connect ...
*  Mic Vin    <--> ESP32-C6: 3.3V
*  Mic GND    <--> ESP32-C6: GND
*  Mic Data   <--> ESP32-C6: GPIO13
*  Mic BCLK   <--> ESP32-C6: GPIO2
*  Mic L/R    <--> ESP32-C6: GPIO15
* 
*  GitHub ref: https://github.com/Interaktive-Medien/im_physical_computing
**************************************************************/

#include <driver/i2s.h>  // Include ESP32 I2S driver for microphone input

#define I2S_WS 15         // I2S word select pin (L/R clock)
#define I2S_SD 13         // I2S serial data input (from microphone)
#define I2S_SCK 2         // I2S bit clock pin
#define I2S_PORT I2S_NUM_0
#define bufferLen 64

int16_t sBuffer[bufferLen];  // Buffer for storing audio samples from I2S

void setup() {
  Serial.begin(115200);  // Initialize serial communication for debug output
  Serial.println("Setup I2S ...");

  delay(1000);
  i2s_install();   // Configure and install I2S driver
  i2s_setpin();    // Set I2S pins
  i2s_start(I2S_PORT);  // Start I2S driver
  delay(500);
}

void loop() {
  size_t bytesIn = 0;
  // Read raw audio data from the microphone into the buffer
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);
  if (result == ESP_OK) {
    int samples_read = bytesIn / 8;  // Number of 16-bit samples read (each sample 2 bytes)
    if (samples_read > 0) {
      float mean = 0;  // Mean value of all samples
      for (int i = 0; i < samples_read; ++i) {
        mean += (sBuffer[i]);
      }
      mean /= samples_read;  // Compute average amplitude
      Serial.println(mean);  // Output result to serial monitor
    }
  }

  delay(500);  // Wait between reads
}

void i2s_install() {
  // I2S configuration structure
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),  // Set as master and receiver
    .sample_rate = 44100,                               // Sampling rate 44.1 kHz
    .bits_per_sample = i2s_bits_per_sample_t(16),       // 16-bit audio
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        // Mono left channel
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = bufferLen,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);  // Apply the configuration
}

void i2s_setpin() {
  // Define which GPIOs are used for I2S
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,      // Bit Clock
    .ws_io_num = I2S_WS,        // Word Select
    .data_out_num = -1,         // We only receive, not transmit
    .data_in_num = I2S_SD       // Data in from mic
  };

  i2s_set_pin(I2S_PORT, &pin_config);  // Apply the pin mapping
}