#include "storagethread.h"
#include <QDebug>

StorageThread::StorageThread(int bs, QObject *parent) :
    QThread(parent), processor_state(PROCESSOR_READY), block_size(bs)
{
    this->moveToThread(this);
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

void StorageThread::notifyAboutReadyProcessor()
{
    qDebug() << "deallocate memory";
    delete[] blocks[0];
    blocks.remove(0);
    callProcessorIfRequired();
}
