#
# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#

#!/bin/bash

# use WAMR SDK to build out the .wasm binary
/opt/wasi-sdk/bin/clang     \
        -Wl,--allow-undefined \
        -Wl,--strip-all,--no-entry \
        --sysroot=/opt/wasi-sdk/share/wasi-sysroot/ \
		-I/home/ahmedounet/Desktop/wasi_nn_tflite/wasm-micro-runtime/core/iwasm/libraries/wasi-nn \
        -o testapp.wasm testapp.c
