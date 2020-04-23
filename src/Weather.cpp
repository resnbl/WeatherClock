#include "Weather.h"
#include <ArduinoJson.h>

// http://api.openweathermap.org/data/2.5/weather?appid=aaaaa&id=iiiii&units=uuuuu
#define OWM_HOST "api.openweathermap.org"
#define OWM_PATH1 "/data/2.5/weather?appid="
#define OWM_PATH2 "&id="
#define OWM_PATH3 "&units="

Weather::Weather(const char* api, const char* cityId, const char* units) {
  apiPath = OWM_PATH1;
  apiPath += api;
  apiPath += OWM_PATH2;
  apiPath += cityId;
  apiPath += OWM_PATH3;
  apiPath += units;
}

String& Weather::getWeather() {
  // Only make API call every XXX mSecs
  if (millis() - lastFetch >= WEATHER_CHECK_MS) {
    fetchWeather();
  }

  return weatherStr;
}

void Weather::fetchWeather() {
  Serial.println(F("Fetching weather..."));

  WiFiClient wifi;

  // Connect to server
  wifi.setTimeout(10000);
  if (!wifi.connect(OWM_HOST, 80)) {
    weatherStr = F("Weather host failed");
    return;
  }

  // Send HTTP request to API
  wifi.println("GET " + apiPath + " HTTP/1.1");
  wifi.println("Host: " OWM_HOST);
  wifi.println("User-Agent: ArduinoWiFi/1.1");
  wifi.println("Connection: close");
  if (wifi.println() == 0) {
    weatherStr = F("Weather host send failed");
    return;
  }
  
  // Check HTTP status
  char status[32] = {0};
  wifi.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    weatherStr = status;
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!wifi.find(endOfHeaders)) {
    weatherStr = F("Invalid HTTP response");
    return;
  }

  parseResponse(wifi);
  wifi.stop();
}

void Weather::parseResponse(WiFiClient client) {
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, client);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    weatherStr = error.c_str();
    return;
  }

  // Update time of last *successful* fetch
  lastFetch = millis();

  // Round floats to nearest int
  int tempF = (doc["main"]["temp"].as<float>()) + 0.5;
  int humid = (doc["main"]["humidity"].as<float>()) + 0.5;

  weatherStr = "*** ";  // lead-in
  weatherStr += String(tempF);
  weatherStr += "F ";
  weatherStr += String(humid);
  weatherStr += "% ";
  weatherStr += doc["weather"][0]["description"].as<String>();
}
