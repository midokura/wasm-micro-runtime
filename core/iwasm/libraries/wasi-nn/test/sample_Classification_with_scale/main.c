/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "nn.h"
#include "logger.h"

#undef EPSILON
#define EPSILON 1e-2

static double
time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000.0;
}

// Function to read the labels from the file and store them in an array
int
load_labels(const char *filename, char ***labels)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening label file.\n");
        return -1;
    }

    // Calculate the number of lines in the file
    int num_labels = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        num_labels++;
    }

    // Allocate memory for the labels
    *labels = (char **)malloc(num_labels * sizeof(char *));
    fseek(file, 0, SEEK_SET); // Reset the file pointer

    // Read the labels into the array
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        (*labels)[i] = strdup(buffer);     // Allocate and copy label
        i++;
    }

    fclose(file);
    return num_labels;
}

// Function to find the index of the maximum value in the output array
int
find_max_index(float *output, int num_classes)
{
    int max_index = 0;
    float max_value = output[0];

    for (int i = 1; i < num_classes; i++) {
        if (output[i] > max_value) {
            max_value = output[i];
            max_index = i;
        }
    }

    return max_index;
}

// Function to parse the inference result and print the predicted label
void
parse_inference_result(float *output, int num_classes)
{
    // Load the labels from the file
    char **labels;
    int num_labels = load_labels("label_mobile.txt", &labels);
    if (num_labels == -1) {
        printf("Error loading labels.\n");
        return;
    }

    int max_index = find_max_index(output, num_classes);
    printf("Predicted label: %s\n", labels[max_index]);

    // Free memory
    for (int i = 0; i < num_labels; i++) {
        free(labels[i]);
    }
    free(labels);
}


#define img_h 315 //h
#define img_w 474 //w
void
test_classification(execution_target target)
{
    uint32_t dim[] = { 1, img_h, img_w, 3 };//1, 315, 474, 3
    FILE *file = fopen("input_tensor_315_474.bin", "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    size_t tensor_size = 1 * img_h * img_w * 3; //

    float *input_tensor = (float *)malloc(tensor_size * sizeof(float));
    if (input_tensor == NULL) {
        perror("Memory allocation failed");
    }

    fread(input_tensor, sizeof(float), tensor_size, file);
    fclose(file);
    printf("input_tensor[0] = %f   ->0.039215688\n", input_tensor[0]);
    printf("input_tensor[1] = %f   ->0.0627451\n", input_tensor[1]);
    assert(fabs(input_tensor[0] - 0.039215688) < EPSILON);
    assert(fabs(input_tensor[1] - 0.0627451) < EPSILON);

    uint32_t output_size = 0;
    double start_time_ = time_ms();
    float *output = run_inference(target, input_tensor, dim, &output_size,
                                  "./mobilenet_v2_1.0_224.tflite", 1, fp32);
    double end_time_ = time_ms();
    printf("Classification size = 8MB, Inference time: %f ms\n", end_time_ - start_time_);

    // Parse and print the result
    parse_inference_result(output, output_size);

    free(input_tensor);
    free(output);
}
int
main()
{
    char *env = getenv("TARGET");
    if (env == NULL) {
        NN_INFO_PRINTF("Usage:\n--env=\"TARGET=[cpu|gpu|tpu]\"");
        return 1;
    }
    execution_target target;
    if (strcmp(env, "cpu") == 0)
        target = cpu;
    else if (strcmp(env, "gpu") == 0)
        target = gpu;
    else if (strcmp(env, "tpu") == 0)
        target = tpu;
    else {
        NN_ERR_PRINTF("Wrong target!");
        return 1;
    }

    NN_INFO_PRINTF("################### Testing classification model...");
    test_classification(target);

    NN_INFO_PRINTF("Tests: passed!");
    return 0;
}
