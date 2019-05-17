import tensorflow as tf
import numpy as np
import os as os


def convert_label(number):
    aminoacid_list = ["ALA", "ARG", "ASN", "ASP", "CYS", "GLU", "GLN", "GLY", "HIS", "ILE", "LEU",
                             "LYS", "MET", "PHE", "PRO", "PYL", "SEL", "SER", "THR", "TRP", "TYR", "VAL"]
    return aminoacid_list[number];


def load_data(filename):

    with open(filename) as f:
        protein_size = int(f.readline())

        shapes = f.readline()
        input_shape = int(shapes.split(",")[1])

        distances_list = np.zeros((protein_size, input_shape))

        for i in range(0, protein_size):
            distances = f.readline()
            distances_list[i, :] = np.array([float(s) for s in distances.split(",")])

        return protein_size, distances_list


def write_data(outputs, protein_size):

    with open("predicted_protein.csv", 'w') as f:

        f.write(str(protein_size))
        f.write("\n")

        for i in range(0, protein_size):
            for j in range(0, 22):
                f.write(str(outputs[i, j]))
                f.write(",")
            f.write("\n")


def single_input_model():
    # activation function: leaky ReLU
    leakyrelu = lambda x: tf.keras.activations.relu(x, alpha=0.01, max_value=None, threshold=0)

    # creation du reseau de neurones
    model = tf.keras.models.Sequential([

        # hidden layer
        tf.keras.layers.Dense(units = 105, activation = leakyrelu, input_shape = (105,)),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units = 210, activation = leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units = 210, activation = leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units = 210, activation = leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units = 100, activation = leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units = 75, activation = leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units = 53, activation = leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),

        # final layer
        tf.keras.layers.Dense(units = 22, activation = tf.nn.softmax),
    ])

    model.compile(optimizer=tf.keras.optimizers.Adam(),
                  loss=tf.keras.losses.sparse_categorical_crossentropy,
                  metrics=['accuracy'])

    # Restoring
    single_input_checkpoint_path = "models/Single input model/checkpoints/checkpoint.ckpt"
    single_input_checkpoint_dir = os.path.dirname(single_input_checkpoint_path)
    single_input_latest = tf.train.latest_checkpoint(single_input_checkpoint_dir)
    model.load_weights(single_input_latest)

    return model


def double_input_model():
    # activation function: leaky ReLU
    leakyrelu = lambda x: tf.keras.activations.relu(x, alpha=0.01, max_value=None, threshold=0)

    # distances branch
    distancesBranch = tf.keras.models.Sequential([
        tf.keras.layers.Dense(units=105, activation=leakyrelu, input_shape=(105,)),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units=210, activation=leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units=210, activation=leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units=100, activation=leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units=75, activation=leakyrelu),
        tf.keras.layers.Dropout(rate=0.2),
        tf.keras.layers.Dense(units=50, activation=leakyrelu),
        tf.keras.layers.Dropout(rate=0.2)
    ])

    # residues branch
    residuesBranch = tf.keras.models.Sequential([
        tf.keras.layers.Dense(units=7, activation=leakyrelu, input_shape=(7, 22)),
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(units=50, activation=leakyrelu)

    ])

    # concatenation
    combinedInput = tf.keras.layers.concatenate([distancesBranch.output, residuesBranch.output])

    x = tf.keras.layers.Dense(units=30, activation=leakyrelu, input_shape=(100,))(combinedInput)
    x = tf.keras.layers.Dense(units=22, activation=tf.nn.softmax)(x)

    model = tf.keras.models.Model(inputs=[distancesBranch.input, residuesBranch.input], outputs=x)

    model.compile(optimizer=tf.keras.optimizers.Adam(),
                  loss=tf.keras.losses.sparse_categorical_crossentropy,
                  metrics=['accuracy'])

    # Restoring
    double_input_checkpoint_path = "models/Double input model/checkpoints/checkpoint.ckpt"
    double_input_checkpoint_dir = os.path.dirname(double_input_checkpoint_path)
    double_input_latest = tf.train.latest_checkpoint(double_input_checkpoint_dir)
    model.load_weights(double_input_latest)

    return model


input_data = load_data("distances_file.csv")
protein_size = int(input_data[0])
inputs = input_data[1]

# outputs : matrix of residues probabilities
outputs = np.zeros((protein_size, 22))

single_input_model = single_input_model()
double_input_model = double_input_model()

# PREDICTION

# predict the 7 first residues using the single input model
for i in range(0, 7):
    input = np.reshape(inputs[i, :], (105, 1)).T
    outputs[i, :] = single_input_model.predict(input)

# predict the following residues using the double input model
for i in range(7, protein_size):
    input = np.reshape(inputs[i, :], (105, 1)).T
    residues_input = np.reshape(outputs[i-7:i, :], (1, 7, 22))

    outputs[i, :] = double_input_model.predict([input,  residues_input])

## OUTPUT
write_data(outputs, protein_size)

