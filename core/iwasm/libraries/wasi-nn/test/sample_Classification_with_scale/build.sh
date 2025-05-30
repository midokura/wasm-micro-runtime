#!/bin/sh

# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

CURR_PATH=$(cd $(dirname $0) && pwd -P)

# WASM application that uses WASI-NN

/opt/wasi-sdk/bin/clang \
    --target=wasm32-wasi \
    -DNN_LOG_LEVEL=1 \
    -Wl,--allow-undefined \
    -I../../include -I../../src/utils \
    -o main.wasm \
    main.c nn.c

# Copy the WASI-NN shared library to the current directory
# cp ~/Project/T4/wamr_hx/product-mini/platforms/linux/build/libwasi_nn_tflite.so  .

# run
#~/Project/T4/wamr_hx/product-mini/platforms/linux/build/iwasm --dir=./ --env="TARGET=cpu" main.wasm



