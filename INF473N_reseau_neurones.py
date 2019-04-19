import tensorflow as tf
import tensorflow.keras as keras
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split, cross_val_score

#importation de la data
data = pd.read_csv('')

#preprocessing --> eventuellement reshape les donnees
X = 
Y = 

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size = 0.2, random_state = 2)


#creation du reseau de neurones
model = tf.keras.models.Sequential() #Sequential pour commencer?

#couche d'input --> meme dimension que l'input?

model.add(tf.keras.layers.Dense(units = 105, activation = tf.nn.relu))
model.add(tf.keras.layers.Dense(units = 53, activation = tf.nn.relu))#diviser par 2?

#couche finale
model.add(tf.keras.layers.Dense(units = 20, activation = tf.nn.softmax))

#entrainement du modele
model.fit(x_train, y_train, epochs = 3)

#evaluation
val_loss, val_acc = model.evaluate(x_test, y_test)

#prediction
model.predict()