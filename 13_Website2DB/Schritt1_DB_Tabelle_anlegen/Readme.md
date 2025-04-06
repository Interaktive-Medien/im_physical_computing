# Kap. 17: Website ➜ DB
## Schritt 1: Datenbanktabelle anlegen

Die Datenbank ist das Herz unserer Applikation. 

Lege die Tabelle `sensordata` mit 3 Tabellenspalten an
In unserem Minimalbesispiel speichern wir immer nur einen Wert in der Spalte `wert`.
Hier wird später der Sensorwert hineingeschrieben.
Zusätzlich erzeugen wir zwei weitere Tabellenspalten: `id` und `timestamp`. Diese werden von der Datenbank automatisch ausgefüllt, sobald ein neuer Wert gespeichert wird