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

# \opt\...\clang \
#     -Wl,--allow-undefined \
# 	-Wl,--strip-all \
# 	-Wl,--export=malloc -Wl,--export=free \
# 	-Wl,--export=__data_end \
# 	-Wl,--export=__heap_base \
# 	-Wl,--export=trans_output_tensor \
# 	--sysroot=/opt/wasi-sdk/share/wasi-sysroot/ \
# 	-z stack-size=8192 \
# 	-Wl,--initial-memory=65536 \
#      // HERE YOUR CODE



# /opt/wasi-sdk/bin/clang     \
#         --target=wasm32-wasi -O0 -z stack-size=4096 -Wl,--initial-memory=65536 \
#         --sysroot=${WAMR_DIR}/wamr-sdk/app/libc-builtin-sysroot  \
#         -Wl,--allow-undefined-file=${WAMR_DIR}/wamr-sdk/app/libc-builtin-sysroot/share/defined-symbols.txt \
#         -Wl,--strip-all,--no-entry -nostdlib \
#         -Wl,--export=generate_float \
#         -Wl,--export=float_to_string \
#         -Wl,--export=calculate\
#         -Wl,--allow-undefined \
#         --sysroot=/opt/wasi-sdk/share/wasi-sysroot/ \
#         -I/home/core/iwasm/libraries/wasi-nn \
#         -I/home/core/shared/utils/uncommon \
#         -I/home/core/shared/utils \
#         -I/home/core/shared/platform/zephyr \
#         -o ${OUT_DIR}/wasm-apps/${OUT_FILE} ${APP_SRC}