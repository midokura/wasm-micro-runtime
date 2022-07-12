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

void
test_load()
{
    printf("loading model\n");
    FILE *pFile = fopen("test.tflite", "r");

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
test_input()
{
    printf("loading inputs\n");

    uint32_t *dim = malloc(4 * sizeof(uint32_t));

    dim[0] = 1;
    dim[1] = 5;
    dim[2] = 5;
    dim[3] = 1;

    uint8_t *input_tensor = malloc(25 * sizeof(uint8_t));

    for (int i = 0; i < 25; i++) {

        input_tensor[i] = 1;
    }

    set_input(0, 0, dim, 3, input_tensor);

    printf("Success input loaded \n");
}

void
test_inference(graph_execution_context context)
{
    printf("running model \n");
    compute(context);
}

void
test_output(graph_execution_context context, uint32_t index,
            uint8_t *out_tensor)
{
    get_output(context, index, out_tensor, 1);
}
int
main()
{
    printf("unit tests \n");

    test_load();

    test_input();

    graph_execution_context context;
    test_inference(context);

    uint32_t index;

    uint8_t out_tensor[1];

    test_output(context, index, out_tensor);

    printf("%d \n", out_tensor[0]);

    printf("finished\n");

    return 0;
}
