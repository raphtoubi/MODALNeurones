#ifndef MODEL_HPP
#define MODEL_HPP

#include <tensorflow/c/c_api.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class Model {
public:

    TF_Graph* graph;
    TF_Session* session;
    TF_Status* status;
    TF_Output input, target, output;
    TF_Operation *init_op, *train_op, *save_op, *restore_op;
    TF_Output checkpoint_file;

    Model(const char* graph_def_filename, const char* checkpoint_prefix);

    void Predict_for_single_input(double* batch, int batch_size);

    void Destroy();

private:
    bool Okay(TF_Status* status);

    TF_Tensor* ScalarStringTensor(const char* str, TF_Status* status);

    TF_Buffer* ReadFile(const char* filename);

};



#endif // MODEL_HPP
