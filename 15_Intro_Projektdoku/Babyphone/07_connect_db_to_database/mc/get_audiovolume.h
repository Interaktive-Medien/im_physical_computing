#include <driver/i2s.h>
#include <math.h>

// --- PIN CONFIGURATION ---
#define I2S_WS          23
#define I2S_SD          13
#define I2S_SCK         2
#define I2S_PORT        I2S_NUM_0

// --- AUDIO SETTINGS ---
#define SAMPLE_RATE     16000
#define BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_32BIT // INMP441 liefert 24 Bit in 32-Bit Slots

// Buffer SETTINGS
const int DMA_BUF_COUNT = 8;
const int DMA_BUF_LEN   = 1024;
const int BUFFER_SIZE   = 512; 
int32_t samples[BUFFER_SIZE]; // Speicher für gelesene Rohdaten

// smoothing measurement values
float smoothedSPL = 0;
const float filterFactor = 0.1; // 0.1 bedeutet: 10% neuer Wert, 90% alter Wert


// calibration
const float AUDIOVOLUME_OFFSET = 122.0; // shift value, compare result with a "real" DB tester

void setup_audiovolume_tester(){
  // 1. I2S driver configuration
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // Master-Modus, Empfangen
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = BITS_PER_SAMPLE,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,         // INMP441 ist Mono
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = DMA_BUF_COUNT,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false
  };

  // 2. Pins konfigurieren
  i2s_pin_config_t pin_config = {
    .mck_io_num = I2S_PIN_NO_CHANGE,
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  // 3. Treiber installieren und starten
  if (i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL) != ESP_OK) {
    Serial.println("I2S Installation fehlgeschlagen!");
    return;
  }
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);
}

float get_audiovolume() {
  size_t bytesRead = 0;
  
  // Audiodaten vom I2S Port lesen
  esp_err_t result = i2s_read(I2S_PORT, &samples, sizeof(samples), &bytesRead, portMAX_DELAY);

  if (result == ESP_OK && bytesRead > 0) {
    int samplesCount = bytesRead / 4; // Da 32 Bit = 4 Bytes
    float sumSq = 0;

    for (int i = 0; i < samplesCount; i++) {
      // WICHTIG: Bit-Shift für INMP441 (Datenkorrektur)
      // Das Mikrofon liefert 24-Bit-Daten innerhalb eines 32-Bit-Werts
      int32_t val = samples[i] >> 8; 
      
      // Normieren auf Bereich -1.0 bis 1.0
      float floatSample = (float)val / 8388608.0; 
      sumSq += (floatSample * floatSample);
    }

    // RMS (Effektivwert) berechnen
    float rms = sqrt(sumSq / samplesCount);
    
    // Dezibel berechnen (dBFS = Dezibel relativ zur Vollaussteuerung)
    // Dieser Wert ist immer negativ (0 = extrem laut, -60 = leise)
    float db = 20.0 * log10(rms + 1e-9); // 1e-9 verhindert log(0)

    // Auf echten Schalldruckpegel (SPL) mappen
    float spl = db + AUDIOVOLUME_OFFSET;

    if (smoothedSPL == 0) {
    smoothedSPL = spl; // Initialisierung beim ersten Durchlauf
    } else {
    // Die Glättungs-Formel
        smoothedSPL = (spl * filterFactor) + (smoothedSPL * (1.0 - filterFactor));
    }

    return smoothedSPL;
  }
  
  // Kurze Pause, um den Serial-Buffer nicht zu fluten
  delay(10);
}