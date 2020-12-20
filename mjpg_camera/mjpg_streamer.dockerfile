FROM debian:latest

WORKDIR /

RUN apt-get install -y git
RUN apt-get install -y  libjpeg62-turbo-dev

RUN git clone https://github.com/jacksonliam/mjpg-streamer.git

WORKDIR /mjpg-streamer/mjpg-streamer-experimental
RUN make
RUN make install


