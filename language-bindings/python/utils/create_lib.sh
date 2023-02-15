#!/bin/sh

CUR_DIR=$(cd $(dirname $0) && pwd -P)
ROOT_DIR=${CUR_DIR}/../../..

WAMR_BUILD_PLATFORM=${WAMR_BUILD_PLATFORM:-$(uname -s|tr A-Z a-z)}

cd ${ROOT_DIR}/product-mini/platforms/${WAMR_BUILD_PLATFORM}

mkdir -p build && cd build
cmake ..
make -j

cp *libiwasm* ${CUR_DIR}/../src/wamr/libs
