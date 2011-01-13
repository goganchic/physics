#include "storagethread.h"
#include <QDebug>

StorageThread::StorageThread(int bs, QObject *parent) :
    QThread(parent), processor_state(PROCESSOR_READY), block_size(bs)
{
    this->moveToThread(this);
}

StorageThread::~StorageThread()
{
    while (blocks.size() > 0)
    {
        delete[] blocks[0];
        blocks.remove(0);
    }
}

void StorageThread::cacheRawData()
{
    qDebug() << "Allocate memory";
    int *block = new int[block_size];
    // TODO copy data
    blocks.push_back(block);

    // TODO: if processor is ready - send signal to process new data
    callProcessorIfRequired();
}

void StorageThread::callProcessorIfRequired()
{
    if (processor_state == PROCESSOR_READY && !blocks.empty())
    {
        emit storageBlockReady(blocks[0]);
    }
}

void StorageThread::notifyAboutReadyProcessor(double avg)
{
    qDebug() << "deallocate memory";
    delete[] blocks[0];
    blocks.remove(0);
    avgs.push_back(avg);
    callProcessorIfRequired();
}
