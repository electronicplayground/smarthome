## docker

```bash
docker run --privileged -it -p 8001:8001 -e "MQTT_BROKER=192.168.0.234" -e "SERIAL_PORT=/dev/ttyUSB0" electronicplayground/serial_controller:armv7
```

```bash
docker run --privileged -it -p 8001:8001 -e "MQTT_BROKER=192.168.0.31" -e "SERIAL_PORT=/dev/ttyUSB0" -e "MQTT_PUBLISH_TOPIC=/test/serial" electronicplayground/serial_controller:arm64
```