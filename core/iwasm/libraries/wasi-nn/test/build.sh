/opt/wasi-sdk/bin/clang \
    -Wl,--allow-undefined \
    -Wl,--strip-all,--no-entry \
    --sysroot=/opt/wasi-sdk/share/wasi-sysroot/ \
    -I/home/wamr/core/iwasm/libraries/wasi-nn \
    -o test_tensorflow.wasm test_tensorflow.c
