/*
    The code in this file is based on the following example:
    https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino
*/

#include "preamble.h"
#include "module_server.h"
#include "gtft.h"

#include "WiFi.h"

const char* ssid = "ssid goes here";
const char* password = "wifi password goes here";


String dr = "\n\n\n\n\n\n"; //display returns

WiFiServer server(80);

bool should_quit = false;

void clear_status();
bool check_should_quit();

void setup_server() {
    tft().fillScreen(ST7735_BLACK);
    tft().println("Connecting to: \n" + (String) ssid);

    WiFi.begin(ssid, password);

    String dots = "";
    while (WiFi.status() != WL_CONNECTED) {
        if (check_should_quit()) {
            return;
        }

        delay(400);
        dots += ".";
        if (dots.equals("....")) {
            dots = ".";
        }
        tft().fillRect(0, CHAR_HEIGHT*3, WIDTH, CHAR_HEIGHT, ST7735_BLACK);
        tft().println("\n\n\n" + dots);
    }

    tft().fillRect(0, CHAR_HEIGHT*3, WIDTH, CHAR_HEIGHT, ST7735_BLACK);

    tft().println("\n\n\nConnected to WiFi.");
    tft().println("\n\n\n\nIP address:");
    tft().println("\n\n\n\n\n" + WiFi.localIP().toString());

    Serial.print("Server available at: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void run_server() {
    Serial.println("Starting Server");
    setup_server();

    while(true) {
        if (check_should_quit()) {
            Serial.println("Quitting Server");
            server.stop();
            draw_setup();
            return;
        }

        WiFiClient client = server.available(); // Listen for client

        if (client) { // If there is a client
            tft().println(dr + "\nClient connected");

            String currentLine = "";

            while(client.connected()) { // While that client is still connected
                if (client.available()) { // If the client has sent data
                    char data = client.read(); // Read incoming byte
                    if (data == '\n') {
                        if (currentLine.length() == 0) { // HTTP request has ended
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();

                            // the content of the HTTP response follows the header:
                            client.print("ESP32 WiFi Server");
                            client.print("");
                            client.print("LED <a href=\"/H\">on</a><br>");
                            client.print("LED <a href=\"/L\">off</a><br>");

                            // The HTTP response ends with another blank line:
                            client.println();
                            // break out of the while loop:
                            break;
                        } else {
                            currentLine = ""; // Clear byte buffer
                        }
                    } else if (data != '\r') {
                        currentLine += data; // Append byte
                    }

                    // Check request
                    if (currentLine.endsWith("GET /H")) {
                        digitalWrite(LED_BUILTIN, HIGH);
                        clear_status();
                        tft().println(dr + "\n\nLED ON");
                    }
                    if (currentLine.endsWith("GET /L")) {
                        digitalWrite(LED_BUILTIN, LOW);
                        clear_status();
                        tft().println(dr + "\n\nLED OFF");
                    }
                }
            }
            // Client has been served
            client.stop(); 
        }
    }
}

void clear_status() {
    tft().fillRect(0, CHAR_HEIGHT*6, WIDTH, CHAR_HEIGHT*3, ST7735_BLACK);
}

bool check_should_quit() {
        if (digitalRead(BUTTON_RETURN) == LOW) {
            should_quit = true;
        }
        return should_quit;
}