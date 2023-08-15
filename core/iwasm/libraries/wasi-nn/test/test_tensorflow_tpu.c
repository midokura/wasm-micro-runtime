/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "logger.h"

#undef EPSILON
#define EPSILON 1e-2

void
test_average_quantized(execution_target target)
{
    int dims[] = { 1, 5, 5, 1 };
    input_info input = create_input(dims);

    uint32_t output_size = 0;
    float *output =
        run_inference(target, input.input_tensor, input.dim, &output_size,
                      "/assets/models/quantized_model.tflite", 1);

    NN_INFO_PRINTF("Output size: %d", output_size);
    NN_INFO_PRINTF("Result: average is %f", output[0]);
    // NOTE: 11.95 instead of 12 because of errors due quantization
    assert(fabs(output[0] - 11.95) < EPSILON);

    free(input.dim);
    free(input.input_tensor);
    free(output);
}

int
main()
{
    NN_INFO_PRINTF("################### Testing quantized model...");
    test_average_quantized(tpu);

    NN_INFO_PRINTF("Tests: passed!");
    return 0;
}