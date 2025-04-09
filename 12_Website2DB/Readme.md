# Kap. 12: Website ➜ DB

Während es im vorherigen Kapitel nur das lokale Netzwerk gebraucht hat, geht es nun in die grosse weite vernetzte Welt hinaus.
Ziel ist, dass in Kap. 13 die eigenen Sensordaten in eine Datenbank geladen werden. In Kap. 14 sollen die Daten visualisiert werden.
Dieser Workflow wird zunächst in einem bekannten Umfeld erprobt - mit Websites. Das ist zwar schon bekannt, aber eine Wiederholung schadet nicht. Denn mit dem Microcontroller funktioniert es genauso gleich wie mit Websites. Sowohl die Website als auch der Microcontroller chatten mit dem Server per HTTP. Es sollen die Parallelen aufgezeigt werden.
Konkret werden hier Daten von einer Website an den Server geschickt, der sie in eine Datenbank schreibt.

Der Prozess wird in den Unterverzeichnissen Schritt für Schritt vorgestellt:

* Schritt 1: Datenbanktabelle anlegen
* Schritt 2: Formular ➜ Datenbank (kombiniert in einer einzigen Datei)
* Schritt 3: Website ➜ Datenbank (Chat-Prinzip: in zwei separaten Dateien: Sender und Receiver - so wird es auch in Kap. 13 mit dem Microcontroller gemacht)