# Modular ESP32 Proof of Concept

#### Disclaimer: This code base is neither efficient nor optimized

### ENGR385 - Microprocessors
### Final Project

This program is designed for the ESP32 with a 5V power supply for best performance.


### Included are the following modules:
-Flappy Pong    Small game which uses a single button. Avoid the ball to increase the score. Game speeds up over time.  
-Bluetooth      Generic two-way bluetooth serial interface.  
-Server         Minimal WiFi server on local address designed for use with SSID + Password (see module_server.cpp for enterprise info).  

Bluetooth and Server can be easily expanded for greater functionality.


### The structure of this program is as follows:

preamble.h: An include-once header to define core project dependencies/configurations across code base.  
main.cpp: Serves as a psuedo-OS (pOS). Handles control flow and main menu.  
gtft.h: A clunky workaround for Adafruit's TFT library (see gtft.h). I would improve this if I had a better understanding of C++.  
modules.h: Simple multi-module include. 

module_xyz.cpp: An application handled by the pOS.  
-Implementation includes a setup function and loop function as defined by the respective header.  
-Modules are expected to handle BUTTON_RETURN events. Generally, this means returning from the module loop.  



## Notes:
-Memory limitations mean this program is limited in functionality. This program currently uses 99% of available program memory.  
-The structure of this application could be improved greatly, namely refactoring gtft to be truly global.  
-This application's primary purpose is to serve as a proof-of-concept for the included features.  
-The focus while developing this program is modularity--to put a lot of mini-programs onto the ESP32 and run them all.
-There may be a better way of loading modules such as from storage rather than memory.
