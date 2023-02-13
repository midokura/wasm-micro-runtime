/opt/wasi-sdk/bin/clang     \
    -O0 -z stack-size=4096 -Wl,--initial-memory=65536 \
    -Wl,--strip-all,--no-entry -nostdlib \
    -Wl,--export=sum\
    -Wl,--allow-undefined \
    -o test.wasm sum.c
