Jonathan Swerdlow
Andrews University
ENGR385 - Microprocessors
Final Project

This program is designed for the ESP32 with a 5V power supply for best performance.


Included are the following modules:
-Flappy Pong    A small game which uses a single button. Avoid the ball to increase the score. Game speeds up over time.
-BT             TODO: A generic two-way bluetooth serial interface.



The structure of this program is as follows:

preamble.h  An include-once header to define core project dependencies/configurations
main.cpp    Serves as a psuedo-OS (pOS)
gtft.h      A clunky workaround for Adafruit's TFT library (see gtft.h)
modules.h   Includes and forward declarations to be used by pOS in interacting with applications

module_xyz.cpp: An application handled by the pOS.
    Implementation includes a setup function and loop function as defined by the respective header.
    modules are expected to handle BUTTON_EXIT calls. Generally, this means returning from the module loop.




Notes:
-The structure of this application could be improved greatly, namely refactoring gtft to be truly global.
-This application's primary purpose is to serve as a proof-of-concept for the included features.