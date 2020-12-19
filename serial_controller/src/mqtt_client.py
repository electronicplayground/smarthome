
import paho.mqtt.client as mqtt

from configuration.config_keys import ConfigKeys
from configuration.configuration import Configuration

config = Configuration.Instance()

class MqttClientMixin:
    _client = None
    mqtt_enabled = False

    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        self._client.subscribe(config[ConfigKeys.MQTT_SUBSCRIBE_TOPIC])

    def initalize_mqtt(self):
        if not config[ConfigKeys.MQTT_BROKER]:
            return

        self._client = mqtt.Client()
        self._client.on_connect = self.on_connect
        self._client.on_message = self.on_message

        self._client.connect(config[ConfigKeys.MQTT_BROKER], config[ConfigKeys.MQTT_PORT], 60)
        self.mqtt_enabled = True

    def publish(self, topic, msg):
        if self.mqtt_enabled:
            self._client.publish(topic, payload=str(msg), qos=0, retain=False)

    def disconnect(self):
        if self.mqtt_enabled:
            self._client.disconnect()

    def loop(self):
        if self.mqtt_enabled:
            self._client.loop()