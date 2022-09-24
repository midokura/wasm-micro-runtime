#include "wasi_nn_tensorflow.hpp"

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/error_reporter.h>

enum Idx { GRAPH = 0, GRAPH_SIZE = 1 };

#define DIM_SIZE 4

std::unique_ptr<tflite::Interpreter> interpreter = NULL;
std::unique_ptr<tflite::FlatBufferModel> model = NULL;

char *model_pointer = NULL;

error
_load(graph_builder_array builder, graph_encoding encoding,
      execution_target target, graph *graph)
{
    if (encoding != tensorflow) {
        return invalid_argument;
    }

    assert(builder.size == 1);

    tflite::ErrorReporter *error_reporter;

    int size = builder.buf[0].size;
    model_pointer = (char *)malloc(size);
    memcpy(model_pointer, builder.buf[0].buf, size);

    model = tflite::FlatBufferModel::BuildFromBuffer(model_pointer, size,
                                                     error_reporter);
    if (model == NULL) {
        NN_ERR_PRINTF("failure: null model. error reported: %s",
                      error_reporter);
        return missing_memory;
    }

    // Build the interpreter with the InterpreterBuilder.
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder tflite_builder(*model, resolver);
    tflite_builder(&interpreter);
    if (interpreter == NULL) {
        NN_ERR_PRINTF("failure: null interpreter.");
        return missing_memory;
    }

    return success;
}

error
_init_execution_context(graph graph)
{
    interpreter->AllocateTensors();
    return success;
}

error
_set_input(graph_execution_context ctx, uint32_t index, tensor *t)
{
    auto *input = interpreter->typed_input_tensor<float>(0);

    if (input == NULL)
        return missing_memory;

    int max_size = 1;
    for (int i = 0; i < t->dimensions->size; i++) {
        max_size *= t->dimensions->buf[i];
    }

    float *float_input = (float *)t->data;
    for (int i = 0; i < max_size; i++)
        input[i] = float_input[i];

    return success;
}

error
_compute(graph_execution_context ctx)
{
    interpreter->Invoke();

    return success;
}

error
_get_output(graph_execution_context context, uint32_t index, tensor_data data,
            uint32_t *data_size)
{
    int num_output_tensors = interpreter->outputs().size();
    NN_DBG_PRINTF("Output tensor max size (%ld), number of tensors (%ld)",
                  *data_size, num_output_tensors);

    uint32_t elems[num_output_tensors];
    uint32_t total_elems = 0;

    for (int i = 0; i < num_output_tensors; ++i) {
        auto tensor = interpreter->output_tensor(i);

        if (tensor == NULL) {
            NN_ERR_PRINTF("missing memory");
            return missing_memory;
        }

        uint32_t n = 1;
        for (int j = 0; j < (int)tensor->dims->size; ++j) {
            n *= (uint32_t)tensor->dims->data[j];
        }
        total_elems += n;

        elems[i] = n;
    }

    int offset = 0;
    for (int i = 0; i < num_output_tensors; ++i) {
        int dims = (int)elems[i];
        assert(offset + dims <= data_size);
        float *tensor = interpreter->typed_output_tensor<float>(i);
        for (int j = 0; j < dims; j++) {
            NN_DBG_PRINTF("output: %f", tensor[j]);
        }
        memcpy(&data[offset * sizeof(float)], tensor, sizeof(float) * dims);
        offset += dims;
    }
    *data_size = total_elems;
    return success;
}
