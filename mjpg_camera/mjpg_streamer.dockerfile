FROM debian:latest

WORKDIR /

RUN apt-get update
RUN apt-get install -y git
RUN apt-get install -y  libjpeg62-turbo-dev cmake

RUN git clone https://github.com/jacksonliam/mjpg-streamer.git

WORKDIR /mjpg-streamer/mjpg-streamer-experimental
RUN make
RUN make install


