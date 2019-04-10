#include "preamble.h"
#include "module_server.h"
#include "gtft.h"

#include "WiFi.h"

/*  
    WPA2 Enterprise flag
    WARNING: Due to needing "esp_wpa2.h", using WPA2 enterprise will exceed the ESP32's program storage capacity. (At 98.5% without including it)
*/
//#define EAP

#ifndef EAP
    const char* ssid = "GPHS";          // Your SSID goes here
    const char* password = "1234abcd";  // SSID password goes here
#else
    #include "esp_wpa2.h"
    const char* ssid = "WPA2 Enterprise SSID";
    #define EAP_ID "donotchangethis@andrews.edu"  // Anonymous Identity (see https://sites.google.com/site/amitsciscozone/home/switching/peap---protected-eap-protocol)
    #define EAP_USERNAME "username"               // Username 
    #define EAP_PASSWORD "password"               // Password
#endif


String dr = "\n\n\n\n\n\n"; //display returns

WiFiServer server(80);

bool should_quit = false;

void clear_status();
bool check_should_quit();

void setup_server() {
    tft().fillScreen(ST7735_BLACK);
    tft().println("Connecting to: \n" + (String) ssid);

#ifndef EAP
    WiFi.begin(ssid, password);
#else
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ID, strlen(EAP_ID));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));

    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);

    WiFi.begin(ssid);
#endif

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
            WiFi.disconnect();
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
                            // and a content-type so the client knows what to expect
                            // finally, a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();

                            // HTTP response content follows the header:
                            client.println("ESP32 WiFi Server<br>");
                            client.println("-----------------<br>");
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