/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

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

void
my_load(char *model_name)
{
    printf("loading model\n");
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

    printf("I copied %ld elements\n", result);

    uint32_t *size = malloc(sizeof(uint32_t));
    *size = 85000000;

    graph_builder_array arr = malloc(sizeof(graph_builder) * 2);

    arr[0] = buffer;
    arr[1] = (graph_builder)size;

    load(arr, 1);

    printf("loaded model successfully\n");
    fclose(pFile);
    free(buffer);

    return;
}

void
my_input(float *input_tensor, uint32_t *dim)
{
    printf("loading inputs\n");

    set_input(0, 0, dim, 3, input_tensor);

    printf("Success input loaded \n");
}

void
my_compute(graph_execution_context context)
{
    printf("running model \n");
    compute(context);
}

void
my_allocate(graph graph)
{
    printf("Allocating tensors \n");
    init_execution_context(graph);
}

void
my_output(graph_execution_context context, uint32_t index, float *out_tensor,
          int out_size)
{
    printf("Retrieving output \n");
    get_output(context, index, out_tensor, out_size);
}

float *
my_inference(float *input, uint32_t *input_size, int *output_size,
             char *model_name)
{
    graph graph;
    printf("end to end  \n");

    my_load(model_name);

    my_allocate(graph);

    my_input(input, input_size);

    graph_execution_context context;
    my_compute(context);

    uint32_t index;

    float *out_tensor = (float *)malloc(sizeof(float) * 5);

    my_output(context, index, out_tensor, 5 * sizeof(float));

    printf("finished\n");

    return out_tensor;
}

void
test_sum()
{
    int output_size = 0;

    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 5;
    dim[2] = 5;
    dim[3] = 1;

    float *input_tensor = malloc(INPUT_SIZE * sizeof(float));

    for (int i = 0; i < INPUT_SIZE; i++) {

        input_tensor[i] = 1;
    }

    float *output =
        my_inference(input_tensor, dim, &output_size, "models/sum.tflite");

    assert(abs(output[0] - 25.0) < EPSILON);
}

void
test_max()
{
    int output_size = 0;
    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 5;
    dim[2] = 5;
    dim[3] = 1;

    float *input_tensor = malloc(INPUT_SIZE * sizeof(float));

    for (int i = 0; i < INPUT_SIZE; i++) {

        input_tensor[i] = i;
    }

    float *output =
        my_inference(input_tensor, dim, &output_size, "models/max.tflite");

    printf("max is: %f \n ", output[0]);

    assert(abs(output[0] - 24.0) < EPSILON);
}

void
test_average()
{
    int output_size = 0;
    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 5;
    dim[2] = 5;
    dim[3] = 1;

    float *input_tensor = malloc(INPUT_SIZE * sizeof(float));

    for (int i = 0; i < INPUT_SIZE; i++) {

        input_tensor[i] = i;
    }

    float *output =
        my_inference(input_tensor, dim, &output_size, "models/average.tflite");

    printf("average is: %f \n ", output[0]);

    assert(abs(output[0] - 12.0) < EPSILON);
}

void
test_mult_dimensions()
{
    int output_size = 0;
    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 3;
    dim[2] = 3;
    dim[3] = 1;

    float *input_tensor = malloc(9 * sizeof(float));

    for (int i = 0; i < 9; i++) {

        input_tensor[i] = i;
    }

    float *output =
        my_inference(input_tensor, dim, &output_size, "models/mult_dim.tflite");

    for (int i = 0; i < 9; i++) {
        printf(" %f  \n ", output[i]);
    }
}

void
test_mult_outputs()
{
    int output_size = 0;
    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 6;
    dim[2] = 6;
    dim[3] = 1;

    float *input_tensor = malloc(INPUT_SIZE * sizeof(float));

    for (int i = 0; i < INPUT_SIZE; i++) {

        input_tensor[i] = i;
    }

    float *output =
        my_inference(input_tensor, dim, &output_size, "models/mult_out.tflite");
}

int
main()
{
    // input tensor
    test_sum();
    test_max();
    test_average();
    test_mult_dimensions();
    test_mult_outputs();

    return 0;
}
