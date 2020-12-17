# FROM debian:latest
FROM python:3.8

# RUN apt-get update && apt-get upgrade

# RUN apt-get install wget -y

# RUN apt-get install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev -y

# RUN wget https://www.python.org/ftp/python/3.8.0/Python-3.8.0.tar.xz

# RUN tar -xf Python-3.8.0.tar.xz

# # RUN cd Python-3.8.0
# WORKDIR /Python-3.8.0

# RUN ./configure --enable-optimizations

# RUN make

# RUN make altinstall

WORKDIR /

RUN wget https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -O get-platformio.py

RUN python3.8 get-platformio.py

RUN apt-get update && apt-get install -y vim

# RUN source /root/.platformio/penv/bin/activate