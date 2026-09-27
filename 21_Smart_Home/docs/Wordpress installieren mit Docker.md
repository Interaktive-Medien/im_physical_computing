# WordPress Docker-Setup auf dem Raspberry Pi

### ohne docker-compose

Diese Anleitung führt Schritt für Schritt durch die manuelle Installation von Docker und das Starten einer vollständigen WordPress-Umgebung (inkl. MariaDB und Adminer) über einzelne Terminal-Befehle.

### 1. Docker installieren

Das offizielle Skript über eine temporäre Datei laden und ausführen. Dieser Schritt kann einige Minuten in Anspruch nehmen. Kopiere den gesamten Block und führe ihn im Terminal aus:

```bash
TMP_DOCKER_SCRIPT=$(mktemp)
curl -fsSL [https://get.docker.com](https://get.docker.com) -o "$TMP_DOCKER_SCRIPT"
sudo sh "$TMP_DOCKER_SCRIPT"
sudo rm -f "$TMP_DOCKER_SCRIPT"
sudo apt-get install -y docker-compose-plugin
sudo systemctl enable docker
sudo systemctl start docker
```

### 2. Rechte vergeben

Damit ab sofort kein sudo mehr vor jedem Docker-Befehl nötig ist, wird der aktuelle Nutzer der Docker-Gruppe hinzugefügt. Die Änderung wird direkt für das aktuelle Terminalfenster übernommen:

```bash
sudo usermod -aG docker $USER
newgrp docker
```

### 3. Infrastruktur anlegen

Erstelle ein virtuelles Netzwerk, damit die Container untereinander kommunizieren können, sowie zwei Volumes für die dauerhafte Speicherung der Datenbank und der Website-Dateien:

```bash
docker network create wp-netzwerk
docker volume create wp_db_data
docker volume create wp_web_data
```

### 4. MariaDB starten

Starte den Datenbank-Container. Der Name wp_mariadb dient später als Hostname für WordPress.

```bash
docker run -d \
 --name wp_mariadb \
 --network wp-netzwerk \
 -v wp_db_data:/var/lib/mysql \
 -e MYSQL_ROOT_PASSWORD=geheim_root \
 -e MYSQL_DATABASE=wordpress \
 -e MYSQL_USER=wp_user \
 -e MYSQL_PASSWORD=geheim_user \
 mariadb:lts
```

### 5. WordPress starten

Starte den eigentlichen Webserver inklusive PHP. Der Port 80 des Containers wird auf den Port 8080 des Raspberry Pi weitergeleitet.

```bash
docker run -d \
 --name wp_core \
 --network wp-netzwerk \
 -p 8080:80 \
 -v wp_web_data:/var/www/html \
 -e WORDPRESS_DB_HOST=wp_mariadb:3306 \
 -e WORDPRESS_DB_USER=wp_user \
 -e WORDPRESS_DB_PASSWORD=geheim_user \
 -e WORDPRESS_DB_NAME=wordpress \
 wordpress:latest
```

### 6. Adminer starten (Optional)

Starte das leichtgewichtige Datenbank-Management-Tool auf Port 8081.

```bash
docker run -d \
 --name wp_adminer \
 --network wp-netzwerk \
 -p 8081:8080 \
 adminer:latest
```

### Aufruf der Dienste im Browser:

- WordPress: http://:8080
- Adminer: http://:8081
  Eingeben ...
  - Server: wp_mariadb
  - Benutzer: root
  - Passwort: geheim_root
  - Datenbank: wordpress
