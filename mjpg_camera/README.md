```bash
sudo docker run -it --privileged -p 8002:8080 electronicplayground/mjpg_streamer:armv7 bash
```

```bash
sudo docker run -it --privileged -p 8002:8080 electronicplayground/mjpg_streamer:arm64 bash
```

```bash
sudo docker run -it --privileged -p 8002:8080 electronicplayground/mjpg_streamer:latest bash
```

```bash
mjpg_streamer -i "input_uvc.so" -o "output_http.so -w ./www"
```