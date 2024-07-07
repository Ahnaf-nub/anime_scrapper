#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Mahir";
const char* password = "Ahnaf767";

// API endpoint for currently airing anime (Jikan API)
const char* apiEndpoint = "https://api.jikan.moe/v4/seasons/now";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Fetch and display currently airing anime episodes
  fetchAnimeEpisodes();
}

void loop() {
  // Nothing to do here
}

void fetchAnimeEpisodes() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(apiEndpoint);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("HTTP Response Code: " + String(httpResponseCode));
      
      // Parse JSON payload
      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      // Display data in a human-readable format
      JsonArray dataArray = doc["data"].as<JsonArray>();
      Serial.println("Currently Airing Anime:");
      Serial.println("======================");
      for (JsonObject data : dataArray) {
        const char* title = data["title"];
        const char* next_episode_date = data["broadcast"]["next_episode"].as<const char*>();

        Serial.print("Title: ");
        Serial.println(title);
        Serial.print("Next Episode Release Date: ");
        Serial.println(next_episode_date ? next_episode_date : "N/A");
        Serial.println("----------------------");
      }
      
    } else {
      Serial.println("Error in HTTP request");
    }
    http.end();
  } else {
    Serial.println("Not connected to Wi-Fi");
  }
}
