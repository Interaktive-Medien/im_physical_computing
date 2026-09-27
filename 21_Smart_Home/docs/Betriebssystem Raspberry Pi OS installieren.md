# Installation Raspberry Pi OS

### 1. Vorbereitung

- Lade den Raspberry Pi Imager herunter
  [https://www.raspberrypi.com/software/](https://www.raspberrypi.com/software/)
- Verbinde Micro SD Karte mit dem Computer

### 2. Modell auswählen

![Modell auswählen](img/installation_01.jpg)

### 3. Betriebssystem auswählen

Wähle die 64-bit-Variante

![Betriebssystem auswählen](img/installation_02.jpg)

### 4. SD Karte auswählen

![Wähle Rpi Modell](img/installation_03.jpg)

### 5. Hostname festlegen

Unter diesem Namen ist der Raspberry später im Netzwerk zu finden. Stattdessen kann zur Identifizierung auch die IP-Adresse verwendet werden.
_jan = z.B. 192.168.0.67_

Beachte: Im Hochschulnetzwerk _eduroam_ wird der Zugriff auf den Rapberry Pi per SSH verhindert.
Im Netzwerk _MMP_MediaApp_ im Medienhaus funktioniert der Zugriff über SSH zwar, aber statt Hostname muss bisher die IP-Adresse des Raspberry Pi angegeben werden. Diese kann über einen Netzwerkscanner herausgefunden werden -> siehe weiter unten.

![Hostname festlegen](img/installation_04.jpg)

### 6. Lokalisierung

![Lokalisierung](img/installation_05.jpg)

### 7. Benutzername / Passwort festlegen

Nimm für den Anfang bitte...

- Benutzername: **pi**
- Passwort: **raspberry**

![Anmelde-Credentials](img/installation_06.jpg)

### 8. WLAN-Verbindung speichern

Dieser Schritt ermöglicht uns, von Anfang an remote auf den Raspberry Pi zugreifen zu können. Er meldet sich automatisch im WLAN-Netzwerk an und man muss nicht erst mühsam Display, Maus und Tastatur anschliessen, um dies zu tun.

![WLAN speichern](img/installation_07.jpg)

### 9. Authentifizierung via SSH aktivieren

Dies ist nötig, damit man später remnote von einem Computer mit Tastur, Maus und Display auf dem Raspberry Pi arbeiten kann, ohne dass man ihm ebenfalls Tastur, Maus und Display geben muss.

![SSH erlauben](img/installation_08.jpg)

### 10. Screen Share

Mit Raspberry Pi Connect kann man sich online von einem beliebigen Computer, auch ausserhalb des Heimnetzwerks, mit dem Rpi verbinden. Besonders interessant ist der Strem des grafischen Displays.

![Screen Share](img/installation_09.jpg)

Aktiviere.

![Raspberry Pi Connect anmelden](img/installation_10.jpg)

Sobald man zum Installationsagenten zurück geleitet wird, wird der Verbindungstoken automatisch eingetragen.

![Rpi Connect Token](img/installation_11.jpg)

### 11. Betriebssystem auf die SD Karte laden

![Auf SD Karte schreiben 1](img/installation_12.jpg)

![Auf SD Karte schreiben 2](img/installation_13.jpg)

![Auf SD Karte schreiben 3](img/installation_14.jpg)

### 12. Inbetriebnahme

- Stecke die frische Micro SD Karte in den Rpi
- Gib ihm Strom (Achtung: 5A-Netzteil nötig)
- Rpi fährt automatisch hoch - warte paar Minuten

### 13. Verbinde mit GUI

![Verbinde mit GUI 1](img/installation_15.jpg)

![Verbinde mit GUI 2](img/installation_16.jpg)

![Verbinde mit GUI 3](img/installation_17.jpg)

- Terminal öffnen

![Terminal öffnen](img/installation_18.jpg)

### 14. Remote-Zugriff per SSH

- In diesem Guide müssen dein Arbeitscomputer und der Rpi im selben Netzwerk sein.

![Netzwerk wählen](img/installation_19.jpg)

- Öffne _Terminal_ am Mac oder _CMD_ bei Windows bzw. nutze den eingebauten Terminal einer IDE, z.B. in _VS Code_.

![Terminal](img/installation_20.jpg)

- mit dem Rpi verbinden:

```
ssh [username]@[hostname]
```

Beispiel:

```
ssh pi@jan
```

Falls das Netzwerk keine hostnames akzeptiert, brauchst du die IP Adresse des Rpi. Diese lässt sich komfortabel z.B. über eine Netzwerkscanner-App herausfinden, wie z.B _iNet_.

![Netzwerkscanner](img/installation_21.jpg)

In diesem Fall funktioniert die Anmeldung per SSH wie folgt:

```
ssh [username]@[ip address]
```

Beispiel:

```
ssh pi@192.168.0.67
```

- Danach alles mit "yes" akzeptieren
- Passwort eingeben

Dann kannst du von remote mit dem Rpi arbeiten.
