#include "processorthread.h"
#include <math.h>

ProcessorThread::ProcessorThread(int bs, int t_1, int t_2, QObject *parent) :
    QThread(parent), block_size(bs), t1(t_1), t2(t_2), mt(0), mt2(0), number(0)
{
    this->moveToThread(this);
}

void ProcessorThread::run()
{
    exec();
}

void ProcessorThread::processData(short *ptr)
{
    double x1 = 0;
    double x2 = 0;
    double x3 = 0;
    double k0 = 0;
    double k1 = 0;
    double t = 0;

    // Process data
    for (int i = 0; i < block_size; i++)
    {
        x1 += ptr[i];
    }
    x1 = x1 / block_size;

    for (int i = 0; i < block_size; i++)
    {
        x2 += ptr[i + block_size];
    }
    x2 = x2 / block_size;

    x3 = x1 + x2 / 2;

    // Temp stuff
    double diff = x2 - x1;
    if (fabs(diff) < 0.01)
    {
        diff = 1;
    }

    k0 = (t1 * x2 - t2 * x1) / diff;
    k1 = (t2 - t1) / diff;
    t = k0 + k1 * x3;

    number++;
    mt = mt * (number - 1) / number + t / number;
    mt2 = mt2 * (number - 1) / number + t * t / number;
    dt = mt2 - mt * mt;


    emit dataProcessed(x1, x2, x3, mt, dt);
    emit processorReady();
}
