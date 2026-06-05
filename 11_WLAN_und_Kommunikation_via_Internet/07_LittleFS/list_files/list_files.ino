/******************************************************************************************************
 * LittleFS Recursive Directory Listing
 * Schaut tief in alle Unterordner (wie /js und /css) und listet jede einzelne Datei auf.
 ******************************************************************************************************/

#include <Arduino.h>
#include "FS.h"
#include "LittleFS.h"

// =========================================================================
// FUNKTION: Durchsucht das Dateisystem rekursiv nach Ordnern und Dateien
// =========================================================================
void listUploadedFiles(const char * dirname, uint8_t levels) {
  // Beim allerersten Aufruf drucken wir den Header
  if (strcmp(dirname, "/") == 0) {
    Serial.println("\n=========================================");
    Serial.printf("📂 Rekursive Verzeichnisauflistung für: %s\n", dirname);
    Serial.println("=========================================");
  }

  File root = LittleFS.open(dirname);
  if (!root) {
    Serial.printf("❌ Fehler beim Öffnen von: %s\n", dirname);
    return;
  }
  if (!root.isDirectory()) {
    return;
  }

  File file = root.openNextFile();
  
  while (file) {
    if (file.isDirectory()) {
      // Wenn es ein Ordner ist, zeigen wir ihn an...
      Serial.printf("  📁 [DIR]  %s\n", file.path());
      
      // ...und tauchen tiefer hinein, falls wir noch "levels" übrig haben
      if (levels > 0) {
        listUploadedFiles(file.path(), levels - 1);
      }
    } else {
      // Wenn es eine Datei ist, Pfad und Größe ausgeben
      Serial.printf("  📄 %-30s \tGröße: %d Bytes\n", file.path(), file.size());
    }
    file = root.openNextFile();
  }

  if (strcmp(dirname, "/") == 0) {
    Serial.println("=========================================\n");
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Pause für den Seriellen Monitor
  
  Serial.println("Initialisiere LittleFS...");

  if (!LittleFS.begin(true)) {
    Serial.println("❌ LittleFS Mount fehlgeschlagen!");
    return;
  }
  Serial.println("✔ LittleFS erfolgreich geladen.");

  // Ruft die Funktion auf. Die "2" bestimmt, wie viele Ordnerebenen tief 
  // das Programm maximal suchen soll (reicht locker für /css/ und /js/).
  listUploadedFiles("/", 2);
}

void loop() {
  // Nichts zu tun im Haupt-Loop
}