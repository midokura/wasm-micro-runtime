#ifndef WASI_NN_WASM_H
#define WASI_NN_WASM_H

#include <stdint.h>
#include "wasi_nn_common.h"

/**
 * Following definition from:
 * [Apr 6, 2022]
 * https://github.com/WebAssembly/wasi-nn/blob/c557b2e9f84b6630f13b3185b43607f0388343b2/phases/ephemeral/witx/wasi_ephemeral_nn.witx
 */

/* The dimensions of a tensor. */

// The array length matches the tensor rank and each element in the array
// describes the size of each dimension.
typedef uint32_t *tensor_dimensions;

/* The tensor data. */

// Initially conceived as a sparse representation, each empty cell would be filled with zeros and
// the array length must match the product of all of the dimensions and the number of bytes in the
// type (e.g., a 2x2 tensor with 4-byte f32 elements would have a data array of length 16).
// Naturally, this representation requires some knowledge of how to lay out data in memory--e.g.,
// using row-major ordering--and could perhaps be improved.
typedef uint8_t *tensor_data;

/* A tensor. */

typedef struct {
    // Describe the size of the tensor (e.g., 2x2x2x2 -> [2, 2, 2, 2]). To represent a tensor
    // containing a single value, use `[1]` for the tensor dimensions.
    tensor_dimensions dimensions;
    // Describe the type of element in the tensor (e.g., f32).
    tensor_type tensor_type;
    // Contains the tensor data.
    tensor_data data;
} tensor;

/* The graph initialization data. */

// This consists of an array of buffers because implementing backends may encode their graph IR in
// parts (e.g., OpenVINO stores its IR and weights separately).
typedef uint8_t *graph_builder;
typedef graph_builder *graph_builder_array;

// An execution graph for performing inference (i.e., a model).
typedef uint32_t graph;

// Bind a `graph` to the input and output tensors for an inference.
typedef uint32_t graph_execution_context;

/**
 * @brief Load an opaque sequence of bytes to use for inference.
 * 
 * @param builder   Builder of the model.
 * @param encoding  Type of encoding of the model.
 * @param target    Execution target.
 * @param graph     Graph.
 * @return error    Execution status.
 */
error
load(graph_builder_array builder, graph_encoding encoding, execution_target target, graph graph);

/**
 * @brief Create an execution instance of a loaded graph.
 * 
 * @param graph                     Initialialize execution context of graph `graph`.
 * @param graph_execution_context   Graph execution context.
 * @return error
 */
error
init_execution_context(graph graph, graph_execution_context ctx);

/**
 * @brief Define the inputs to use for inference.
 * 
 * @param ctx       Execution context.
 * @param index     Input tensor index.
 * @param tensor    Input tensor. 
 * @return error    Execution status.
 */
error
set_input(graph_execution_context ctx, uint32_t index, tensor tensor);


/**
 * @brief Compute the inference on the given inputs.
 * 
 * @param ctx       Execution context.
 * @return error    Execution status.
 */
error
compute(graph_execution_context ctx);

/**
 * @brief Extract the outputs after inference.
 * 
 * @param ctx               Execution context.
 * @param index             Index of the output tensor.
 * @param tensor_data       Address 
 * @param tensor_data_size  `tensor_data` maximum size.
 * @return error            Execution status.
 */
error
get_output(graph_execution_context ctx, uint32_t index, tensor_data tensor_data, uint32_t tensor_data_size);

#endif
