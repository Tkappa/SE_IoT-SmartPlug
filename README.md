# SE_IoT-SmartPlug
#Introduzione
E' stata realizzata una presa della corrente "Intelligente". Essa può essere controllata tramite dispositivi Bluetooth e tramite wifi. E' dotata inoltre di un lettore di corrente che informa l'utente del consumo.
Insieme al dispositivo vengono forniti anche l'apk per Android che permette l'interazione con il dispositivo e il server Node.js che permette la connettività tramite WIFI.  
#Arduino
Si è realizzato il main loop di arduino con un architettura a Task, i vari task vengono eseguiti in sequenza seguendo un ordine prefinito. Ogni Task viene eseguito dopo un certo periodo di tempo. Per realizzare questa funzionalità si è usato un Timer.
## Master Loop
Il loop principale viene eseguito con un periodo di base di 50ms. Questa quantità è stata scelta per garantire la responsivness nell'accensione della modalità bluetooth.  
FOTO LOOP  

## Classi generali
## Routine Device
### Routine
### Dispositivi utilizzati
## Routine Power
### Routine
### Dispositivi utilizzati
## Routine Wifi
### Routine
### Dispositivi 
## Routine BT
### Routine
### Dispositivi
## Routine UX
### Routine
### Dispositivi

## Altre difficoltà incontrate

# App android

# Server Node

# Conclusioni
## Possibilità di espansione

