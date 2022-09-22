#ifndef WASI_NN_COMMON_H
#define WASI_NN_COMMON_H

// The type of the elements in a tensor.
typedef enum {
    fp16 = 0,
    fp32,
    up8,
    ip32
} tensor_type;

// Describes the encoding of the graph. This allows the API to be implemented by various backends
// that encode (i.e., serialize) their graph IR with different formats.
typedef enum {
    openvino = 0,
    onnx,
    tensorflow,
    pytorch
} graph_encoding;

// Define where the graph should be executed.
typedef enum {
    cpu = 0,
    gpu,
    tpu
} execution_target;

// Error codes returned by functions in this API.
typedef enum { 
    // No error occurred.
    success = 0,
    // Caller module passed an invalid argument.
    invalid_argument,
    // Invalid encoding.
    invalid_encoding,
    // Caller module is missing a memory export.
    missing_memory,
    // Device or resource busy.
    busy,
    // Runtime Error.
    runtime_error,
} error;

#endif
