/*

Global TFT "reference"

See gtft.h for more information


TODO: Implement cursor & dimension functions here

*/

#include "preamble.h"
#include "gtft.h"

Adafruit_ST7735 gtft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

Adafruit_ST7735 tft() {
    return gtft;
}