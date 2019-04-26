print("Importing libraries...")

import os as os

import tensorflow as tf
from tensorflow import keras
from sklearn.model_selection import train_test_split

import INF473N_data_extraction as data

print("Libraries imported.")

#data iprocessing
print("Data processing...")

input_shape, output_shape, distances, labels = data.load_data()
print("Dataset loaded")

x_train, x_test, y_train, y_test = train_test_split(
    distances, labels, test_size=0.2, random_state=42)
print("Data sliced between train and test datasets")
print("Data processed.")


# returns a short sequential model
def create_model():
    # activation function: leaky ReLU
    leakyrelu = lambda x: tf.keras.activations.relu(x, alpha=0.01, max_value=None, threshold=0)

    # creation du reseau de neurones
    model = tf.keras.models.Sequential([

        # hidden layer
        keras.layers.Dense(units = 512, activation = leakyrelu, input_shape=(input_shape,)),
        keras.layers.Dropout(rate=0.2),
        keras.layers.Dense(units = 105, activation = leakyrelu),
        keras.layers.Dropout(rate=0.2),

        # final layer
        keras.layers.Dense(units = output_shape, activation = tf.nn.softmax),
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
print("Creating the model...")
model = create_model()
model.summary()


if latest: model.load_weights(latest)
print("Previous state imported.")
loss, acc = model.evaluate(X_train, Y_train)



# training
print("Training...")
model.fit(X_train, Y_train, epochs = 50,
          validation_data = (X_test, Y_test),
          callbacks = [cp_callback]) # save checkpoints at the end of each epoch

model.save('my_model.h5')