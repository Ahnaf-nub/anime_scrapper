#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Mahir";       // Replace with your WiFi SSID
const char* password = "Ahnaf2007"; // Replace with your WiFi password
String url = "https://zoroxtv.to/updated"; // Zoro website URL

void setup() {
  Serial.begin(115200); // Start the Serial Monitor at a baud rate of 115200
  WiFi.begin(ssid, password); // Connect to WiFi

  // Wait for the connection to establish
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url); // Initiate HTTP request to the URL

    int httpCode = http.GET(); // Send the request
    if (httpCode > 0) {
      String payload = http.getString(); // Get the response payload
      Serial.println("Response received:");
      // Serial.println(payload); // Uncomment to print the full HTML content

      // Simple parsing to extract anime names
      // Assuming the anime names are in <h3 class="film-name"> tags
      String startTag = "<h3 class=\"film-name\">";
      String endTag = "</h3>";
      int startIndex = 0;

      while ((startIndex = payload.indexOf(startTag, startIndex)) != -1) {
        startIndex += startTag.length();
        int endIndex = payload.indexOf(endTag, startIndex);
        if (endIndex != -1) {
          String animeName = payload.substring(startIndex, endIndex);
          Serial.println("Anime Name: " + animeName);
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    
    http.end(); // Close the HTTP connection
  }

  delay(60000); // Wait a minute before making the next request
}
