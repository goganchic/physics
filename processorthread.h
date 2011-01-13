#ifndef PROCESSORTHREAD_H
#define PROCESSORTHREAD_H

#include <QThread>

class ProcessorThread : public QThread
{
    Q_OBJECT
public:
    explicit ProcessorThread(int bs, QObject *parent = 0);

protected:
    void run();

signals:
    void processorReady(double avg);
    void dataProcessed(double res);

public slots:
    void processData(int *ptr);

private:
    int block_size;

};

#endif // PROCESSORTHREAD_H
