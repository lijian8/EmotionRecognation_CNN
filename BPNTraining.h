#ifndef BPNTRAINING_H
#define BPNTRAINING_H

#include <iostream>
#include <memory.h>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include <cstring>
#include <cstdio>

using namespace std;

class BPNTraining
{
public:
    BPNTraining(int d, int i, int h, int o, int t, double r, string df, string lf);
    virtual ~BPNTraining();
    void Train();
private://function
    void init();
    void train();
    void comput(int index);
    void backUpdate(int index);
    void printresult();
    bool readdata(const char* file);
    bool readlabel(const char* file);
    bool outweights();
private://data
    const int DATA;
    const int IN;
    const int HIDEN;
    const int OUT;
    const int trainTimes;
    const double RATE;
    string m_df;
    string m_lf;
    double **data;
    double *input;
    double *hiden;
    double *ohiden;
    double *output;
    double *maxin;
    double *minin;
    double *maxout;
    double *minout;
    double **w_input_hiden;
    double **dw_input_hiden;
    double **w_hiden_output;
    double **dw_hiden_output;
    double **d_output_data;
};

#endif // BPNTRAINING_H
