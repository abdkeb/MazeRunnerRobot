#ifndef COMM_H
#define COMM_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


// Replace with your Google Apps Script web app URL
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbwd34TDhpt5AGHXRG5R1E_HHrW5pwkp3OkGSgzL4QSRIfCyVT2BY3pf9qvPh6CGi1iPtw/exec?read";

void commSetup() {
  digitalWrite(LEFT_LED_PIN, HIGH);
  digitalWrite(RIGHT_LED_PIN, HIGH);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  digitalWrite(LEFT_LED_PIN, LOW);
  digitalWrite(RIGHT_LED_PIN, LOW);

}

void commReadData() {
    if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status
    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.begin(scriptUrl);  // Specify the URL
    int httpCode = http.GET();  // Make the request

    if (httpCode == 200) { // Check for the returning code
      String payload = http.getString();
      Serial.print("HTTP Code: ");
      Serial.println(httpCode);
      Serial.print("Data from Google Sheets: ");
      Serial.println(payload);
  
      // Parse JSON
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        // Example: Assuming the JSON is an array of arrays
        for (size_t i = 0; i < doc.size(); i++) {
          JsonArray row = doc[i];
          for (size_t j = 0; j < row.size(); j++) {
            Serial.print(row[j].as<String>());
            Serial.print(" "); 
          }
          if(row[0] == "right"){turns.emplace_back(right, row[1]);}
          if(row[0] == "left"){turns.emplace_back(left, row[1]);}
          Serial.println();
          digitalWrite(LEFT_LED_PIN,HIGH);
          digitalWrite(RIGHT_LED_PIN,HIGH);
          delay(500);
          digitalWrite(LEFT_LED_PIN,LOW);
          digitalWrite(RIGHT_LED_PIN,LOW);
          delay(500);
        }
      } else {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      }

    } else {
      Serial.print("Error on HTTP request, HTTP code: ");
      Serial.println(httpCode);
      while(1){
        digitalWrite(LEFT_LED_PIN,HIGH);
        digitalWrite(RIGHT_LED_PIN,LOW);
        delay(200);
        digitalWrite(LEFT_LED_PIN,LOW);
        digitalWrite(RIGHT_LED_PIN,HIGH);
        delay(200);
        }
    }

    http.end();  // Free the resources
  }

  delay(2000);  
}

#endif // COMM_H
