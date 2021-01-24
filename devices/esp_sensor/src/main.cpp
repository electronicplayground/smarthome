// #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

// //needed for WiFiManager
// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
// #include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

// #include <PubSubClient.h>
#include "ArduinoJson.h"
#include "DHT.h"
#include "Timer.h"
#include <Adafruit_NeoPixel.h>

#include "config.h"
#include "connectivity.h"

// WiFiClient espClient;
// PubSubClient client(espClient);

Timer t;

// const char* mqtt_server = "192.168.0.31";
// const char* client_name = "esp8266_x";
// char* topic_in = "/sensors/esp8266_x/in";
// char* topic_out = "/sensors/esp8266_x/out";





Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);



DHT dht(DHT_PIN, DHTTYPE);

// void configModeCallback (WiFiManager *myWiFiManager) {
//   Serial.println("Entered config mode");
//   Serial.println(WiFi.softAPIP());
//   //if you used auto generated SSID, print it
//   Serial.println(myWiFiManager->getConfigPortalSSID());
// }


long lastMsg = 0;
//char msg[50];
int pirState = LOW;



void PIR() {
  int oldState = pirState;
  int val = digitalRead(PIR_PIN);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  if (oldState != pirState) {
    char buffer[100];
    StaticJsonDocument<100> jsonBuffer;
    JsonObject root = jsonBuffer.to<JsonObject>();
    root["type"] = "PIR"; 
    root["payload"] = val;
    serializeJson(root, buffer);
    mqtt_publish(TOPIC_OUT, buffer);  
  }  
}


void takeReading()
{
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  /*Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  Serial.print("light: ");
  Serial.println(analogRead(LDR_PIN));*/

  char buffer[100];
  StaticJsonDocument<100> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["type"] = "DHT";
  root["t"] = t;
  root["h"] = h;
  serializeJson(root, buffer);
  mqtt_publish(TOPIC_OUT, buffer);

  root = jsonBuffer.to<JsonObject>();
  root["type"] = "LDR";
  root["payload"] = analogRead(LDR_PIN);
  serializeJson(root, buffer);
  mqtt_publish(TOPIC_OUT, buffer);
}

void rgb_command(char* payload);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived ");
  Serial.println(topic);
  char payload_arr[200];
  for (unsigned int i=0; i<length; i++) {
    payload_arr[i] = (char)payload[i];
  }
  rgb_command(payload_arr);
}

void rgb_command(char* payload) {
  Serial.println(payload);
  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("error");
    Serial.println(error.c_str());
    return;
  }
  long r = doc["payload"]["r"];
  long g = doc["payload"]["g"];
  long b = doc["payload"]["b"];
  int intensity = doc["payload"]["i"];
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  
  intensity = NUMPIXELS * intensity / 100;
  int noOfPixels = NUMPIXELS;
  if (intensity != 0) {
    noOfPixels = intensity;
  }
  Serial.println(noOfPixels);
  for (short i = 0; i < NUMPIXELS; i++) {
    // Turn all off first
    pixels.setPixelColor(i, 0,0,0); // g, r, b
    pixels.show(); // this sends the updated pixel color to the hardware.

  }
  for (short i = 0; i < noOfPixels; i++) {
    pixels.setPixelColor(i, r, g, b); // g, r, b
    pixels.show(); // this sends the updated pixel color to the hardware.

  }

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Started change");
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(RGB_PIN, OUTPUT);

  config_connectivity();
  configure_callback(&callback);
  dht.begin();
  t.every(10000, takeReading);

  pixels.begin();
  pixels.show();
}

void loop() {
  mqtt_loop();
  t.update();
  
  PIR();
  
}
