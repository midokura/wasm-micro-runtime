import tensorflow as tf
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils
from utils import save_model

model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=[3, 3, 1]),
    tf.keras.layers.Conv2D(1, (1, 1), kernel_initializer=tf.keras.initializers.Constant(
        value=1), bias_initializer='zeros'
    )
])
# Export model to tflite

save_model(model, "mult_dim.tflite")
