# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

from ctypes import CFUNCTYPE, Array, byref
from ctypes import c_char
from ctypes import c_uint
from ctypes import c_uint8
from ctypes import c_int32
from ctypes import c_char_p
from ctypes import c_void_p
from ctypes import cast
from ctypes import create_string_buffer
from ctypes import POINTER
from ctypes import pointer
from typing import Any
from wamr.wamr_api.iwasm import String

from wamr.wamr_api.iwasm import Alloc_With_Pool
from wamr.wamr_api.iwasm import RuntimeInitArgs
from wamr.wamr_api.iwasm import wasm_application_execute_main
from wamr.wamr_api.iwasm import wasm_exec_env_t
from wamr.wamr_api.iwasm import wasm_function_inst_t
from wamr.wamr_api.iwasm import wasm_module_inst_t
from wamr.wamr_api.iwasm import wasm_module_t
from wamr.wamr_api.iwasm import wasm_runtime_call_indirect  # XXX
from wamr.wamr_api.iwasm import wasm_runtime_call_wasm
from wamr.wamr_api.iwasm import wasm_runtime_create_exec_env
from wamr.wamr_api.iwasm import wasm_runtime_deinstantiate
from wamr.wamr_api.iwasm import wasm_runtime_destroy
from wamr.wamr_api.iwasm import wasm_runtime_destroy_exec_env
from wamr.wamr_api.iwasm import wasm_runtime_full_init
from wamr.wamr_api.iwasm import wasm_runtime_get_module_inst
from wamr.wamr_api.iwasm import wasm_runtime_instantiate
from wamr.wamr_api.iwasm import wasm_runtime_load
from wamr.wamr_api.iwasm import wasm_runtime_lookup_function
from wamr.wamr_api.iwasm import wasm_runtime_module_malloc
from wamr.wamr_api.iwasm import wasm_runtime_unload
from wamr.wamr_api.iwasm import NativeSymbol

class Engine:
    def __init__(self, native_symbols):
        self.init_args = self._get_init_args(native_symbols)
        wasm_runtime_full_init(pointer(self.init_args))

    def __del__(self):
        print("deleting Engine")
        wasm_runtime_destroy()

    def _get_init_args(self, native_symbols, heap_size: int = 1024 * 512) -> RuntimeInitArgs:
        init_args = RuntimeInitArgs()
        init_args.mem_alloc_type = Alloc_With_Pool
        init_args.mem_alloc_option.pool.heap_buf = cast(
            (c_char * heap_size)(), c_void_p
        )
        init_args.mem_alloc_option.pool.heap_size = heap_size

        init_args.n_native_symbols = len(native_symbols)
        init_args.native_module_name = String.from_param("env")
        init_args.native_symbols = cast(native_symbols, POINTER(NativeSymbol))

        return init_args

class Module:
    __create_key = object()

    @classmethod
    def from_file(cls, engine: Engine, fp: str) -> "Module":
        return Module(cls.__create_key, engine, fp)

    def __init__(self, create_key: object, engine: Engine, fp: str) -> None:
        assert (
            create_key == Module.__create_key
        ), "Module objects must be created using Module.from_file"
        self.engine = engine
        self.module, self.file_data = self._create_module(fp)

    def __del__(self):
        print("deleting Module")
        wasm_runtime_unload(self.module)

    def _create_module(self, fp: str) -> tuple[wasm_module_t, Array[c_uint]]:
        with open(fp, "rb") as f:
            data = f.read()
            data = (c_uint8 * len(data))(*data)

        error_buf = create_string_buffer(128)
        module = wasm_runtime_load(data, len(data), error_buf, len(error_buf))
        if not module:
            raise Exception("Error while creating module")
        return module, data


class Instance:
    def __init__(self, module: Module):
        if module is not None:
            self.module = module
            self.module_inst = self._create_module_inst(module)
            self.own_c = True

    @classmethod
    def wrap(cls, c_inst: wasm_module_inst_t):
        i = cls(None)
        i.module = None
        i.module_inst = cast(c_inst, wasm_module_inst_t)
        i.own_c = False
        return i

    def __del__(self):
        if self.own_c:
            print("deleting Instance")
            wasm_runtime_deinstantiate(self.module_inst)

    def malloc(self, nbytes: int, native_handler) -> c_uint:
        return wasm_runtime_module_malloc(self.module_inst, nbytes, native_handler)

    def lookup_function(self, name: str) -> wasm_function_inst_t:
        func = wasm_runtime_lookup_function(self.module_inst, name, None)
        if not func:
            raise Exception("Error while looking-up function")
        return func

    def _create_module_inst(self, module: Module) -> wasm_module_inst_t:
        error_buf = create_string_buffer(128)
        module_inst = wasm_runtime_instantiate(
            module.module, 8092, 8092, error_buf, len(error_buf)
        )
        if not module_inst:
            raise Exception("Error while creating module instance")
        return module_inst

    def application_execute_main(self, args) -> bool:
        # TODO convert args
        c_ret = wasm_application_execute_main(self.module_inst, 0, None)
        return bool(c_ret)


class ExecEnv:
    def __init__(self, module_inst: Instance):
        if module_inst is not None:
            self.exec_env = self._create_exec_env(module_inst)
            self.module_inst = module_ist
            self.own_c = True

    @classmethod
    def wrap(cls, c_env: wasm_exec_env_t):
        i = cls(None)
        i.exec_env = cast(c_env, wasm_exec_env_t)
        i.module_inst = None
        i.own_c = False
        return i

    def get_module_inst(self):
        if self.module_inst is not None:
            return self.module_inst
        return Instance.wrap(wasm_runtime_get_module_inst(self.exec_env))

    def __del__(self):
        if self.own_c:
            print("deleting ExecEnv")
            wasm_runtime_destroy_exec_env(self.exec_env)

    def call(self, func: wasm_function_inst_t, argc: int, argv: "POINTER[c_uint]"):
        if not wasm_runtime_call_wasm(self.exec_env, func, argc, argv):
            raise Exception("Error while calling function")

    def call_indirect(self, elem_idx: int, argc: int, argv: "POINTER[c_uint]"):
        if not wasm_runtime_call_indirect(self.exec_env, elem_idx, argc, argv):
            raise Exception("Error while calling function")

    def _create_exec_env(self, module_inst: Instance) -> wasm_exec_env_t:
        exec_env = wasm_runtime_create_exec_env(module_inst.module_inst, 8092)
        if not exec_env:
            raise Exception("Error while creating execution environment")
        return exec_env
