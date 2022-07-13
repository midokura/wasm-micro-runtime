#include "wasi_nn_tensorflow.hpp"

#include <iostream>

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/error_reporter.h>

enum Idx { GRAPH = 0, GRAPH_SIZE = 1 };

#define DIM_SIZE 4

std::unique_ptr<tflite::Interpreter> interpreter = NULL;
std::unique_ptr<tflite::FlatBufferModel> model = NULL;

uint32_t
_load(graph_builder_array builder, graph_encoding encoding)
{

    if (encoding != tensorflow) {
        return invalid_argument;
    }

    uint32_t *size = (uint32_t *)builder[Idx::GRAPH_SIZE];

    tflite::ErrorReporter *error_reporter;

    model = tflite::FlatBufferModel::BuildFromBuffer(
        (const char *)builder[Idx::GRAPH], *size, error_reporter);

    if (model == nullptr) {
        printf("failure: null model \n");
        return missing_memory;
    }

    // Build the interpreter with the InterpreterBuilder.
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder tflite_builder(*model, resolver);
    tflite_builder(&interpreter);

    if (interpreter == nullptr) {
        printf("failure: null interpreter \n");
        return missing_memory;
    }

    return success;
}

uint32_t
_set_input(tensor input_tensor)
{
    auto *input = interpreter->typed_input_tensor<float>(0);

    if (input == nullptr) {
        return missing_memory;
    }

    // Recomputes the dimensions each time, not optimal but have to follow witx
    // for now

    int max_size = 1;

    for (int i = 0; i < DIM_SIZE; i++) {
        max_size *= input_tensor.dimensions[i];
    }

    float* float_input = (float*) input_tensor.data;

    for (int i = 0; i < max_size; i++) {
        input[i] =  float_input[i];
    }

    return success;
}

uint32_t
_compute(graph_execution_context context)
{
    interpreter->Invoke();

    return success;
}

uint32_t
_get_output(graph_execution_context context, uint32_t index,
            uint8_t *out_tensor, buffer_size out_size)
{
    auto* out = interpreter->typed_output_tensor<float>(0);

    if (out==NULL)
    {
        printf("missing memory\n");
        return missing_memory;
    }
    printf("native is : %f \n  size: %d \n" ,out[0] ,out_size);

    memcpy(out_tensor, out, out_size);
    
    return success;
}

uint32_t
_init_execution_context(graph graph)
{
    interpreter->AllocateTensors();
    return success;
}