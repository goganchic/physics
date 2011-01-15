#include "storagethread.h"

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
    // block_size * 2 = size for t1 + size for t2
    int *block = new int[block_size * 2];

    // TODO copy data
    blocks.push_back(block);
    emitUsedMemoryChanged();

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
    delete[] blocks[0];
    blocks.remove(0);
    emitUsedMemoryChanged();
    callProcessorIfRequired();
}

void StorageThread::emitUsedMemoryChanged()
{
    emit changeUsedMemory(blocks.size(), blocks.size() * block_size * 2);
}
