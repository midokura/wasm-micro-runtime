# WARM API

<<<<<<< HEAD
## Setup

### Pre-requisites

Install requirements,

```
pip install -r requirements.txt
```

### Build native lib and update bindings

<<<<<<< HEAD
Run the following command,
=======
The following command builds the iwasm library and generates the Python bindings,

```sh
bash language-bindings/python/utils/create_lib.sh
```

This will build and copy libiwasm into the package.
>>>>>>> b4f02284... Python WAMR API binding: Add malloc/free and register_native (#1989)

## Examples

There is a [simple example](./samples/main.py) to show how to use bindings.

```
python samples/main.py
=======
## Examples

Copy in `language-bindings/python/wamr/libs` the library `libiwasm` generated from `product-mini/platforms`.

There is a [simple example](./samples/main.py) to show how to use bindings.

```
python samples/main.py
```

## Update WAMR API bindings

Install requirements,

```
pip install -r requirements.txt
```

Run the following command,

```sh
ctypesgen ../../../../core/iwasm/include/wasm_export.h -l ../libs/libiwasm.so -o iwasm.py
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
```
