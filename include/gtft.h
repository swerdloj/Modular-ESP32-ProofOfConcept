/*

Adafruit library doesn't allow the use of "=".
It also forces instantiation as opposed to declaration.

So this mess is required to share the tft resource

*/

#pragma once

#include <Adafruit_ST7735.h>


Adafruit_ST7735 tft();