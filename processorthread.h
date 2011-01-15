#ifndef PROCESSORTHREAD_H
#define PROCESSORTHREAD_H

#include <QThread>

class ProcessorThread : public QThread
{
    Q_OBJECT
public:
    explicit ProcessorThread(int bs, int t_1, int t_2, QObject *parent = 0);

protected:
    void run();

signals:
    void processorReady();
    void dataProcessed(double res1, double res2, double res3, double mt, double dt);

public slots:
    void processData(int *ptr);

private:
    int block_size;
    int t1;
    int t2;
    double mt;
    double mt2;
    double dt;
    int number;
};

#endif // PROCESSORTHREAD_H
