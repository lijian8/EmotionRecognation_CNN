#include <iostream>
#include "BPNTraining.h"
#include <cstdio>
#include "IBPNComput.h"
#include "BPNComput.h"
using namespace std;
void helpmsg();
#define TRAIN
void createfile()
{
    FILE* pfdata = fopen("data.dat", "w");
    FILE* pflabel = fopen("label.dat", "w");

    srand(time(NULL));

    for (int i = 0; i < 100; ++i)
    {
        double num1 = (double)rand() / RAND_MAX * 10000.0;
        double num2 = (double)rand() / RAND_MAX * 10000.0;
        fprintf(pfdata, "%lf %lf \n", num1, num2);
        fprintf(pflabel, "%lf \n", num1 + num2);
    }

    fclose(pfdata);
    fclose(pflabel);
}

int main(int argc, char** argv)
{
//    helpmsg();
//    createfile();
    int input_node_num = 0;
    int output_node_num = 0;
    int neuro_node_num = 0;
    double max_error_rate = 0.0;
    int data_num = 0;
    string data_file;
    string label_file;

    if (argc <= 1)
    {
        helpmsg();
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (0 == strcmp(argv[i], "-i"))
            {
                ++i;
                input_node_num = atoi(argv[i]);
            }
            else if (0 == strcmp(argv[i], "-o"))
            {
                ++i;
                output_node_num = atoi(argv[i]);
            }
            else if (0 == strcmp(argv[i], "-n"))
            {
                ++i;
                neuro_node_num = atoi(argv[i]);
            }
            else if (0 == strcmp(argv[i], "-e"))
            {
                ++i;
                max_error_rate = atof(argv[i]);
            }
            else if (0 == strcmp(argv[i], "-d"))
            {
                ++i;
                data_num = atoi(argv[i]);
            }
            else if (0 == strcmp(argv[i], "-df"))
            {
                ++i;
                data_file = string(argv[i]);
            }
            else if (0 == strcmp(argv[i], "-lf"))
            {
                ++i;
                label_file = string(argv[i]);
            }
        }
#ifdef TRAIN
        BPNTraining* train = new BPNTraining(data_num, input_node_num,
                                             neuro_node_num, output_node_num,
                                             100, 0.01, data_file, label_file);
        train->Train();
        delete train;
#else
        IBPNComput * IBPN = new BPNComput();
        int in, out;
        IBPN->Initialise(in, out);
        double * input = new double[in];
        double * output = new double[out];
        FILE* pfin, *pfout;
        pfin = fopen(argv[1], "r");
        pfout = fopen(argv[2], "w");
        for (int i = 0; i < in; ++i)
        {
            fscanf(pfin, "%lf ", input + i);
        }
        IBPN->Compute(input, output);
        for (int i = 0; i < out; ++i)
        {
            fprintf(pfout, "%lf ", output[i]);
        }
        fclose(pfin);
        fclose(pfout);

#endif
//#ifdef TRAIN
    }
//#endif
    return 0;
}

void helpmsg()
{
    cout << "\thelp message\n" << endl;
    cout << "\t-h\t---\thelp message" << endl;
    cout << "\t-i\t---\tinput node number" << endl;
    cout << "\t-o\t---\toutput node number" << endl;
    cout << "\t-n\t---\tneuro node number" << endl;
    cout << "\t-d\t---\tinput data number" << endl;
    cout << "\t-e\t---\tmax error rate" << endl;
    cout << "\t-df\t---\tdata file name" << endl;
    cout << "\t-lf\t---\tlabel file name" << endl;
    cout << "\nFor example : ./BPNTrain -i 10 -o 2 -n 20 -d 1000 -e 0.01 -df data.dat -lf label.dat\n" << endl;
}
