## Platformio

```bash
docker build . -f platformio.dockerfile -t electronicplayground/platformio:latest
docker image push electronicplayground/platformio:latest
```

```bash
docker run --privileged -i -t electronicplayground/platformio:latest /bin/bash
```

Run with volume mount to code repo:

```bash
docker run --privileged -v /path/to/code:/code -i -t electronicplayground/platformio:latest /bin/bash
```


In container:
```bash
source /root/.platformio/penv/bin/activate
```

```bash
pio --version
```