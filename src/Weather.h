#ifndef _WEATHER_H_
#define _WEATHER_H_

#include "config.h"
#include <WiFiClient.h>

/*  This class will fetch the current weather conditions every
    WEATHER_CHECK_MS msecs via the OpenWeatherMap API. Calls
    within that time return the buffered earlier response.
 */
class Weather {
  public:
    Weather(const char* api, const char* cityId, const char* units);
    // Get the current weather info
    String& getWeather();

  private:
    String apiPath;         // formatted API path
    String weatherStr;      // buffer result between HTTP calls
    unsigned long lastFetch = -WEATHER_CHECK_MS;  // time since last HTTP

    // Make HTTP call to get latest weather
    void fetchWeather();
    // Parse and format HTTP response
    void parseResponse(WiFiClient client);
};

#endif
