#include "preamble.h"
#include "module_server.h"
#include "gtft.h"

#include "WiFi.h"

void sv_setup() {
    tft().fillScreen(ST7735_BLACK);
    tft().println("Server not implemented");

    //TODO: Start server and display IP
}

//TODO: Read from socket and toggle LED or print a message or accept draw commands, etc.
void sv_loop() {
    while(true) {
        if (digitalRead(BUTTON_RETURN) == LOW) {
            Serial.println("Quitting Server");
            draw_setup();
            return;
        }
    }
}