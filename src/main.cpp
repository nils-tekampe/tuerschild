
#include <Arduino.h>
#include <ArduinoLog.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <epd7in5.h>
#include <images.h>

// stop 1
// welcome 2

const char *SSID = "alpha";
const char *PASSWORD = "halima@2003";

#define uS_TO_S_FACTOR                                                         \
  1000000 /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP 60 /* Time ESP32 will go to sleep (in seconds) */
#define TIME_TO_SLEEP 600 /* Time ESP32 will go to sleep (in seconds) */

// the following two variables are stored in RTC memory so that they can survive
// deep sleep
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int lastResult = 0;

#define LED_BUILTIN 2

void update() {

  if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status

    HTTPClient http;

    http.begin("https://doorsign.konfidas.de/status.php"); // Specify the URL
    int httpCode = http.GET();                             // Make the request

    if (httpCode > 0) { // Check for the returning code
      String payload = http.getString();
      int payload_int = atoi(payload.c_str());
      Log.notice("Der payload lautet: %s" CR, payload.c_str());
      if (payload_int != lastResult) {
        Log.notice("Payload hat sich geändert. Wir zeichnen das Bild neu" CR);
        Epd epd;
        if (epd.Init() != 0) {
          Log.error("e-Paper init failed" CR);
          return;
        }

        if (payload_int == 2) {
          Log.notice("Printing Willkommen" CR);
          epd.DisplayFrame(WELCOME_IMAGE);
        }

        else if (payload_int == 1) {
          Log.notice("Printing Stop" CR);
          epd.DisplayFrame(STOP_IMAGE);
        }

        lastResult = payload_int;
        Log.notice("Bild wurde aktualisiert" CR);
      } else {
        Log.notice(
            "Der Status hat sich nicht geändert. Ich gehe wieder schlafen." CR);
      }
    }

    else {
      Log.error("Error on HTTP request" CR);
    }

    http.end(); // Free the resources
  }
}

void confirmUpdate() {
  if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status
    HTTPClient http;
    http.begin(
        "https://doorsign.konfidas.de/confirmUpdate.php"); // Specify the URL
    int httpCode = http.GET();
    Log.notice("confirmUpdate beendet." CR);
    Log.notice("httpCode: %d" CR, httpCode);
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  // Log.begin(LOG_LEVEL_SILENT, &Serial);

  Log.notice("Bootcounter: %d" CR, bootCount);
  Log.notice("lastResult: %d" CR, lastResult);

  bootCount++;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Log.notice("Connection to WiFi...");
  }

  Log.notice("Connected to the WiFi network" CR);
  pinMode(LED_BUILTIN, OUTPUT);

  update();
  confirmUpdate();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {}
