// Libraries
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <TimeLib.h>

#include "SSD1306Brzo.h"
#include "OLEDDisplayUi.h"
#include "font.h"
#include "images.h"

// Display Pins
#define SDA D6
#define SCL D5
#define I2C 0x3C

// Create display
SSD1306Brzo display(I2C, SDA, SCL);
OLEDDisplayUi ui ( &display );

// WiFi settings
const char* ssid     = "***";
const char* password = "***";
const int httpsPort = 443;
const int httpPort = 80;

// API server
//https://api.coinbase.com/v2/prices/BTC-USD/buy
const char* host = "api.coinbase.com";
String url = "/v2/prices/BTC-USD/buy";


WiFiClientSecure client;

// utility function for digital clock display: prints leading 0
String twoDigits(int digits){
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

void showfail(String msg) {
  Serial.print("error: ");
  Serial.println(msg);
  display.clear();
  display.drawXbm(30, 0, icon_round_width, icon_round_height, icon_round_bits);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, msg);
  display.display();
}

void setup() {

  //Serial
  Serial.begin(115200);
  delay(10);

  // Initialize display
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Synchronize time useing SNTP. This is necessary to verify that
  // the TLS certificates offered by the server are currently valid.
  Serial.print("Setting time using SNTP");
  configTime(-8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 1000) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Startup time: ");
  Serial.print(asctime(&timeinfo));

}

void loop() {

  // Connect to API
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClientSecure class to create TCP connections
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    showfail("connect fail");
    return;
  }

  // We now create a URI for the request
  //Serial.print("Requesting URL: ");
  //Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
   "Host: " + host + "\r\n" +
   "User-Agent: esp8266/2.4.0" +
   "Connection: close\r\n\r\n");
  delay(100);

  // Read all the lines of the reply from server and print them to Serial
  String answer;
  while (client.available()) {
    String line = client.readStringUntil('\r');
    answer += line;
  }

  client.stop();
  Serial.println();
  Serial.println("closing connection");

  // Process answer
  Serial.println();
  //Serial.println("Answer: ");
  //Serial.println(answer);

  // Convert to JSON
  String jsonAnswer;
  int jsonIndex;

  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{' || answer[i] == '[') {
      jsonIndex = i;
      break;
    }
  }

  // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  //Serial.println("JSON answer: ");
  //Serial.println(jsonAnswer);
  jsonAnswer.trim();

  // From https://arduinojson.org/assistant/
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + 50;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(jsonAnswer);
  if (!root.success())
  {
    showfail("JSON parse failed!");
    Serial.println(answer);
    delay(2000);
    return;
  }

  JsonObject& data = root["data"];
  const char* data_base = data["base"]; // "BTC"
  const char* data_currency = data["currency"]; // "USD"
  const char* data_amount = data["amount"]; // "10252.78"


  Serial.print("Bitcoin price: ");
  Serial.println(data_amount);

  String priceString = String(data_amount);
  priceString.trim();

  time_t now = time(nullptr);
  struct tm timeinfo;
  now = time(nullptr);
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
  //String timenow = String(hour())+":"+twoDigits(minute())+":"+twoDigits(second());

  // Display on OLED
  display.clear();
  display.setFont(Nimbus_Roman_No9_L_Medium_22);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(128/2, 64/2, priceString);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(128, 0, asctime(&timeinfo));
  display.display();

  // Wait
  delay(1000);
}

//TODO resync time once in a while. (every hour?)
