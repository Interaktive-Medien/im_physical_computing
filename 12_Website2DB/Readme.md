# Kap. 12: Website ➜ DB

[Videokurs: 12_Website_to_DB](https://youtu.be/qyGE9ZJ8ang)

Während es im vorherigen Kapitel nur das lokale Netzwerk gebraucht hat, geht es nun in die grosse weite vernetzte Welt hinaus.
Ziel ist, dass in Kap. 13 die eigenen Sensordaten in eine Datenbank geladen werden. In Kap. 14 sollen die Daten visualisiert werden.
Dieser Workflow wird zunächst in einem bekannten Umfeld erprobt - mit Websites. Das ist zwar schon bekannt, aber eine Wiederholung schadet nicht. Denn mit dem Mikrocontroller funktioniert es genau gleich wie mit Websites. Sowohl die Website als auch der Mikrocontroller chatten mit dem Server per HTTP. Es sollen die Parallelen aufgezeigt werden.
Konkret werden hier Daten von einer Website an den Server geschickt, der sie in eine Datenbank schreibt.

Der Prozess wird in den Unterverzeichnissen Schritt für Schritt vorgestellt:

- Schritt 1: Datenbanktabelle anlegen
- Schritt 2: Website ➜ Datenbank (HTTP-Chat: je ein Sender (JS) und ein Receiver (PHP). So wird es auch in Kap. 13 mit dem Mikrocontroller gemacht)
