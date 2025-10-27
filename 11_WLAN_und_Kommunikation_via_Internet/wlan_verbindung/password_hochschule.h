/**********************************************************************************************************
 * password_hochschule.h
 * Hier befinden sich die Zugangsdaten für das WPA2-Enterprise-verschlüsselte Netzwerk, zB. eduroam an der Hochschule
 * Diese Datei ist geheim und muss in .gitignore aufgenommen werden
 **********************************************************************************************************/ 


#pragma once

// Werte werden eingebunden in connect_hochschule.h

#define EAP_IDENTITY "fiessjan@fhgr.ch"    
const char *ssid = "eduroam";
const char *EAP_PASSWORD = "mypassword";