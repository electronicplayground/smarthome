## docker

```bash
docker run --privileged -it -p 8001:8001 -e "MQTT_BROKER=192.168.0.234" -e "SERIAL_PORT=/dev/ttyUSB0" serial_controller_test
```