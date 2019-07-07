
#include <Arduino.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <epd7in5.h>
#include <images.h>

const char *ssid = "alpha";
const char *password = "halima@2003";

#define uS_TO_S_FACTOR                                                         \
  1000000                 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 600 /* Time ESP32 will go to sleep (in seconds) */

// the following two variables are stored in RTC memory so that they can survive
// deep sleep
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR String lastResult = "";

#define LED_BUILTIN 2

void setup() {

  Serial.begin(9600);
  Serial.print("Bootcounter: ");
  Serial.println(bootCount);

  bootCount++;

  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status

    HTTPClient http;

    http.begin("https://doorsign.konfidas.de/status.php"); // Specify the URL
    int httpCode = http.GET();                             // Make the request

    if (httpCode > 0) { // Check for the returning code
      String payload = http.getString();

      Serial.println("Der payload lautet:" + payload);
      if (payload != lastResult) {
        Serial.println("Payload hat sich geändert. Wir zeichnen das Bild neu");
        Epd epd;
        if (epd.Init() != 0) {
          Serial.print("e-Paper init failed");
          return;
        }

        if (payload == "welcome") {
          Serial.println("Printing Willkommen");
          epd.DisplayFrame(WELCOME_IMAGE);
        }

        else if (payload == "stop") {
          Serial.println("Printing Stop");
          epd.DisplayFrame(STOP_IMAGE);
        }

        lastResult = payload;
        Serial.println("Bild wurde aktualisiert");
      } else {
        Serial.println(
            "Der Status hat sich nicht geändert. Ich gehe wieder schlafen.");
      }
    }

    else {
      Serial.println("Error on HTTP request");
    }

    http.begin(
        "https://doorsign.konfidas.de/confirmUpdate.php"); // Specify the URL

    http.end(); // Free the resources
  }

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
