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
    explicit StorageThread(int bs, QObject *parent = 0);
    ~StorageThread();

signals:
    void storageBlockReady(int *ptr);
    void changeUsedMemory(int blocks_count, int size);
public slots:
    void cacheRawData();
    void notifyAboutReadyProcessor();

private:
    QVector<int*> blocks;
    int processor_state;
    int block_size;

    void callProcessorIfRequired();
    void emitUsedMemoryChanged();
};

#endif // STORAGETHREAD_H
