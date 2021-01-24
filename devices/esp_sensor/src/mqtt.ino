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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_name)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic_out, "hello world");
      // ... and resubscribe
      client.subscribe(topic_in);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
