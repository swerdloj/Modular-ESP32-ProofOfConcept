#include "preamble.h"
#include "module_bt.h"
#include "gtft.h"

#include "BluetoothSerial.h"

const int MSG_LED_ON = 1;
const int MSG_LED_OFF = 0;

const String DEVICE_NAME = "ESP_BT";

// Bluetooth object
BluetoothSerial ESP_BT;
int msg;

void handle_msg(int);
void draw_status(String);

void bt_setup() {
    ESP_BT.begin(DEVICE_NAME); // Set device name
    Serial.println("BT device ready for pairing");

    pinMode(LED_BUILTIN, OUTPUT);
    tft().fillScreen(ST7735_BLACK);
    tft().println("BT Device Name: \n" + DEVICE_NAME);
}

void bt_loop() {
    while(true) {
        if (digitalRead(BUTTON_RETURN) == LOW) {
            ESP_BT.end(); // Stop BT

            draw_setup();
            return;
        }

        if (ESP_BT.available()) {
            msg = ESP_BT.read();
            Serial.print("BT message: "); Serial.println(msg);
            handle_msg(msg);
        }
    }
}

void draw_status(String status) {
    tft().fillRect(0, CHAR_HEIGHT*2, WIDTH, HEIGHT-CHAR_HEIGHT*2, ST7735_BLACK);
    tft().println("\n\n\n" + status);
}

// TODO: Draw valid messages--maybe at the bottom of the screen

// NOTE: Data may be received as ASCII. Ex: sending "1" will actually send 'int 49'
// Ensure that data is sent in raw (HEX)
void handle_msg(int msg) {
    switch (msg) {
        case(MSG_LED_ON):
            digitalWrite(LED_BUILTIN, HIGH);
            draw_status("LED ON");
            ESP_BT.println("Turning LED on");
            break;
        case(MSG_LED_OFF):
            digitalWrite(LED_BUILTIN, LOW);
            draw_status("LED OFF");
            ESP_BT.println("Turning LED off");
            break;
        default:
            draw_status("Bad message: " + (String) msg);
            ESP_BT.println("Message not implemented");
    }
}