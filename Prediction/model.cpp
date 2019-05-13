#include "model.hpp"

using namespace std;

Model::Model(const char* graph_def_filename, const char* checkpoint_prefix){
    this->status = TF_NewStatus();
    this->graph = TF_NewGraph();

    {
        // Create the session.
        TF_SessionOptions* opts = TF_NewSessionOptions();
        this->session = TF_NewSession(this->graph, opts, this->status);
        TF_DeleteSessionOptions(opts);
        assert(Okay(this->status));
    }
    TF_Graph* g = this->graph;
    {
        // Import the graph.
        TF_Buffer* graph_def = ReadFile(graph_def_filename);
        if (graph_def == nullptr) exit(EXIT_FAILURE);
        TF_ImportGraphDefOptions* opts = TF_NewImportGraphDefOptions();
        TF_GraphImportGraphDef(g, graph_def, opts, this->status);
        TF_DeleteImportGraphDefOptions(opts);
        TF_DeleteBuffer(graph_def);
        assert(Okay(this->status));
    }

    //Restore weights from checkpoint
    {
        //Verify the existence of checkpoints directory
        struct stat buf;
       assert(stat(checkpoint_prefix, &buf) == 0);

       TF_Tensor* t = ScalarStringTensor(checkpoint_prefix, this->status);
       if (!Okay(this->status)) {
           TF_DeleteTensor(t);
           exit(EXIT_FAILURE);
       }
       TF_Output inputs[1] = {this->checkpoint_file};
       TF_Tensor* input_values[1] = {t};
       const TF_Operation* op[1] ={this->restore_op};
       TF_SessionRun(this->session, nullptr, inputs, input_values, 1,
       /* No outputs */
       nullptr, nullptr, 0,
       /* The operation */
                   op, 1, nullptr, this->status);
       TF_DeleteTensor(t);
       assert(Okay(this->status));
    }
}

void Model::Predict_for_single_input(double* batch, int batch_size = 1) {
    // batch consists of 105x1 matrices.
    const int64_t dims[3] = {batch_size, 105, 1};
    const size_t nbytes = static_cast<unsigned long>(batch_size) * sizeof(double);
    TF_Tensor* t = TF_AllocateTensor(TF_DOUBLE, dims, 3, nbytes);
    memcpy(TF_TensorData(t), batch, nbytes);
    TF_Output inputs[1] = {this->input};
    TF_Tensor* input_values[1] = {t};
    TF_Output outputs[1] = {this->output};
    TF_Tensor* output_values[1] = {nullptr};
    TF_SessionRun(this->session, nullptr, inputs, input_values, 1, outputs,
                  output_values, 1,
    /* No target operations to run */
                    nullptr, 0, nullptr, this->status);
    TF_DeleteTensor(t);
    assert(Okay(this->status));
    if (TF_TensorByteSize(output_values[0]) != nbytes) {
        cerr << "ERROR: Expected predictions tensor to have" << nbytes << "bytes, has" << TF_TensorByteSize(output_values[0]) << endl;
        TF_DeleteTensor(output_values[0]);
        exit(EXIT_FAILURE);
     }
    float* predictions = new float[nbytes];
    memcpy(predictions, TF_TensorData(output_values[0]), nbytes);
    TF_DeleteTensor(output_values[0]);
    printf("Predictions:\n");
    for (int i = 0; i < batch_size; ++i) {
        cout << "x =" << batch[i] << "predicted y =" << predictions[i] << endl;
    }
    free(predictions);
}

TF_Buffer* Model::ReadFile(const char* filename) {
    int fd = open(filename, 0);
    if (fd < 0) {
        perror("failed to open file: ");
        return nullptr;
    }

    struct stat stat;
    if (fstat(fd, &stat) != 0) {
        perror("failed to read file: ");
        return nullptr;
    }
    char* data =  new char[stat.st_size];
    ssize_t nread = read(fd, data, static_cast<size_t>(stat.st_size));
    if (nread < 0) {
        perror("failed to read file: ");
        free(data);
        return nullptr;
    }
    if (nread != stat.st_size) {
        cerr << "read " << nread << "bytes, expected to read " <<  stat.st_size << endl;
        free(data);
        return nullptr;
    }
    TF_Buffer* ret = TF_NewBufferFromString(data, static_cast<size_t>(stat.st_size));
    free(data);
    return ret;
}

TF_Tensor* Model::ScalarStringTensor(const char* str, TF_Status* status) {
    size_t nbytes = 8 + TF_StringEncodedSize(strlen(str));
    TF_Tensor* t = TF_AllocateTensor(TF_STRING, nullptr, 0, nbytes);
    char* data = static_cast<char*>(TF_TensorData(t));
    memset(data, 0, 8);  // 8-byte offset of first string.
    TF_StringEncode(str, strlen(str), data + 8, nbytes - 8, status);
    return t;
}

bool Model::Okay(TF_Status* status) {
    if (TF_GetCode(status) != TF_OK) {
        fprintf(stderr, "ERROR: %s\n", TF_Message(status));
        return false;
    }
    return true;
}

void Model::Destroy() {
    TF_DeleteSession(this->session, this->status);
    Okay(this->status);
    TF_DeleteGraph(this->graph);
    TF_DeleteStatus(this->status);
}
