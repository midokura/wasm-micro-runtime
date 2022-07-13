import tensorflow as tf
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils

import pathlib


model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=[5, 5, 1]),
    tf.keras.layers.Conv2D(1, (5, 5), kernel_initializer=tf.keras.initializers.Constant(
        value=1), bias_initializer='zeros'
                           )
])

inp=np.ones((1, 5, 5, 1))


res=model.predict(inp)

print(res)


converter=tf.lite.TFLiteConverter.from_keras_model(model)

tflite_model=converter.convert()

tflite_models_dir=pathlib.Path("./")

tflite_model_file=tflite_models_dir/"test.tflite"
tflite_model_file.write_bytes(tflite_model)
