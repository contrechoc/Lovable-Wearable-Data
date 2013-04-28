Lovable-Wearable-Data
=====================

Scripts used in the vest made for the NASA Challenge

There are a lot of small scripts used in the chips of this vest.

Raspberry Pi: (WIFI internet connection)
  Python script to fetch data and send it using UART to an atmega328 with RF12 transceiver
  Python script for interaction with a wii-remote
  
In a separate folder  
Atmega328 (a) with RF12 transceiver receives data from Raspberry Pi using UART and sends the data wireless
  script raspberryUART

Atmega328 (b) with RF12 transceiver receives data wireless from Atmega328 (a)
  script to send text to 5 Liquid Crystal screens in the vest
	life_jacket2

In a separate folder:
Atmega328 (c) UART connection with Atmega328 (b) both inside vest
  script to receive IR data and send UART codes to 5 Attiny2313's in the vest
  
In a separat folder:
The scripts of the page on the internet where you can adopt an exoplanet:
http://www.contrechoc.com/exovest/adopt_exoplanet.php?indication=habitable
