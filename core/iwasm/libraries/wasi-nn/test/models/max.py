import tensorflow as tf
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
import pathlib


model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=[5, 5, 1]),
    tf.keras.layers.MaxPooling2D(
        pool_size=(5, 5), strides=None, padding="valid", data_format=None)

])

#Export model to tflite 

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()
tflite_models_dir = pathlib.Path("./")
tflite_model_file = tflite_models_dir/"max.tflite"
tflite_model_file.write_bytes(tflite_model)
