# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set (WASI_NN_DIR ${CMAKE_CURRENT_LIST_DIR})

add_definitions (-DWASM_ENABLE_WASI_NN=1)


#find_package(tensorflow-lite REQUIRED)

find_library(tensorflow-lite NAMES libtensorflow-lite.so 
 PATHS ${CMAKE_SYSROOT}
 PATH_SUFFIXES /usr/lib)

if("${tensorflow-lite}" STREQUAL "tensorflow-lite-NOTFOUND")
    if (NOT EXISTS "${WAMR_ROOT_DIR}/core/deps/tensorflow-src")
        execute_process(COMMAND ${WAMR_ROOT_DIR}/core/deps/install_tensorflow.sh
                        RESULT_VARIABLE TENSORFLOW_RESULT
        )
    else ()
        message("Tensorflow is already downloaded.")
    endif()
    set(TENSORFLOW_SOURCE_DIR "${WAMR_ROOT_DIR}/core/deps/tensorflow-src")
    include_directories (${CMAKE_CURRENT_BINARY_DIR}/flatbuffers/include)
    include_directories (${TENSORFLOW_SOURCE_DIR})
    add_subdirectory(
        "${TENSORFLOW_SOURCE_DIR}/tensorflow/lite"
        "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)      
endif()

if(TFLITE_ENABLE_EXTERNAL_DELEGATE EQUAL ON)
add_definitions(-D_TFLITE_ENABLE_EXTERNAL_DELEGATE)
endif()
if(TFLITE_ENABLE_GPU EQUAL ON)
add_definitions(-D_TFLITE_ENABLE_GPU)
endif()

include_directories (${WASI_NN_DIR})
include_directories (${WASI_NN_DIR}/src)
include_directories (${WASI_NN_DIR}/src/utils)

set (
    LIBC_WASI_NN_SOURCE
    ${WASI_NN_DIR}/src/wasi_nn.c
    ${WASI_NN_DIR}/src/wasi_nn_tensorflowlite.cpp
    ${WASI_NN_DIR}/src/utils/wasi_nn_app_native.c
)


set (NN_LIBS tensorflow-lite stdc++ pthread m -ldl rt)

