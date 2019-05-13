#include <iostream>
#include <fstream>
#include <string>

#include "model.hpp"


using namespace std;

string convert_label(const int& number){
    const string aminoacidList[22] = {"ALA", "ARG", "ASN", "ASP", "CYS", "GLU", "GLN", "GLY", "HIS", "ILE", "LEU",
                             "LYS", "MET", "PHE", "PRO", "PYL", "SEL", "SER", "THR", "TRP", "TYR", "VAL"};
    return aminoacidList[number];
}

double** get_distances(){ // works fine

    ifstream file("distances_file.csv"); // opening the file in reading

    if(file.fail()){
        std::cerr << "File does not exist!" << std::endl;
        return nullptr;
    }

    else {

        int protein_size;
        file >> protein_size;
        cout << protein_size << endl;
        file.ignore();
        file.ignore();
        file.ignore();
        file.ignore();

        int input_shape;
        file >> input_shape;
        cout << input_shape << endl;
        file.ignore();
        file.ignore(); // jump one line

         double** distances_list = new double*[protein_size];
         for(int i=0; i<protein_size; i++) {distances_list[i] = new double[input_shape];}



        for(int i=0; i< protein_size; i++){
            assert(file.peek() != EOF);// not at the end of the file

            for(int j=0; j< input_shape; j++){

                file >> distances_list[i][j];
                file.ignore(); // comma or end of line
            }

            file.ignore();// jump one line
        }
        file.close();
        return distances_list;
    }
}

int argmax(const double* tab){ // only works for size 22 array !!
    double max = 0;
    int argmax = -1;

    for(int i=0; i< 22; i++){
        if(tab[i]>max){
            max = tab[i];
            argmax = i;
        }
    }
    return argmax;
}

int main() {

    int protein_size = 15;

    // initializing the matrix of residues probabilities
    double** residues_probabilities = new double*[protein_size];
    for (int i=0;i<protein_size;i++) {
        residues_probabilities[i] = new double[22];
    for (int j=0;j<22;j++) {residues_probabilities[i][j] = 0;}
  }

  double** distances = get_distances();

  cout << distances[4][6] << endl;
  Model model("single_input_model.pb", "checkpoints");




  /*

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

        double** residues_input = new double*[7];
        for(int res = 1; res <= 7; res++){ residues_input[res] = residues_probabilities[i - res];}

        // prediction
        double* residue_prob = predict_double_input(distances_input, residues_input);

        residues_probabilities[i] = residue_prob;
    }

    // OUTPUT
    string* output = new string[protein_size];

    for(int i=0; i< protein_size; i++) {
        int label = argmax(residues_probabilities[i]);
        output[i] = convert_label(label);
    }
    */

    return 0;

}

