# Kap. 13: Website ➜ DB
## Schritt 2: Formular ➜ Datenbank

Wir erstellen zunächst ein Formular. 
Die Inhalte werden in eine Datenbanktabelle geschrieben, sobald es abgeschickt wird.
Dies ist innerhalb einer einzigen PHP-Datei möglich, die server-seitige und client-seitige Teile (HTML) enthält.

![File_relations](File_relations.jpg)

1. Lade von folgende Dateien herunter: 
   * `website_form.php`
   * `db_config.php_template` 
2. Verstehe den Code
3. Benenne die Datei `db_config.php_template` um zu `db_config.php`.
4. Gebe die Zugangsdaten zu deiner Datenbank ein in `db_config.php` und achte darauf, dass diese Datei nicht auf GitHub landet (!!!)
5. Lade beide Dateien per FTP auf dein Online-Dateiverzeichnis auf Infomaniak (z. B. mit FileZilla).
6. Rufe den URL auf, über den `website_form.php` (Web-Formular) erreichbar ist und gebe einige Werte ein 
7. Überprüfe via PHPmyAdmin, ob die Werte in die Datenbanktabelle geschrieben wurden.
