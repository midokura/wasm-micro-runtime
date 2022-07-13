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

#define MODEL_NAME "test.tflite"

#define EPSILON 1e-8

void
my_load()
{
    printf("loading model\n");
    FILE *pFile = fopen(MODEL_NAME, "r");

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
my_input()
{
    printf("loading inputs\n");

    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 5;
    dim[2] = 5;
    dim[3] = 1;

    float *input_tensor = malloc(INPUT_SIZE * sizeof(float));

    for (int i = 0; i < INPUT_SIZE; i++) {

        input_tensor[i] = 1;
    }

    set_input(0, 0, dim, 3, input_tensor);

    printf("Success input loaded \n");
}

void
my_inference(graph_execution_context context)
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
my_output(graph_execution_context context, uint32_t index, float *out_tensor, int out_size)
{
    printf("Retrieving output \n");
    get_output(context, index, out_tensor, out_size);
}

float *
infer(float *input, int input_size, int * output_size)
{
    graph graph;
    printf("end to end  \n");

    my_load();

    my_allocate(graph);

    my_input();

    graph_execution_context context;
    my_inference(context);

    uint32_t index;

    float *out_tensor= (float *)malloc(sizeof(float) * 1);


    my_output(context, index, out_tensor,1* sizeof(float) );

    printf("wasm is: %f \n", out_tensor[0]);

    printf("finished\n");
}

void
test_sum()
{
    int output_size=0; 

    //my_inference( &output_size);

    // assert( abs(out-25.0) < EPSILON);

    // assert (output_size == 1);

    //printf("");
}

void
test_max()
{
}

void
test_average()
{

}

int
main()
{
    // input tensor

    float *input;
    int input_size;
    int * output_size;

    infer(input,  input_size, output_size);

    // retrieve output tensor

    return 0;
}
