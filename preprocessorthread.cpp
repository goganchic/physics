#include "preprocessorthread.h"

PreprocessorThread::PreprocessorThread(int bs, QObject *parent) :
    QThread(parent), block_size(bs)
{
    this->moveToThread(this);
}

void PreprocessorThread::run()
{
    exec();
}


void PreprocessorThread::processData(short *ptr)
{
    double res = 0;

    for (int i = 0; i < block_size; i++)
    {
        res += ptr[i];
    }

    emit dataProcessed(res / block_size);
}
