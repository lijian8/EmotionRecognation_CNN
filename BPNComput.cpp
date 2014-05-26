#include "BPNComput.h"
#include <memory.h>
#include <cmath>
#include <cstdio>

BPNComput::BPNComput()
{
}

BPNComput::~BPNComput()
{
    delete[] minin;
    delete[] maxin;
    delete[] minout;
    delete[] maxout;
    delete[] hiden;

    for (int i = 0; i < IN; ++i)
    {
        delete[] w_input_hiden[i];
    }
    delete[] w_input_hiden;

    for (int i = 0; i < HIDEN; ++i)
    {
        delete[] w_hiden_output[i];
    }
    delete[] w_hiden_output;
}

void BPNComput::Initialise(int &in_node, int &out_node)
{
    FILE* pf = fopen("weights", "r");
    if (NULL == pf)
    {
        cout << "wrong weights" << endl;
    }

    fscanf(pf, "%d\n", &IN);
    fscanf(pf, "%d\n", &HIDEN);
    fscanf(pf, "%d\n", &OUT);
    minin = new double[IN];
    maxin = new double[IN];
    minout = new double[OUT];
    maxout = new double[OUT];
    hiden = new double[HIDEN];

    w_input_hiden = new double*[IN];
    for (int i = 0; i < IN; ++i)
    {
        w_input_hiden[i] = new double[HIDEN];
    }

    w_hiden_output = new double*[HIDEN];
    for (int i = 0; i < HIDEN; ++i)
    {
        w_hiden_output[i] = new double[OUT];
    }


    in_node = IN;
    out_node = OUT;
    fscanf(pf, "\n");
    for (int i = 0; i < IN; ++i)
    {
        fscanf(pf, "%lf ", &minin[i]);
    }

    fscanf(pf, "\n");
    for (int i = 0; i < IN; ++i)
    {
        fscanf(pf, "%lf ", &maxin[i]);
    }

    fscanf(pf, "\n");
    for (int i = 0; i < OUT; ++i)
    {
        fscanf(pf, "%lf ", &minout[i]);
    }

    fscanf(pf, "\n");
    for (int i = 0; i < OUT; ++i)
    {
        fscanf(pf, "%lf ", &maxout[i]);
    }

    fscanf(pf, "\n");
    for (int i = 0; i < IN; ++i)
    {
        for (int j = 0; j < HIDEN; ++j)
        {
            fscanf(pf, "%lf ", &w_input_hiden[i][j]);
        }
        fscanf(pf, "\n");
    }

    fscanf(pf, "\n");
    for (int i = 0; i < HIDEN; ++i)
    {
        for (int j = 0; j < OUT; ++j)
        {
            fscanf(pf, "%lf ", &w_hiden_output[i][j]);
        }
        fscanf(pf, "\n");
    }

    fclose(pf);
}

void BPNComput::Compute(double *input, double *output)
{
    cout << "--------input------" << endl;
    for (int i = 0; i < IN; ++i)
    {
        cout << input[i] << endl;
    }
    memset(hiden, 0, sizeof(double) * HIDEN);
    memset(output, 0, sizeof(double) * OUT);

    cout << "standarilize.." << endl;
    for (int i = 0; i < IN; ++i)
    {
//        input[i] = (input[i] - minin[i] + 1) / (maxin[i] - minin[i] + 1);
        input[i] = (input[i] + 1) / (maxin[i] + 1);
        cout << input[i] << endl;
    }

    for (int i = 0; i < IN; ++i)
    {
        for (int j = 0; j < HIDEN; ++j)
        {
            hiden[j] += w_input_hiden[i][j] * input[i];
        }
    }

    for (int i = 0; i < HIDEN; ++i)
    {
        double dneuro = 1 / (1 + exp(-1 * hiden[i]));
        for (int j = 0; j < OUT; ++j)
        {
            output[j] += dneuro * w_hiden_output[i][j];
        }
    }
    cout << "--------output--------- " << endl;
    for (int i = 0; i < OUT; ++i)
    {
//        output[i] = output[i] * (maxout[i] - minout[i] + 1) +  minout[i] - 1;
        output[i] = output[i] * (maxout[i] + 1) - 1;
        cout << output[i]<< endl;
    }
    cout << "---------end------------" << endl;
}
