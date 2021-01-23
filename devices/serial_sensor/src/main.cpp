#include "ArduinoJson.h"
#include "DHT.h"
#include "timer.h"

long lastMsg = 0;
//char msg[50];
int pirState = LOW;




Timer<> t = timer_create_default();

#define PIR_PIN 2
#define DHT_PIN 3
#define LDR_PIN A0
#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHTTYPE);
void PIR() {
  int oldState = pirState;
  int val = digitalRead(PIR_PIN);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      //Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      //Serial.println("Motion ended!");
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
    Serial.println(buffer);
  }  
}


bool takeReading(void* argument)
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
    return true;
  }

  // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  char buffer[100];
  StaticJsonDocument<100> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["type"] = "DHT";
  root["payload"]["temperature"] = t;
  root["payload"]["humidity"] = h;
  serializeJson(root, buffer);
  Serial.println(buffer);

  root = jsonBuffer.to<JsonObject>();
  root["type"] = "LDR";
  root["payload"] = analogRead(LDR_PIN);
  serializeJson(root, buffer);
  Serial.println(buffer);

  return true;
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Started");

  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  
  dht.begin();
  t.every(10000, takeReading);
}

void loop() {
 t.tick();
  
 PIR();
}
