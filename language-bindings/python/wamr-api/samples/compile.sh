<<<<<<< HEAD
=======
#!/bin/sh

>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

/opt/wasi-sdk/bin/clang     \
    -O0 -z stack-size=4096 -Wl,--initial-memory=65536 \
    -Wl,--strip-all,--no-entry -nostdlib \
    -Wl,--export=sum\
    -Wl,--allow-undefined \
<<<<<<< HEAD
    -o sum.wasm sum.c
=======
    -o test.wasm sum.c
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
