# Copyright (C) 2019 Intel Corporation. All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Try to find installed TFLite
find_path(tensorflow_lite_INCLUDE_DIR tensorflow/lite/model.h
  PATHS /usr/include /usr/local/include)

find_library(tensorflow_lite_LIBRARY NAMES tensorflow-lite
  PATHS /usr/lib /usr/local/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tensorflow_lite
  REQUIRED_VARS tensorflow_lite_LIBRARY tensorflow_lite_INCLUDE_DIR)

if(tensorflow_lite_FOUND AND NOT TARGET tensorflow_lite::tensorflow_lite)
  message(STATUS "Found system-installed TensorFlow Lite")
  add_library(tensorflow_lite::tensorflow_lite UNKNOWN IMPORTED)
  set_target_properties(tensorflow_lite::tensorflow_lite PROPERTIES
    IMPORTED_LOCATION "${tensorflow_lite_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${tensorflow_lite_INCLUDE_DIR}"
  )
else()
  message(STATUS "TensorFlow Lite not found. Falling back to FetchContent")

  include(FetchContent)

  set(TFLITE_SOURCE_DIR "${WAMR_ROOT_DIR}/core/deps/tensorflow-src")
  if(EXISTS ${TFLITE_SOURCE_DIR})
    message(STATUS "Using existing source under ${TFLITE_SOURCE_DIR}")
    FetchContent_Declare(
      tensorflow_lite
      SOURCE_DIR     ${TFLITE_SOURCE_DIR}
      SOURCE_SUBDIR  tensorflow/lite
    )
  else()
    message(STATUS "Downloading source into ${TFLITE_SOURCE_DIR}")
    FetchContent_Declare(
      tensorflow_lite
      GIT_REPOSITORY https://github.com/tensorflow/tensorflow.git
      GIT_TAG        v2.12.0
      GIT_SHALLOW    ON
      GIT_PROGRESS   ON
      SOURCE_DIR     ${TFLITE_SOURCE_DIR}
      SOURCE_SUBDIR  tensorflow/lite
      PATCH_COMMAND  git apply ${CMAKE_CURRENT_LIST_DIR}/add_telemetry.patch
    )
  endif()

  FetchContent_MakeAvailable(tensorflow_lite)

  if(NOT TARGET tensorflow_lite::tensorflow_lite)
    add_library(tensorflow_lite::tensorflow_lite ALIAS tensorflow_lite)
  endif()
endif()

# GPU / XNNPACK options regardless of source
if(WAMR_BUILD_WASI_NN_ENABLE_GPU EQUAL 1)
  set(TFLITE_ENABLE_GPU ON)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(TFLITE_ENABLE_XNNPACK OFF)
endif()
