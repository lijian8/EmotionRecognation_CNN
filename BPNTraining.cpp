#include "BPNTraining.h"

BPNTraining::BPNTraining(int d, int i, int h, int o, int t, double r, string df, string lf) :
    DATA(d),
    IN(i),
    HIDEN(h),
    OUT(o),
    trainTimes(t),
    RATE(r),
    m_df(df),
    m_lf(lf)
{

    input = new double[IN];
    hiden = new double[HIDEN];
    ohiden = new double[HIDEN];
    output = new double[OUT];
    maxin = new double[IN];
    minin = new double[IN];
    maxout = new double[OUT];
    minout = new double[OUT];

    data = new double*[DATA];
    for (int i = 0; i < DATA; ++i)
    {
        data[i] = new double[IN];
    }
    w_input_hiden = new double*[IN];
    for (int i = 0; i < IN; ++i)
    {
        w_input_hiden[i] = new double[HIDEN];
    }
    dw_input_hiden = new double*[IN];
    for (int i = 0; i < IN; ++i)
    {
        dw_input_hiden[i] = new double[HIDEN];
    }
    w_hiden_output = new double*[HIDEN];
    for (int i = 0; i < HIDEN; ++i)
    {
        w_hiden_output[i] = new double[OUT];
    }
    dw_hiden_output = new double*[HIDEN];
    for (int i = 0; i < HIDEN; ++i)
    {
        dw_hiden_output[i] = new double[OUT];
    }
    d_output_data = new double*[DATA];
    for (int i = 0; i < DATA; ++i)
    {
        d_output_data[i] = new double[OUT];
    }

}

BPNTraining::~BPNTraining()
{
    delete[] input;
    delete[] hiden;
    delete[] ohiden;
    delete[] output;
    delete[] maxin;
    delete[] minin;
    delete[] maxout;
    delete[] minout;


    for (int i = 0; i < DATA; ++i)
    {
        delete[] data[i];
    }
    delete[] data;

    for (int i = 0; i < IN; ++i)
    {
        delete[] w_input_hiden[i];
    }
    delete[] w_input_hiden;


    for (int i = 0; i < IN; ++i)
    {
        delete[] dw_input_hiden[i];
    }
    delete[] dw_input_hiden;


    for (int i = 0; i < HIDEN; ++i)
    {
        delete[] w_hiden_output[i];
    }
    delete[] w_hiden_output;

    for (int i = 0; i < HIDEN; ++i)
    {
        delete[] dw_hiden_output[i];
    }
    delete[] dw_hiden_output;

    for (int i = 0; i < DATA; ++i)
    {
       delete[]  d_output_data[i];
    }
    delete[] d_output_data;
}

void BPNTraining::Train()
{
    if (!readdata(m_df.c_str()))
    {
        return;
    }
    if (!readlabel(m_lf.c_str()))
    {
        return;
    }
    init();
    train();
    outweights();
}


void BPNTraining::init()
{
    //standarilize
    //get max&min
    timeval times, timee;
    gettimeofday(&times, NULL);
    cout << "init....." << times.tv_sec << "::" << times.tv_usec << endl;
    //input max&min
    for (int i = 0; i < IN; ++i)
    {
        maxin[i] = data[0][i];
        minin[i] = data[0][i];
        for (int j = 0; j < DATA; ++j)
        {
//            maxin[i] = data[j][i] > maxin[i] ? data[j][i] : maxin[i];
//            minin[i] = data[j][i] < minin[i] ? data[j][i] : minin[i];
            maxin[i] = 255.0;
            minin[i] = 0.0;
        }
    }
    //output max&min
    for (int i = 0; i < OUT; ++i)
    {
        maxout[i] = d_output_data[0][i];
        minout[i] = d_output_data[0][i];
        for (int j = 0; j < DATA; ++j)
        {
            maxout[i] = d_output_data[j][i] > maxout[i] ? d_output_data[j][i] : maxout[i];
            minout[i] = d_output_data[j][i] < minout[i] ? d_output_data[j][i] : minout[i];
        }
    }

    //standarilize
    for (int i = 0; i < IN; ++i)
    {
        for (int j = 0; j < DATA; ++j)
        {
//            data[j][i] = (data[j][i] - minin[i] + 1) / (maxin[i] - minin[i] + 1);
            data[j][i] = (data[j][i] + 1) / (maxin[i] + 1);
            data[j][i] = data[j][i];
        }
    }
    for (int i = 0; i < OUT; ++i)
    {
        for (int j = 0; j < DATA; ++j)
        {
//            d_output_data[j][i] = (d_output_data[j][i] - minout[i] + 1) / (maxout[i] - minout[i] + 1);
            d_output_data[j][i] = (d_output_data[j][i] + 1) / (maxout[i] + 1);
        }
    }

    //init hiden layer
    srand(time(NULL));
    for (int i = 0; i < IN; ++i)
    {
        for (int j = 0; j < HIDEN; ++j)
        {
            w_input_hiden[i][j] = (double)rand() / RAND_MAX * RATE;
            dw_input_hiden[i][j] = 0.0;
        }
    }
    for (int i = 0; i < HIDEN; ++i)
    {
        for (int j = 0; j < OUT; ++j)
        {
            w_hiden_output[i][j] = (double)rand() / RAND_MAX * RATE;
            dw_hiden_output[i][j] = 0.0;
        }
    }
    //end
    gettimeofday(&timee, NULL);
    cout << "init end....." << timee.tv_sec << "::" << timee.tv_usec << endl;
}

void BPNTraining::train()
{
    timeval times, timee;
    gettimeofday(&times, NULL);
    cout << "train....." << times.tv_sec << "::" << times.tv_usec << endl;
    int c = 0;
    static double e = 0.0f;
    int trains = 0;
    do
    {
        c = 0;
        do
        {
            e = 0.0;
            for(int i = 0; i < DATA; ++i)
            {
                comput(i);
                for (int j = 0; j < OUT; ++j)
                {
                    e += fabs((output[j] - d_output_data[i][j]) / d_output_data[i][j]) / DATA;
                }

                backUpdate(i);
            }
            ++c;
            cout << "\ttrain times : " << c + trains * trainTimes << endl;
            cout << "\terror rate : " << e << endl;
        }while((c < trainTimes) && (e  > 0.01));
        trains++;
        cout << "train times :" << trains * trainTimes << endl;
        cout << "error : " <<  e << endl;
    }while((e / DATA > 0.01));
    gettimeofday(&timee, NULL);
    cout << "train end....." << timee.tv_sec << "::" << timee.tv_usec << endl;
    cout << "w_input_hiden > ";
    for (int i = 0; i < IN; ++i)
    {
        cout << endl;
        for (int j = 0; j < HIDEN; ++j)
        {
            cout << w_input_hiden[i][j] << " ";
        }
    }
    cout << endl;

    cout << "w_hiden_output > ";
    for (int i = 0; i < HIDEN; ++i)
    {
        cout << endl;
        for (int j = 0; j < OUT; ++j)
        {
            cout << w_hiden_output[i][j] << " ";
        }
    }
    cout << endl;
}

void BPNTraining::comput(int index)
{
    memcpy(input, data[index], sizeof(double) * IN);
    memset(hiden, 0, sizeof(double) * HIDEN);
    memset(output, 0, sizeof(double) * OUT);

    for (int i = 0; i < IN; ++i)
    {
        for (int j = 0; j < HIDEN; ++j)
        {
            hiden[j] += w_input_hiden[i][j] * input[i];
        }
    }

    for (int i = 0; i < HIDEN; ++i)
    {
        ohiden[i] = 1.0 / (1.0 + exp(-1 * hiden[i]));
        for (int j = 0; j < OUT; ++j)
        {
            output[j] += ohiden[i] * w_hiden_output[i][j];
        }
    }
}

void BPNTraining::backUpdate(int index)
{
#define A 0.2
#define B 0.4
#define a 0.2;
#define b 0.3
    double t ;
    for (int i = 0; i < HIDEN; ++i)
    {
        t = 0.0;
        for (int j = 0; j < OUT; ++j)
        {
            t += (output[j] - d_output_data[index][j]) * w_hiden_output[i][j];

            dw_hiden_output[i][j] = A * dw_hiden_output[i][j] + B * (output[j] - d_output_data[index][j]) * ohiden[i];
            w_hiden_output[i][j] -=dw_hiden_output[i][j] * RATE;
        }

        for (int j = 0; j < IN; ++j)
        {
            dw_input_hiden[j][i] = dw_input_hiden[j][i] * a + b * t * (ohiden[i]) * (1 - ohiden[i]) * (data[index][j]);
            w_input_hiden[j][i] -= dw_input_hiden[j][i] * RATE;
        }
    }
#undef A
#undef B
#undef a
#undef b
}

void BPNTraining::printresult()
{
    //    cout << "--------input------" << endl;
    //    for (int i = 0; i < IN; ++i)
    //    {
    //        cout << input[i] << endl;
    //    }
    //    memset(hiden, 0, sizeof(double) * HIDEN);
    //    memset(output, 0, sizeof(double) * OUT);

    //    cout << "standarilize.." << endl;
    //    for (int i = 0; i < IN; ++i)
    //    {
    //        input[i] = (input[i] - minin[i] + 1) / (maxin[i] - minin[i] + 1);
    //        cout << input[i] << endl;
    //    }

    //    for (int i = 0; i < IN; ++i)
    //    {
    //        for (int j = 0; j < HIDEN; ++j)
    //        {
    //            hiden[j] += w_input_hiden[i][j] * input[i];
    //        }
    //    }

    //    for (int i = 0; i < HIDEN; ++i)
    //    {
    //        ohiden[i] = 1 / (1 + exp(-1 * hiden[i]));
    //        for (int j = 0; j < OUT; ++j)
    //        {
    //            output[j] += ohiden[i] * w_hiden_output[i][j];
    //        }
    //    }
    //    cout << "--------output--------- " << endl;
    //    for (int i = 0; i < OUT; ++i)
    //    {
    //        cout << output[i] * (maxout[i] - minout[i]) +  minout[i]<< endl;
    //    }
    //    cout << "---------end------------" << endl;
}

bool BPNTraining::readdata(const char* file)
{
    FILE* pf = fopen(file, "r");
    if (NULL == pf)
    {
        cout << "Wrong data file" << endl;
        return false;
    }

    for (int i = 0; i < DATA; ++i)
    {
        for (int j = 0; j < IN; ++j)
        {
            fscanf(pf, "%lf ", &data[i][j]);
        }
        fscanf(pf, "\n");
    }
    fclose(pf);
    return true;
}

bool BPNTraining::readlabel(const char* file)
{
    FILE* pf = fopen(file, "r");
    if (NULL == pf)
    {
        cout << "Wrong label file" << endl;
        return false;
    }

    for (int i = 0; i < DATA; ++i)
    {
        for (int j = 0; j < OUT; ++j)
        {
            fscanf(pf, "%lf ", &d_output_data[i][j]);
        }
        fscanf(pf, "\n");
    }
    fclose(pf);
    return true;
}

bool BPNTraining::outweights()
{
    FILE * pf = fopen("weights", "w");
    if (NULL == pf)
    {
        cout << "write wrong" << endl;
        return false;
    }
    fprintf(pf, "%d\n", IN);
    fprintf(pf, "%d\n", HIDEN);
    fprintf(pf, "%d\n", OUT);

    fprintf(pf, "\n");
    for (int i = 0; i < IN; ++i)
    {
        fprintf(pf, "%lf ", minin[i]);
    }

    fprintf(pf, "\n");
    for (int i = 0; i < IN; ++i)
    {
        fprintf(pf, "%lf ", maxin[i]);
    }

    fprintf(pf, "\n");
    for (int i = 0; i < OUT; ++i)
    {
        fprintf(pf, "%lf ", minout[i]);
    }

    fprintf(pf, "\n");
    for (int i = 0; i < OUT; ++i)
    {
        fprintf(pf, "%lf ", maxout[i]);
    }

    fprintf(pf, "\n");
    for (int i = 0; i < IN; ++i)
    {
        for (int j = 0; j < HIDEN; ++j)
        {
            fprintf(pf, "%lf ", w_input_hiden[i][j]);
        }
        fprintf(pf, "\n");
    }

    fprintf(pf, "\n");
    for (int i = 0; i < HIDEN; ++i)
    {
        for (int j = 0; j < OUT; ++j)
        {
            fprintf(pf, "%lf ", w_hiden_output[i][j]);
        }
        fprintf(pf, "\n");
    }

    fclose(pf);
    return true;
}
