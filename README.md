# WeatherClock

This is a simple program for the ESP8266 board (specifically, I used a
Wemos D1 mini board). It will display the time, date and weather conditions
on a 4x1 Max7219 LED display (32x8 pixels). At the "top of the minute"
(HH:MM:00), the date in format "Wkd Mon DD", is scrolled along the display.
On the "half-minute" (HH:MM:30) it scrolls the current temperature, humidity
and weather conditions fetched via the OpenWeatherMap API. Otherwise, the
current time in the format "HH:MM" 12-hour format is displayed, with the
colon flashing on/off every second.

## config.h(.org)

Please copy the `config.h.org` file to `config.h` before continuing.
This file contains all the definitions you should need to
customize this program for your situation. It includes parameters for
your local WiFi LAN and Max7219 connections.

You will also need to choose an NTP server pool name (usually in the form
`xx.ntp.pool.org` where `xx` is your country code) as well as one of the
supported time zones listed in `<TZ.h>`. For the PlatformIO IDE, this is found
in `.../.platformio/packages/framework-arduinoespressif8266/cores/esp8266`; for
the Arduino IDE it is under `.../Library/Arduino15/packages/esp8266/hardware/esp8266/2.6.2/cores/esp8266`.

You must register at https://openweather.org to receive an API key
and get the ID number for the city to request the weather about. These items
also need to be included here.

## IDE Notes

This code was originally written using the *PlatformIO* IDE. To use the
standard Arduino IDE, you'll need to make the following changes:

- copy the `src\` directory to another location with an appropriate project
name, such as "myWeatherClock".
- open the Arduino IDE and use the Library Manager to load the following
libraries (if you haven't already):

  - AdaFruit GFX Library
  - ArduinoJson (version 6 or later)

- the Max7219 library I used is not available through the Arduino Library
Manager. You will need to download it directly from
https://github.com/markruys/arduino-Max72xxPanel
into the `libs\` sub-directory in your project.

## Clock case

The case that I 3D-printed can be seen at https://www.thingiverse.com/make:794146.
Please follow back the "remixed from" links to see the original
creator's version.