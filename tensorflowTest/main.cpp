#include <iostream>
#include <fstream>

#include <tensorflow/c/c_api.h>

using namespace std;

String convert_label(int number){
    String* aminoacidList = ["ALA", "ARG", "ASN", "ASP", "CYS", "GLU", "GLN", "GLY", "HIS", "ILE", "LEU", "LYS", "MET", "PHE", "PRO", "PYL", "SEL", "SER", "THR", "TRP", "TYR", "VAL"];
    return aminoacidList[number];
}

double** distances(){

    ifstream file(distances_file.csv); // opening the file in reading

    if(!file.fail()){

        int protein_size;
        file >> protein_size;
        file.ignore();

        int input_shape = 105; // a refaire plus proprememt si possible (en utilisant les donnees du fichier)
        file.ignore();
        file.ignore();

         double** distances_list = new double[protein_size][input_shape];
         int j = 0;

        while(file.peek() != EOF){ // not at the end of the file

            double* distances = new double[input_shape];
            int i = 0;

            while (file.peek() != '\n'){ // while we are not at the end of the line

                double d;
                if(file.peek() == ','){
                    fichier.ignore(); // go to the next character
                }

                else {
                    file >> d;
                    distances[i] = d;
                    i++;
                }
            }
            distances_list[j] = distances;
            file.ignore(); // go to the next line
            file.ignore();
        }

        file.close();

        return distances_list;


    }
    else {
        std::cerr << "File does not exist!" << std::endl;
    }

}

int argmax(int* tab){
    int max = 0;
    int argmax = -1;

    for(int i=0; i<tab.size(); i++){
        if(tab[i]>max){
            max = tab[i];
            argmax = i;
        }
    }
    return argmax;
}

int main() {
  int protein_size = 15;
  double** residues_probabilities = new double[protein_size][22];
  double** distances = new double[protein_size][105];

  // PREDICTION

  // predict the 7 first residues using the single inputs model
    for(int i=0; i<7; i++){
        double* input = distances[i];

        // prediction
        double* residue_prob = predict_single_input(input);
        residues_probabilities[i] = residue_prob;

    }

    // predict the following residues using the double inputs model
    for(int i=7; i<protein_size; i++){

        //building the input
        double* distances_input = distances[i];

        double** residues_input = new int[7][22];
        for(int res = 1; res <= 7; res++){ residues_input[res] = residues_probabilities[i - res];}

        // prediction
        double* residue_prob = predict_double_input(distances_input, residues_input);

        residues_probabilities[i] = residue_prob;
    }

    // OUTPUT
    String* output = String[protein_size];

    for(int i=0; i< protein_size; i++) {
        int label = argmax(residues_probabilities[i]);
        output[i] = convert_label(label);
    }

}
