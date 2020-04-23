#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "Clock.h"
#include "Panel.h"
#include "Weather.h"

#define SER_MON_SPEED 74880     // same as ESP8266EX boot default speed?

Panel screen;                   // Max7219 LED panel
Clock theClock;                 // NTP services
Weather weatherMan(OWM_API, OWM_ID, OWM_UNITS);   // weather services
struct tm tms;        // date/time components
char buff[16];        // date/time string buffer
int lastSec = -1;     // previous second value

void setup() {
  Serial.begin(SER_MON_SPEED);
//  Serial.setDebugOutput(true);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);  // turns OFF built-in LED on Wemos D1

	// Connect to WiFi network
	Serial.print("\nConnecting to ");
	Serial.println(WIFI_SSID);

	WiFi.mode(WIFI_STA);
	WiFi.hostname(WIFI_HOST);
  delay(500);
	WiFi.begin(WIFI_SSID, WIFI_PSWD);
  screen.centerText("WiFi");

	for (int cnt = 0; WiFi.status() != WL_CONNECTED; ++cnt) {
    if (cnt > WIFI_MAX_CONN_COUNT) {
      screen.scrollText(F("WiFi failure: restart..."));
      ESP.restart();
    }
		delay(WIFI_CONN_DELAY_MS);
	}

	Serial.print(F("WiFi connected at IP: "));
	Serial.println(WiFi.localIP());
  screen.scrollText(WiFi.localIP().toString());

  // Connect to NTP server
  screen.centerText("NTP");
  theClock.begin_P(NTP_TZ, NTP_POOL);     // use PROGMEM data
	for (int cnt = 0; !theClock.isSynced(); ++cnt) {
    if (cnt > NTP_MAX_SYNC_COUNT) {
      screen.scrollText(F("NTP failure: restart..."));
      ESP.restart();
    }
		delay(NTP_SYNC_DELAY_MS);
	}
}

// Update display at most once per second
void loop() {
  theClock.getTime(tms);
  if (lastSec != tms.tm_sec)        // if time changed
  {
    lastSec = tms.tm_sec;
    if (lastSec == 0) {             // show date "on the minute"
      theClock.dateStr(buff, tms);
      screen.scrollText(buff);
    }
    else if (lastSec == 30) {       // show forecast "on the half-minute"
      screen.scrollText(weatherMan.getWeather());
    }
    else {
      theClock.hm12Str(buff, tms);  // flash colon on/off every second
      screen.centerText(buff);
    }
  }
  
  delay(500);
}