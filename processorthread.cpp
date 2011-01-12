#include "processorthread.h"

ProcessorThread::ProcessorThread(int bs, QObject *parent) :
    QThread(parent), block_size(bs)
{
    this->moveToThread(this);
}

void ProcessorThread::run()
{
    exec();
}

void ProcessorThread::processData(int *ptr)
{
    double result = 0;
    // Process data
    for (int i = 0; i < block_size; i++)
    {
        result += ptr[i];
    }
    result = result / block_size;

    emit dataProcessed(result);
    emit processorReady();
}
