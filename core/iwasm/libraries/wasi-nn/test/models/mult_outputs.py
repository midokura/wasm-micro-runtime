import tensorflow as tf
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D, AveragePooling2D, Conv2D
from keras.utils import np_utils

from tensorflow.keras import Input, Model

import pathlib

inputs = Input(shape=(6, 6, 1))

output1 = Conv2D(1, (2, 2), kernel_initializer=tf.keras.initializers.Constant(
        value=1), bias_initializer='zeros'
                           )(inputs)
output2 = AveragePooling2D(pool_size=(
    2, 2), strides=None, padding="valid", data_format=None)(inputs)

model = Model(inputs=inputs, outputs=[output1, output2])

#inp = np.ones((1, 6, 6, 1))

inp=np.arange(36).reshape((1, 6, 6, 1))

print (inp)

res = model.predict(inp)

print(res)

converter = tf.lite.TFLiteConverter.from_keras_model(model)

tflite_model = converter.convert()

tflite_models_dir = pathlib.Path("./")

tflite_model_file = tflite_models_dir/"mult_out.tflite"
tflite_model_file.write_bytes(tflite_model)
