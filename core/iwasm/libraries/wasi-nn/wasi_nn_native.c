#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "wasm_export.h"

#include "wasi_nn_tensorflow.hpp"
#include "logger.h"

static uint8_t is_initialized;
static graph_encoding _encoding;

typedef struct {
    uint32_t buf_offset;
    uint32_t size;  
} graph_builder_wasm;

typedef struct {
    uint32_t buf_offset;
    uint32_t size;
} graph_builder_array_wasm;

typedef struct {
    uint32_t dimensions_offset;
    tensor_type type;
    uint32_t data_offset;
} tensor_wasm;

typedef struct {
    uint32_t buf_offset;
    uint32_t size;
} tensor_dimensions_wasm;

// uint32_t
// flatten_dimensions(uint32_t *dim)
// {
//     uint32_t res = 0;

//     for (int i = 0; i < DIM_SIZE; i++) {
//         res *= dim[i];
//     }

//     return res;
// }

/* WASI-NN implementation */

uint32_t
wasi_nn_load(wasm_exec_env_t exec_env, graph_builder_array_wasm *builder, graph_encoding encoding, execution_target target, graph *graph)
{
    wasm_module_inst_t instance = wasm_runtime_get_module_inst(exec_env);

    if (!wasm_runtime_validate_native_addr(instance, builder, sizeof(graph_builder_array_wasm)))
        return invalid_argument;

    if (!wasm_runtime_validate_app_addr(instance, builder->buf_offset, builder->size*sizeof(graph_builder)))
        return invalid_argument;
    NN_DBG_PRINTF("Graph builder array contains %d elements", builder->size);

    graph_builder *gb_n = (graph_builder*) malloc(builder->size*sizeof(graph_builder));

    graph_builder_wasm *gb_w = (graph_builder_wasm*) wasm_runtime_addr_app_to_native(instance, builder->buf_offset);
    for (int i = 0; i < builder->size; ++i) {
        if (!wasm_runtime_validate_app_addr(instance, gb_w[i].buf_offset, gb_w[i].size*sizeof(uint8_t)))
            return invalid_argument;

        gb_n[i].buf = (uint8_t*) wasm_runtime_addr_app_to_native(instance, gb_w[i].buf_offset);
        gb_n[i].size = gb_w[i].size;

        NN_DBG_PRINTF("Graph builder %d contains %d elements", i,  gb_w[i].size);
    }

    graph_builder_array gba_n = {.buf = gb_n, .size = builder->size};

    if (!wasm_runtime_validate_native_addr(instance, graph, sizeof(graph)))
        return invalid_argument;

    NN_DBG_PRINTF("Encoding: %d", encoding);
    NN_DBG_PRINTF("Target: %d", target);
    NN_DBG_PRINTF("Graph: %d", *graph);

    switch (encoding) {
        case tensorflow:
            break;
        default:
            return invalid_argument;
    }

    _encoding = encoding;
    is_initialized = 1;

    error ret = _load(gba_n, _encoding, target, graph);
    free(gb_n);
    return ret;
}

uint32_t
wasi_nn_init_execution_context(wasm_exec_env_t exec_env, graph graph, graph_execution_context *ctx)
{
    NN_DBG_PRINTF("here: %d", graph);
    *ctx = graph;
    return _init_execution_context(graph);
}

uint32_t
wasi_nn_set_input(wasm_exec_env_t exec_env, graph_execution_context ctx, uint32_t index, tensor_wasm *t)
{
    wasm_module_inst_t instance = wasm_runtime_get_module_inst(exec_env);
    NN_DBG_PRINTF("ctx: %d", ctx);
    NN_DBG_PRINTF("index: %d", index);

    if (!wasm_runtime_validate_native_addr(instance, t, sizeof(tensor_wasm)))
        return invalid_argument;

    NN_DBG_PRINTF("aaaa");
    if (!wasm_runtime_validate_app_addr(instance, t->dimensions_offset, sizeof(uint32_t)))
        return invalid_argument;

    NN_DBG_PRINTF("aaaa");
    tensor_dimensions_wasm *dimensions_w = (tensor_dimensions_wasm*) wasm_runtime_addr_app_to_native(instance, t->dimensions_offset);
    NN_DBG_PRINTF("dimensions: %d", dimensions_w->size);

    if (!wasm_runtime_validate_app_addr(instance, dimensions_w->buf_offset, dimensions_w->size*sizeof(uint32_t)))
        return invalid_argument;
    
    tensor_dimensions dimensions = {
        .buf = (uint32_t*) wasm_runtime_addr_app_to_native(instance, dimensions_w->buf_offset),
        .size = dimensions_w->size
    };

    int total_elements = 1;
    for (int i = 0; i < dimensions.size; ++i) {
        NN_DBG_PRINTF("dimension %d: %d", i, dimensions.buf[i]);
        total_elements *= dimensions.buf[i];
    }

    NN_DBG_PRINTF("total elements %d", total_elements);
    if (!wasm_runtime_validate_app_addr(instance, t->data_offset, total_elements))
        return invalid_argument;

    NN_DBG_PRINTF("aaaa");
    tensor tensor = {
        .type = t->type,
        .dimensions = &dimensions,
        .data = (uint8_t*) wasm_runtime_addr_app_to_native(instance, t->data_offset)
    };

    return _set_input(ctx, index, tensor);
}

uint32_t
wasi_nn_compute(wasm_exec_env_t exec_env, graph_execution_context ctx)
{
    return _compute(ctx);
}

// uint32_t
// wasi_nn_get_output(wasm_exec_env_t exec_env, graph_execution_context context,
//                    uint32_t index, uint8_t *out_buffer,
//                    buffer_size out_buffer_max_size)
// {
//     _get_output(context, index, out_buffer, out_buffer_max_size);

//     return success;
// }

/* Register native symbols and utility */

/* clang-format off */
#define REG_NATIVE_FUNC(func_name, signature) \
    { #func_name, wasi_nn_##func_name, signature, NULL }
/* clang-format on */

static NativeSymbol native_symbols_wasi_nn[] = {
    REG_NATIVE_FUNC(load, "(*ii*)i"),
    REG_NATIVE_FUNC(init_execution_context, "(i*)i"),
    REG_NATIVE_FUNC(set_input, "(ii*)i"),
    REG_NATIVE_FUNC(compute, "(i)i"),
    // REG_NATIVE_FUNC(get_output, "(ii*~)i"),
};

uint32_t
get_wasi_nn_export_apis(NativeSymbol **p_libc_wasi_apis)
{
    *p_libc_wasi_apis = native_symbols_wasi_nn;
    return sizeof(native_symbols_wasi_nn) / sizeof(NativeSymbol);
}
