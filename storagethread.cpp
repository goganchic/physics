#include "storagethread.h"

StorageThread::StorageThread(int preprocessor_bs, int processor_bs, int device_bs, int ti, int t1, int t2, QObject *parent) :
    QThread(parent), preprocessor_state(PROCESSOR_READY), processor_state(PROCESSOR_READY),
    preprocessor_block_size(preprocessor_bs), processor_block_size(processor_bs),
    device_buffer_size(device_bs), temp_preprocessor_buffer_position(0),
    temp_processor_buffer_position(0), timer_interval(ti)
{
    preprocessor = new PreprocessorThread(preprocessor_block_size);
    processor = new ProcessorThread(processor_block_size, t1, t2);
    this->moveToThread(this);
    this->timer.moveToThread(this);
    temp_preprocessor_buffer = new short[preprocessor_block_size];
    temp_processor_buffer = new double[processor_block_size * 2];
    connect(&timer, SIGNAL(timeout()), this, SLOT(cacheRawData()));
    connect(this, SIGNAL(preprocessorBlockReady(short*)), preprocessor, SLOT(processData(short*)));
    connect(preprocessor, SIGNAL(dataProcessed(double)), this, SLOT(cachePreprocessedData(double)));
    connect(processor, SIGNAL(processorReady()), this, SLOT(notifyAboutReadyProcessor()));
    connect(this, SIGNAL(processorBlockReady(double*)), processor, SLOT(processData(double*)));
}

StorageThread::~StorageThread()
{
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(cacheRawData()));

    processor->quit();
    processor->wait();

    disconnect(processor, SIGNAL(processorReady()), this, SLOT(notifyAboutReadyProcessor()));
    disconnect(this, SIGNAL(processorBlockReady(double*)), processor, SLOT(processData(double*)));

    preprocessor->quit();
    preprocessor->wait();
    disconnect(this, SIGNAL(preprocessorBlockReady(short*)), preprocessor, SLOT(processData(short*)));
    disconnect(preprocessor, SIGNAL(dataProcessed(double)), this, SLOT(cachePreprocessedData(double)));

    while (preprocessor_blocks.size() > 0)
    {
        delete[] preprocessor_blocks[0];
        preprocessor_blocks.remove(0);
    }

    while (processor_blocks.size() > 0)
    {
        delete[] processor_blocks[0];
        processor_blocks.remove(0);
    }

    delete[] temp_preprocessor_buffer;
    delete[] temp_processor_buffer;

    delete processor;
    delete preprocessor;
}

void StorageThread::run()
{
    preprocessor->start();
    processor->start();
    timer.start(timer_interval);
    exec();
}

void StorageThread::cacheRawData()
{
    short *device_buffer = new short[device_buffer_size];
    int coping_size = 0;

    if (temp_preprocessor_buffer_position + device_buffer_size < preprocessor_block_size)
    {
        copyDataToTempBuffer(device_buffer, device_buffer_size);
    }
    else
    {
        coping_size = preprocessor_block_size - temp_preprocessor_buffer_position;
        copyDataToTempBuffer(device_buffer, coping_size);
        temp_preprocessor_buffer_position = 0;
        preprocessor_blocks.push_back(temp_preprocessor_buffer);
        callPreprocessorIfRequired();
        temp_preprocessor_buffer = new short[preprocessor_block_size];
        copyDataToTempBuffer(device_buffer + coping_size, device_buffer_size - coping_size);
    }

    delete[] device_buffer;

    emitUsedMemoryChanged();
}

void StorageThread::cachePreprocessedData(double avg)
{
    temp_processor_buffer[temp_processor_buffer_position] = avg;
    temp_processor_buffer_position++;

    if (temp_processor_buffer_position >= processor_block_size * 2)
    {
        temp_processor_buffer_position = 0;
        processor_blocks.push_back(temp_processor_buffer);
        callProcessorIfRequired();
        temp_processor_buffer = new double[processor_block_size * 2];
    }

    delete[] preprocessor_blocks[0];
    preprocessor_blocks.remove(0);
    emitUsedMemoryChanged();

    preprocessor_state = PROCESSOR_READY;
    callPreprocessorIfRequired();
}

void StorageThread::callProcessorIfRequired()
{
    if (processor_state == PROCESSOR_READY && !processor_blocks.empty())
    {
        processor_state = PROCESSOR_BUSY;
        emit processorBlockReady(processor_blocks[0]);
    }
}

void StorageThread::callPreprocessorIfRequired()
{
    if (preprocessor_state == PROCESSOR_READY && !preprocessor_blocks.empty())
    {
        preprocessor_state = PROCESSOR_BUSY;
        emit preprocessorBlockReady(preprocessor_blocks[0]);
    }
}

void StorageThread::notifyAboutReadyProcessor()
{
    delete[] processor_blocks[0];
    processor_blocks.remove(0);
    emitUsedMemoryChanged();
    processor_state = PROCESSOR_READY;
    callProcessorIfRequired();
}

void StorageThread::emitUsedMemoryChanged()
{
    // TODO: Fix this
    // blocks count + temp buffer
    emit changeUsedMemory(preprocessor_blocks.size() + 1, processor_blocks.size() + 1, temp_processor_buffer_position);
}

void StorageThread::copyDataToTempBuffer(short *data, int size)
{
    memcpy(temp_preprocessor_buffer + temp_preprocessor_buffer_position, data, size * sizeof(short));
    temp_preprocessor_buffer_position += size;
}
