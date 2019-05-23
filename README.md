# MODALNeurones

Our model permits to find the primary structure of protein from a 3D structure drawn on Samson.
We consider the distances between th 14 amino-acids that surround the one considered to to predict a probability of which amino-acid it can be.

The folder Training contains :

    - the Jupiter Notebook that creates the Machine Learning model
    
    - the data with which it was trained
    
    - the checkpoints that contains the coefficients of the neural network
    
    
There are two kind of models : 

    - Single input model : for the 7 first amino acid of the protein
    
    - Double input model : for the rest of the amino acid so that it considers the 7 former amino acid to find the one considered
    

The folder Prediction containes : 

    - prediction.py which permits to analyse a matrix of distances obtained from the drawing of a protein on Samson. This file recreates the neural network architecture and stores the weights of each neuron in it to predict the primary structure of the protein
    
    - main.cpp which is a C++ files that calls a python script which encapsulates the neural network
      You need to have TensorFlow on your computer to make it work (https://www.tensorflow.org/install/pip)
      
    - the folder Models that contains the weights of each neuron
    
