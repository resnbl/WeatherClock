#include "Panel.h"

Panel::Panel() {
	panel.setIntensity(PANEL_BRIGHTNESS);

  // Set individual 8x8 panels' rotation and order (right to left)
	int maxDisplay = PANEL_WIDTH * PANEL_HEIGHT;    // # 8x8 units
	for (int disp = 0; disp < maxDisplay; ++disp)
	{
		panel.setRotation(disp, 3);						    // 90 degrees CCW
		panel.setPosition(disp, maxDisplay - disp - 1, 0); // map displays to logical pos
	}
	panel.fillScreen(LOW); // clear screen
	panel.write();
}

void Panel::centerText(String msg) {
	int msgWidth = getTextWidth(msg);
	int x = (panel.width() - msgWidth) / 2;

  if (x < 0) x = 0;		// use first X chars if text too long
	panel.fillScreen(LOW); // clear screen first
	panel.setCursor(x, 0); // char top along top of display
	panel.print(msg);
	panel.write();
}

void Panel::scrollText(String msg) {
	// Compute width of message in pixels, padded by width of display so it scrolls off
	int pixelWidth = getTextWidth(msg) + panel.width() - SPACER;
  
	for (int pixelNum = 0; pixelNum < pixelWidth; pixelNum++)
	{
		panel.fillScreen(LOW); // clear screen

		int charPos = pixelNum / CHAR_WIDTH;
		int x = (panel.width() - 1) - pixelNum % CHAR_WIDTH;
		int y = (panel.height() - 8) / 2; // center text vertically

		while (x + CHAR_WIDTH - SPACER >= 0 && charPos >= 0)
		{
			if (charPos < (signed)msg.length())
			{
				panel.drawChar(x, y, msg[charPos], HIGH, LOW, 1);
			}

			charPos--;
			x -= CHAR_WIDTH;
		}

		panel.write(); // send bitmap to display
		delay(SCROLL_DELAY_MS);
	}
}

void Panel::scrollTextUp(String msg) {
	int msgWidth = getTextWidth(msg);
	int x = (panel.width() - msgWidth) / 2;

	if (x < 0) x = 0;		// use first chars if text too long
	for (int y = panel.height() - 1; y >= 0; --y) {
		panel.fillScreen(LOW); 	// clear screen first
		panel.setCursor(x, y); 	// baseline starts below display
		panel.print(msg);
		panel.write();
		delay(SCROLL_DELAY_MS * 2);		// vertical scroll rate < horizontal rate
	}
}

void Panel::scrollTextDown(String msg) {
	int msgWidth = getTextWidth(msg);
	int x = (panel.width() - msgWidth) / 2;

	if (x < 0) x = 0;		// use first chars if text too long
	for (int y = 1 - panel.height(); y <= 0; ++y) {
		panel.fillScreen(LOW); 	// clear screen first
		panel.setCursor(x, y); 	// baseline starts above display
		panel.print(msg);
		panel.write();
		delay(SCROLL_DELAY_MS * 2);		// vertical scroll rate < horizontal rate
	}
}

int Panel::getTextWidth(String txt) {
	return txt.length() * CHAR_WIDTH;   // built-in font is fixed width
}
