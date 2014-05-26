#ifndef BPNCOMPUT_H
#define BPNCOMPUT_H

#include "IBPNComput.h"

class BPNComput : public IBPNComput
{
public:
    BPNComput();
    ~BPNComput();
    void Initialise(int &in_node, int &out_node);
    void Compute(double *in, double* out);
private:
    int IN;
    int HIDEN;
    int OUT;

    double *minin;
    double *maxin;
    double *minout;
    double *maxout;
    double *hiden;
    double **w_input_hiden;
    double **w_hiden_output;
};

#endif // BPNCOMPUT_H
