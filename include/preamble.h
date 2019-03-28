/*
Contains core functionality shared across the project.
*/

#pragma once

#define PREAMBLE

/* Core libraries */
#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

/* Modularity Support */
#define TARGET "ESP32"
#define TFT_DRIVER ST7735

/* Pin layout */
#define TFT_CS 34
#define TFT_RST -1 // Internal Reset
#define TFT_DC 21
#define TFT_MOSI 23
#define TFT_SCLK 18

#define LED_BUILTIN 2 // ESP32 Internal LED pin

/* Button configurations */
#define BUTTON1 12
#define BUTTON2 13
#define BUTTON_RETURN 15 // Dedicated return to main menu

/* Font/Text properties */
#define CHAR_HEIGHT 8
#define CHAR_WIDTH 5

/* TFT Properties */
#define HEIGHT 160
#define WIDTH 128

void draw_setup(); // Main menu init when returning from application