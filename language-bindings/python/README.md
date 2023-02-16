# wamr-python

The WAMR Python package contains a set of high-level bindings for WAMR API and WASM-C-API.

## Installation

To Install from local source tree in _development mode_ run the following command,

```bash
python -m pip install -e .
```

In this mode the package appears to be installed but still is editable from the source tree.

## Usage

From the same package you can use two set of APIs.

To use the WAMR API you can import the symbols as follows,

```py
<<<<<<< HEAD
from wamr.wamr_api.wamr import Engine, Module, Instance, ExecEnv
=======
from wamr.wamrapi.wamr import Engine, Module, Instance, ExecEnv
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
```

In the order hand, to use the WASM-C-API,

```py
<<<<<<< HEAD
import wamr.wasm_c_api.ffi as ffi
=======
import wamr.wasmcapi.ffi as ffi
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
```

For more information:

* [WAMR API](./wamr_api)
* [WASM-C-API](./wasm_c_api)
