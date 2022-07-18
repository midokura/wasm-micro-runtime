import tensorflow as tf
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from utils import save_model

model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=[5, 5, 1]),
    tf.keras.layers.AveragePooling2D(
        pool_size=(5, 5), strides=None, padding="valid", data_format=None)

])

# Export model to tflite

save_model(model, "average.tflite")
