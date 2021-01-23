class ConfigKeys:
    CONFIG_KEYS = (
        WS_PORT,
        WS_ENDPOINT,
        SERIAL_PORT,
        SERIAL_BAUD_RATE,
        MQTT_BROKER,
        MQTT_PORT,
        MQTT_PUBLISH_TOPIC,
        MQTT_SUBSCRIBE_TOPIC,
    ) = (
        'WS_PORT',
        'WS_ENDPOINT',
        'SERIAL_PORT',
        'SERIAL_BAUD_RATE',
        'MQTT_BROKER',
        'MQTT_PORT',
        'MQTT_PUBLISH_TOPIC',
        'MQTT_SUBSCRIBE_TOPIC',
    )

    DEFAULTS = {
        SERIAL_PORT: "COM9",
        SERIAL_BAUD_RATE: 9600,
        WS_PORT: 8001,
        WS_ENDPOINT: '/ws',
        MQTT_PUBLISH_TOPIC: '/433mhz_controller/out',
        MQTT_SUBSCRIBE_TOPIC: '/433mhz_controller/in',
        MQTT_PORT: 1883,
        MQTT_BROKER: '192.168.0.31',
    }

