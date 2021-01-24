#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager


#include <PubSubClient.h>

#include "config.h"

typedef void (*mqtt_callback_function)(char* topic, byte* payload, unsigned int length);

void config_connectivity();
void reconnect();
void mqtt_loop();
void configure_callback(mqtt_callback_function callback);
void mqtt_publish(char* topic, char* payload);
