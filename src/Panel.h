#ifndef _PANEL_H_
#define _PANEL_H_

#include "config.h"
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

/*  This class implements simple operations on a Max7219 LED
    panel in the default 4x1 (32x8 pixels) orientation.
    It uses the Max72xxPanel library from:
      https://github.com/markruys/arduino-Max72xxPanel
    which is a simplistic, but light-weight library.
 */

#define SPACER 1                // pixels between chars
#define CHAR_WIDTH (5 + SPACER) // 5x7 chars
#define SCROLL_DELAY_MS 30

class Panel {
  public:
    Panel();
    // Display text centered on panel
    void centerText(String msg);
    // Scroll text from left to right
    void scrollText(String msg);
    // Scroll text from bottom up
    void scrollTextUp(String msg);
    // Scroll text from top down
    void scrollTextDown(String msg);

  protected:
    Max72xxPanel panel = Max72xxPanel(PANEL_CS, PANEL_WIDTH, PANEL_HEIGHT);

    // Return text width in pixels
    int getTextWidth(String txt);
};

#endif
