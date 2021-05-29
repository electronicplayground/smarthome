# smarthome
Services and applications for smart home.


## IoTHub

Acts as gateway between devices and the data ingestion and control system.

Listens to <MQTT_TOPIC>/#

### MQTT 
- MQTT client lib: https://github.com/chkr1011/MQTTnet/wiki/Client

Environment variables:
- MQTT_TOPIC: 123456789 - base topic. Each IotHub instance should have a unic topic
- MQTT_BROKER: smarthome_mosquitto
- MQTT_PORT: 1883


