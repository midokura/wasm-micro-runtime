import tensorflow as tf
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils

from tensorflow.keras import Input, Model

import pathlib

inputs = Input(shape=(5,5,1))

output1 = Dense(1)(inputs)
output2 = Dense(3, activation='softmax')(inputs)

model = Model(inputs=inputs, outputs=[output1, output2])

inp = np.ones((1, 5, 5, 1))

res = model.predict(inp)

print(res)

converter=tf.lite.TFLiteConverter.from_keras_model(model)

tflite_model=converter.convert()

tflite_models_dir=pathlib.Path("./")

tflite_model_file=tflite_models_dir/"multiple_out.tflite"
tflite_model_file.write_bytes(tflite_model)
