# Raspberry Pi Filesystem in VS Code

Vorteile:

- File Operations ganz ohne Unix Comands.
- Dateien editieren in einem anständigen Editor mit KI, Syntax-Highlighting und Maus

## Schritt 1: Remote-Erweiterung in VS Code installieren

- Öffne VS Code.
- Installiere die Extension `Remote - SSH` (von Microsoft).

## Schritt 2: SSH-Konfiguration anlegen

- Drücke in VS Code die Tastenkombination `Cmd + Shift + P`, um die Befehlspalette zu öffnen.
- Wähle `Remote-SSH: Open SSH Configuration File...`
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH1.jpg)
- Wähle die vorgeschlagene Datei aus (bei Mac meistens `/Users/[dein_benutzername]/.ssh/config`).
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH2.jpg)
- In der Datei muss stehen:

  ```
  Host SomeFunnyName
    HostName your_hostname
    User your_username
  ```

  Beispiel:

  ```
  Host janpi
    HostName jan
    User pi
  ```

  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH3.jpg)

- Speichere die Datei und schliesse sie.

## Schritt 3: Mit dem Raspberry Pi verbinden

- Wähle `CMD + P`und dann `Remote-SSH: Connect to Host...`
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH4.jpg)
- Wähle deinen Server:
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH5.jpg)
  Alternativ kann man auch den Pfeil drücken:
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH6.jpg)
- Gib das Passwort ein, z.B. `raspberry`.
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH7.jpg)
- Links ist nun das Dateiverzeichnis zu sehen. Dazu musst du in VS Code den Explorer angewählt haben (`Cmd + Shift + E`). Rechts kann man die Dateien komfortabel bearbeiten.
  ![SSH-Verbindeung herstellen in VS Code](img/Remote-SSH8.jpg)

Geschafft! über `Cmd + J`kannst du das Terminal öffnen, welches direkt auf dem RPi läuft.
Klicke
