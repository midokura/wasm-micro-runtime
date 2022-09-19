FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    cmake build-essential git

WORKDIR /home/wamr

COPY core core
COPY build-scripts build-scripts
COPY product-mini product-mini

WORKDIR /home/wamr/core/iwasm/libraries/wasi-nn/test/build

RUN cmake ..

