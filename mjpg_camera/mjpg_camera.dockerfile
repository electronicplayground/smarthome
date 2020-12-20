FROM debian:latest

RUN apt-get update

RUN apt install -y g++
RUN apt install -y cmake
RUN apt install -y make
RUN apt install -y wget unzip

RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip
RUN unzip opencv.zip
RUN mv opencv-master opencv

RUN mkdir -p build

WORKDIR build
RUN cmake ../opencv
RUN make -j4
RUN make install
