#include "storagethread.h"

StorageThread::StorageThread(int bs, int dbs, int ti, QObject *parent) :
    QThread(parent), processor_state(PROCESSOR_READY), block_size(bs),
    device_buffer_size(dbs), temp_buffer_position(0), timer_interval(ti)
{
    this->moveToThread(this);
    temp_buffer = new short[block_size * 2];
    connect(&timer, SIGNAL(timeout()), this, SLOT(cacheRawData()));
}

StorageThread::~StorageThread()
{
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(cacheRawData()));
    while (blocks.size() > 0)
    {
        delete[] blocks[0];
        blocks.remove(0);
    }

    delete[] temp_buffer;
}

void StorageThread::run()
{
    timer.start(timer_interval);
    exec();
}

void StorageThread::cacheRawData()
{
    // block_size * 2 = size for t1 + size for t2
    short *device_buffer = new short[device_buffer_size];
    int coping_size = 0;

    if (temp_buffer_position + device_buffer_size < block_size * 2)
    {
        copyDataToTempBuffer(device_buffer, device_buffer_size);
    }
    else
    {
        coping_size = block_size * 2 - (temp_buffer_position + device_buffer_size);
        copyDataToTempBuffer(device_buffer, coping_size);
        temp_buffer_position = 0;
        blocks.push_back(temp_buffer);
        callProcessorIfRequired();
        temp_buffer = new short[block_size * 2];
        copyDataToTempBuffer(device_buffer + coping_size, device_buffer_size - coping_size);
    }

    delete[] device_buffer;

    emitUsedMemoryChanged();
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
    // blocks count + temp buffer
    emit changeUsedMemory(blocks.size() + 1, (blocks.size() + 1) * block_size * 2 * sizeof(short), temp_buffer_position);
}

void StorageThread::copyDataToTempBuffer(short *data, int size)
{
    memcpy(temp_buffer + temp_buffer_position, data, size * sizeof(short));
    temp_buffer_position += size;
}
