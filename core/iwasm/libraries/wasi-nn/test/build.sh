# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

CURR_PATH=$(cd $(dirname $0) && pwd -P)

# WASM application that uses WASI-NN

/opt/wasi-sdk/bin/clang \
    -Wl,--allow-undefined \
    -Wl,--strip-all,--no-entry \
    --sysroot=/opt/wasi-sdk/share/wasi-sysroot \
    -I.. -I../src/utils \
    -o test_tensorflow.wasm \
    test_tensorflow.c utils.c

# TFLite models to use in the tests

pushd ${CURR_PATH}/models
python3 average.py
python3 max.py
python3 mult_dimension.py
python3 mult_outputs.py
python3 sum.py
popd

# Specific tests for TPU

/opt/wasi-sdk/bin/clang \
    -Wl,--allow-undefined \
    -Wl,--strip-all,--no-entry \
    --sysroot=/opt/wasi-sdk/share/wasi-sysroot \
    -I.. -I../src/utils \
    -o test_tensorflow_tpu.wasm \
    test_tensorflow_tpu.c utils.c

pushd ${CURR_PATH}/models
python3 quantized.py
popd