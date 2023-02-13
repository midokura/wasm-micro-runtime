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
ctypesgen ../ppl-wasm/submodules/wasm-micro-runtime/core/iwasm/include/wasm_export.h -l libiwasm.so -o libiwasm.py
```
