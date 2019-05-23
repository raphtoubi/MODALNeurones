#include "sequence.hpp"

using namespace std;

Sequence::Sequence(double** probabilities, int protein_size) {

    //building the output
    string** sequence = new string*[protein_size];
    for(int i=0; i<22; i++){sequence[i] = new string[22];}

    // converting to strings
    for(int i=0; i<protein_size; i++){ //iterating over the sequence
        for(int j=0; j<22; j++){ // iterating over aminoacids probabilities
            sequence[i][j] = writeAAProb(probabilities[i][j], j);
        }
    }

    // sorting by decreasing probability
    for(int i=0; i<protein_size; i++){ // iterating over the sequence
        sequence[i] = sortAA(sequence[i], probabilities[i]); // sorting the texts depending on probabilities
    }

    // assigning to this->sequence
    this->sequence = sequence;

}

string** Sequence::getSequence(){
    return sequence;
}

string Sequence::writeAAProb(double prob, int aaNumber){

    double percentage = round(prob*10000) / 10000;

    string probString = aminoacid_list[aaNumber];
    probString.append(" ");
    probString.append(to_string(percentage));

    return probString;
}

string* Sequence::sortAA(string* arrayToSort, double* weights){

    for (int i = 1; i <22; i++) {

        double weightsElem = weights[i];
        string arrayToSortElem = arrayToSort[i];

        int j=i;
        while(j>0 && weights[j-1] < weightsElem){
            weights[j] = weights[j-1];
            arrayToSort[j] = arrayToSort[j-1];

            weights[j] = weightsElem;
            arrayToSort[j] = arrayToSortElem;

            j--;
        }
    }

    return arrayToSort;
}
