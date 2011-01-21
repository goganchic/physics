#ifndef STORAGETHREAD_H
#define STORAGETHREAD_H

#include <QThread>
#include <QVector>

#define BLOCK_SIZE 512
#define PROCESSOR_READY 0
#define PROCESSOR_BUSY 1

class StorageThread : public QThread
{
    Q_OBJECT
public:
    explicit StorageThread(int bs, int dbs, QObject *parent = 0);
    ~StorageThread();

signals:
    void storageBlockReady(short *ptr);
    void changeUsedMemory(int blocks_count, int size, int temp_buffer_postion);
public slots:
    void cacheRawData();
    void notifyAboutReadyProcessor();

private:
    QVector<short*> blocks;
    int processor_state;
    int block_size;
    int device_buffer_size;
    short *temp_buffer;
    int temp_buffer_position;

    void callProcessorIfRequired();
    void emitUsedMemoryChanged();
    void copyDataToTempBuffer(short *data, int size);
};

#endif // STORAGETHREAD_H
