FROM homeassistant/home-assistant:stable

WORKDIR /config
COPY /config /config

EXPOSE 8123