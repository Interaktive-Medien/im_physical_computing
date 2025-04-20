# Kap. 17: Batteriebetrieb

Wenn der Datenaustausch schon drahtlos erfolgt, ist es praktisch, wenn auch die Stromversorgung drahtlos ist.
Batterie-Optionen gibt es genügend.
Doch muss dafür der Microcontroller immer komplett an sein?
Eine Messstation liefert meistens nicht paermanent Daten. In den Wartezeiten kann der Microcontroller in den Deepsleep Modus versetzt werden. Energiehungrige Aktivitäten wie Bluietooth und WLAN werden dabei ausgeschaltet.
Das steigert die Akku-Dauer gewaltig.
Dieses Kapitel gibt eine praktische Einführung in den Deepsleep-Modus.

Anbei eine Anleitung zum Laden des Akkus:
![Akku_laden](Akku_laden.png)

Anbei eine Schaltung für's Laden bei gleichzeitigem Betrieb des Microcontrollers:
![Akku_laden_plus_Betrieb](Akku_laden_plus_Betrieb.png)

Anbei zusätzlich mit Spannungsregler auf 3.3V (ptimal für den ESP32-C6):
![Akku_laden_plus_Betrieb](Akku_laden_plus_Betrieb_plus_Spannungswandler.png)