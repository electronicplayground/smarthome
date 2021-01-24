#include "connectivity.h"

const char *mqtt_server = MQTT_SERVER;
const char *client_name = DEVICE_NAME;
char *topic_in = TOPIC_IN;
char *topic_out = TOPIC_OUT;

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
}

void config_connectivity()
{
    Serial.println("Configuring connectivity");
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset settings - for testing
    //wifiManager.resetSettings();

    //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wifiManager.setAPCallback(configModeCallback);

    //fetches ssid and pass and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.autoConnect())
    {
        Serial.println("failed to connect and hit timeout");
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    mqtt_client.setServer(mqtt_server, 1883);
    //   mqtt_client.setCallback(callback);
}

void reconnect()
{
    // Loop until we're reconnected
    while (!mqtt_client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (mqtt_client.connect(client_name))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            mqtt_client.publish(topic_out, "hello world");
            // ... and resubscribe
            mqtt_client.subscribe(topic_in);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void mqtt_loop()
{
    // put your main code here, to run repeatedly:
    if (!mqtt_client.connected())
    {
        reconnect();
        delay(2000);
    }
    mqtt_client.loop();
}

void configure_callback(mqtt_callback_function callback) 
{
    mqtt_client.setCallback(callback);
}

void mqtt_publish(char* topic, char* payload)
{
    mqtt_client.publish(topic, payload);
}