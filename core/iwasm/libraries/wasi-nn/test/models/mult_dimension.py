import tensorflow as tf
from utils import save_model

model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=[3, 3, 1]),
    tf.keras.layers.Conv2D(1, (1, 1), kernel_initializer=tf.keras.initializers.Constant(
        value=1), bias_initializer='zeros'
    )
])
# Export model to tflite

save_model(model, "mult_dim.tflite")
