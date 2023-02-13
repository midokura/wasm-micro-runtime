# WARM API

## Examples

Copy in `language-bindings/python/wamr/libs` the library `libiwasm` generated from `product-mini/platforms`.

There is a [simple example](./samples/main.py) to show how to use bindings.

```
python samples/main.py
```

## Update WAMR API bindings

Run the following command,

```sh
ctypesgen ../../../../core/iwasm/include/wasm_export.h -l ../libs/libiwasm.so -o iwasm.py
```
