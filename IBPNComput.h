#ifndef IBPNCOMPUT_H
#define IBPNCOMPUT_H

#include <iostream>
using namespace std;

class IBPNComput
{
public:
    IBPNComput(){}
    virtual ~IBPNComput(){}
    virtual void Initialise(int &in_node, int &out_node) = 0;
    virtual void Compute(double *in, double* out) = 0;
};

#endif // IBPNCOMPUT_H
