#ifndef STORAGETHREAD_H
#define STORAGETHREAD_H

#include <QThread>
#include <QVector>
#include <QTimer>
#include "preprocessorthread.h"
#include "processorthread.h"

#define BLOCK_SIZE 512
#define PROCESSOR_READY 0
#define PROCESSOR_BUSY 1

class StorageThread : public QThread
{
    Q_OBJECT
public:
    explicit StorageThread(int preprocessor_bs, int processor_bs, int device_bs, int ti, int t1, int t2, QObject *parent = 0);
    ~StorageThread();
    const ProcessorThread* getProcessor() { return processor; }

signals:
    void preprocessorBlockReady(short *ptr);
    void processorBlockReady(double *ptr);
    void changeUsedMemory(int preprocessor_blocks_count, int processor_blocks_count, int temp_buffer_postion);
public slots:
    void cacheRawData();
    void cachePreprocessedData(double avg);
    void notifyAboutReadyProcessor();

private:
    QVector<short*> preprocessor_blocks;
    QVector<double*> processor_blocks;
    int preprocessor_state;
    int processor_state;
    int preprocessor_block_size;
    int processor_block_size;
    int device_buffer_size;
    short *temp_preprocessor_buffer;
    int temp_preprocessor_buffer_position;
    double *temp_processor_buffer;
    int temp_processor_buffer_position;
    QTimer timer;
    PreprocessorThread *preprocessor;
    ProcessorThread *processor;
    int timer_interval;

    void callPreprocessorIfRequired();
    void callProcessorIfRequired();
    void emitUsedMemoryChanged();
    void copyDataToTempBuffer(short *data, int size);

protected:
    void run();
};

#endif // STORAGETHREAD_H
