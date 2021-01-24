// long lastMsg = 0;
// //char msg[50];
// int pirState = LOW;



// void PIR() {
//   int oldState = pirState;
//   int val = digitalRead(PIR_PIN);  // read input value
//   if (val == HIGH) {            // check if the input is HIGH
//     if (pirState == LOW) {
//       // we have just turned on
//       Serial.println("Motion detected!");
//       // We only want to print on the output change, not state
//       pirState = HIGH;
//     }
//   } else {
//     if (pirState == HIGH){
//       // we have just turned of
//       Serial.println("Motion ended!");
//       // We only want to print on the output change, not state
//       pirState = LOW;
//     }
//   }
//   if (oldState != pirState) {
//     char buffer[100];
//     StaticJsonDocument<100> jsonBuffer;
//     JsonObject root = jsonBuffer.to<JsonObject>();
//     root["type"] = "PIR"; 
//     root["payload"] = val;
//     serializeJson(root, buffer);
//     client.publish(topic_out, buffer); 
//   }  
// }


// void takeReading()
// {
  
//   // Reading temperature or humidity takes about 250 milliseconds!
//   // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//   float h = dht.readHumidity();
//   // Read temperature as Celsius (the default)
//   float t = dht.readTemperature();
//   // Read temperature as Fahrenheit (isFahrenheit = true)
//   // float f = dht.readTemperature(true);

//   // Check if any reads failed and exit early (to try again).
//   if (isnan(h) || isnan(t)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

//   // Compute heat index in Fahrenheit (the default)
//   // float hif = dht.computeHeatIndex(f, h);
//   // Compute heat index in Celsius (isFahreheit = false)
//   // float hic = dht.computeHeatIndex(t, h, false);

//   /*Serial.print(F("Humidity: "));
//   Serial.print(h);
//   Serial.print(F("%  Temperature: "));
//   Serial.print(t);
//   Serial.print(F("°C "));
//   Serial.print(f);
//   Serial.print(F("°F  Heat index: "));
//   Serial.print(hic);
//   Serial.print(F("°C "));
//   Serial.print(hif);
//   Serial.println(F("°F"));
//   Serial.print("light: ");
//   Serial.println(analogRead(LDR_PIN));*/

//   char buffer[100];
//   StaticJsonDocument<100> jsonBuffer;
//   JsonObject root = jsonBuffer.to<JsonObject>();
//   root["type"] = "DHT";
//   root["t"] = t;
//   root["h"] = h;
//   serializeJson(root, buffer);
//   client.publish(topic_out, buffer);

//   root = jsonBuffer.to<JsonObject>();
//   root["type"] = "LDR";
//   root["payload"] = analogRead(LDR_PIN);
//   serializeJson(root, buffer);
//   client.publish(topic_out, buffer);
// }
