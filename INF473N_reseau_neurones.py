import os

import tensorflow as tf
from tensorflow import keras

#data importing -> mnist to test
mnist = tf.keras.datasets.mnist

(X_train, Y_train), (X_test, Y_test) = mnist.load_data()

Y_train = Y_train[:1000]
Y_test = Y_test[:1000]

X_train = X_train[:1000].reshape(-1, 28 * 28) / 255.0
X_test = X_test[:1000].reshape(-1, 28 * 28) / 255.0

# returns a short sequential model
def create_model():
    # activation function: leaky ReLU
    leakyrelu = lambda x: tf.keras.activations.relu(x, alpha=0.01, max_value=None, threshold=0)

    # creation du reseau de neurones
    model = tf.keras.models.Sequential([

        # hidden layer
        keras.layers.Dense(units = 512, activation = leakyrelu, input_shape=(784,)),
        keras.layers.Dropout(rate=0.2),
        keras.layers.Dense(units = 105, activation = leakyrelu),
        keras.layers.Dropout(rate=0.2),

        # final layer
        keras.layers.Dense(units = 10, activation = tf.nn.softmax),
    ])

    model.compile(optimizer=tf.keras.optimizers.Adam(),
                  loss=tf.keras.losses.sparse_categorical_crossentropy,
                  metrics=['accuracy'])

    return model

# Create checkppoints
checkpoint_path = "checkpoints/cp-{epoch:04d}.ckpt"
checkpoint_dir = os.path.dirname(checkpoint_path)

cp_callback = tf.keras.callbacks.ModelCheckpoint(
    checkpoint_path, save_weights_only=True, verbose=1,
    # save weights, every 1-epoch
    period=1)

latest = tf.train.latest_checkpoint(checkpoint_dir)

# create a model instance
model = create_model()
model.summary()



if latest: model.load_weights(latest)
loss, acc = model.evaluate(X_train, Y_train)



# training
model.fit(X_train, Y_train, epochs = 3,
          validation_data = (X_test, Y_test),
          callbacks = [cp_callback]) # save checkpoints at the end of each epoch

model.save('my_model.h5')