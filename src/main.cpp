#include "preamble.h"
#include "modules.h"
#include "gtft.h"

// Flag for whether a module was just exited (unused)
bool did_return = false;

int current_selection = 0;
const int NUM_OPTIONS = 3;
String options[] = {"Flappy Pong", "Bluetooth", "Server"};

int adjustment_last = 20; //Arbitrary number for initial cursor draw

void draw_menu(int);
void dispatch(int);

// Initial project setup
void setup() {
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON_RETURN, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);

    tft().initR(INITR_BLACKTAB);

    Serial.begin(115200);

    draw_setup();
}

void draw_setup() {
    current_selection = 0;
    adjustment_last = 20;

    tft().fillScreen(ST77XX_BLACK);
    draw_menu(current_selection);
}

// The "main menu"
void loop() {
    delay(200); // Allow holding down the button to cycle through menu

   if (digitalRead(BUTTON1) == LOW) { // Update selection
       if (++current_selection > NUM_OPTIONS-1) {
           current_selection = 0;
       }
       draw_menu(current_selection);
   }

   if (digitalRead(BUTTON2) == LOW) { // Make selection
       dispatch(current_selection);
   }
}

void draw_menu(int current) {
    //tft().setCursor(0, 0);

    String option_string = "";
    int option_lines = 0;

    for (int i = 0; i < NUM_OPTIONS; i++) {
        option_string += "\n\n  " + options[i];
        option_lines += 2;
    }

    tft().println("Main Menu.\nPress the right \nbutton to advance." + option_string);
    //Lines: 1+1+1 + 2(fp) + 2(bt)
    // fp = line 5, bt = line 7
    
    int adjustment = current * 2 * CHAR_HEIGHT - 5;

    tft().fillCircle(5, CHAR_HEIGHT*5 + adjustment, 2, ST7735_WHITE);
    tft().fillCircle(5, CHAR_HEIGHT*5 + adjustment_last, 2, ST7735_BLACK);

    adjustment_last = adjustment;
}

void dispatch(int item) {
    if (item == 0) {
        run_flappy_pong();
    } else if (item == 1) {
        run_bluetooth();
    } else if (item == 2) {
        run_server();
    } else {
        // error
    }
    did_return = true;
}