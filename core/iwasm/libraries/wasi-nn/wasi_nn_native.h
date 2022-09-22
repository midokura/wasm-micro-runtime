#ifndef WASI_NN_NATIVE_H
#define WASI_NN_NATIVE_H

#include <stdint.h>
#include "wasi_nn_common.h"

typedef struct {
    uint32_t dimensions;
    uint32_t tensor_type;
    uint32_t data;
} tensor;

error
load(uint32_t builder, uint32_t encoding, uint32_t target, uint32_t graph);

error
init_execution_context(uint32_t graph, uint32_t ctx);

error
set_input(uint32_t ctx, uint32_t index, uint32_t tensor);

error
compute(uint32_t ctx);

error
get_output(uint32_t ctx, uint32_t index, uint32_t tensor_data, uint32_t tensor_data_size);

#endif
