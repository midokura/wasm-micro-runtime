#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>
#include "wasi_nn.h"

#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 32

#define INPUT_SIZE 25

#define EPSILON 1e-8
#define MAX_OUTPUT_TENSOR_SIZE 200
#define MAX_MODEL_SIZE 85000000

typedef struct {
    float *input_tensor;
    uint32_t *dim;
    uint32_t elements;
} input_info;

void
my_load(char *model_name, graph *graph)
{
    FILE *pFile = fopen(model_name, "r");

    assert(pFile != NULL);

    uint8_t *buffer;
    size_t result;

    int lsize = 85000000;

    // allocate memory to contain the whole file:
    buffer = (uint8_t *)malloc(sizeof(uint8_t) * lsize);

    if (buffer == NULL) {
        fputs("Memory error\n", stderr);
        exit(2);
    }

    result = fread(buffer, 1, lsize, pFile);

    graph_builder_array arr;

    arr.buf = (graph_builder_array*) malloc(sizeof(graph_builder));
    arr.size = 1;

    arr.buf[0].buf = buffer;
    arr.buf[0].size = result;

    load(&arr, tensorflow, cpu, graph);

    fclose(pFile);
    free(buffer);
}

void
my_input(float *input_tensor, uint32_t *dim)
{
    tensor_dimensions dims;
    dims.size = 4;
    dims.buf = (uint32_t*)malloc(dims.size*sizeof(uint32_t));

    tensor tensor;
    tensor.dimensions = &dims;
    for (int i = 0; i < tensor.dimensions->size; ++i)
        tensor.dimensions->buf[i] = dim[i];
    tensor.type = fp32;
    tensor.data = input_tensor;
    set_input(44, 0, &tensor);

    free(tensor.dimensions->buf);
}

void
my_compute(graph_execution_context context)
{
    compute(context);
}

void
my_allocate(graph graph)
{
    graph_execution_context gec;
    init_execution_context(graph, &gec);
}

void
my_output(graph_execution_context context, uint32_t index, float *out_tensor,
          uint32_t *out_size)
{
    get_output(context, index, out_tensor, out_size);
}

float *
my_inference(float *input, uint32_t *input_size, int *output_size,
             char *model_name)
{
    graph graph = 444;
    my_load(model_name, &graph);

    my_allocate(graph);

    my_input(input, input_size);

    graph_execution_context context;
    my_compute(context);

    float *out_tensor = (float *)malloc(sizeof(float) * MAX_OUTPUT_TENSOR_SIZE);

    uint32_t index;
    uint32_t out_size = MAX_OUTPUT_TENSOR_SIZE;
    my_output(context, index, out_tensor, &out_size);
    return out_tensor;
}

// UTILS

input_info
create_input(int N, int *dims)
{
    input_info input = { .dim = NULL, .input_tensor = NULL, .elements = 1 };

    input.dim = malloc(N * sizeof(uint32_t));
    for (int i = 0; i < N; ++i) {
        input.dim[i] = dims[i];
        input.elements *= dims[i];
    }

    input.input_tensor = malloc(input.elements * sizeof(float));
    for (int i = 0; i < input.elements; ++i)
        input.input_tensor[i] = i;

    return input;
}

// TESTS

void
test_sum()
{
    int N = 4;
    int dims[] = { 1, 5, 5, 1 };
    input_info input = create_input(4, dims);

    int output_size = 0;
    float *output = my_inference(input.input_tensor, input.dim, &output_size,
                                 "models/sum.tflite");

    // TODO: assert(output_size == 1);
    assert(abs(output[0] - 300.0) < EPSILON);

    free(input.dim);
    free(input.input_tensor);
    free(output);
}

void
test_max()
{
    int N = 4;
    int dims[] = { 1, 5, 5, 1 };
    input_info input = create_input(4, dims);

    int output_size = 0;
    float *output = my_inference(input.input_tensor, input.dim, &output_size,
                                 "models/max.tflite");

    // TODO: assert(output_size == 1);
    assert(abs(output[0] - 24.0) < EPSILON);
    printf("Result: max is %f\n", output[0]);

    free(input.dim);
    free(input.input_tensor);
    free(output);
}

void
test_average()
{
    int N = 4;
    int dims[] = { 1, 5, 5, 1 };
    input_info input = create_input(4, dims);

    int output_size = 0;
    float *output = my_inference(input.input_tensor, input.dim, &output_size,
                                 "models/average.tflite");

    // TODO: assert(output_size == 1);
    assert(abs(output[0] - 12.0) < EPSILON);
    printf("Result: average is %f\n", output[0]);

    free(input.dim);
    free(input.input_tensor);
    free(output);
}

void
test_mult_dimensions()
{
    int N = 4;
    int dims[] = { 1, 3, 3, 1 };
    input_info input = create_input(4, dims);

    int output_size = 0;
    float *output = my_inference(input.input_tensor, input.dim, &output_size,
                                 "models/mult_dim.tflite");

    // TODO: assert(output_size == 9);
    for (int i = 0; i < 9; i++)
        assert(abs(output[i] - i) < EPSILON);

    free(input.dim);
    free(input.input_tensor);
    free(output);
}

void
test_mult_outputs()
{
    int N = 4;
    int dims[] = { 1, 4, 4, 1 };
    input_info input = create_input(4, dims);

    int output_size = 0;
    float *output = my_inference(input.input_tensor, input.dim, &output_size,
                                 "models/mult_out.tflite");

    // TODO: assert(output_size == 8);
    // first tensor check
    for (int i = 0; i < 4; i++)
        assert(abs(output[i] - (i*4 + 24)) < EPSILON);
    // second tensor check
    for (int i = 0; i < 4; i++)
        assert(abs(output[i+4] - (i + 6)) < EPSILON);

    free(input.dim);
    free(input.input_tensor);
    free(output);
}

int
main()
{
    printf("################### Testing sum...\n");
    test_sum();
    printf("################### Testing max...\n");
    test_max();
    printf("################### Testing average...\n");
    test_average();
    printf("################### Testing multiple dimensions...\n");
    test_mult_dimensions();
    printf("################### Testing multiple outputs...\n");
    test_mult_outputs();

    printf("Tests: passed!\n");
    return 0;
}
