#ifndef PREPROCESSORTHREAD_H
#define PREPROCESSORTHREAD_H

#include <QThread>

class PreprocessorThread : public QThread
{
    Q_OBJECT
public:
    explicit PreprocessorThread(int bs, QObject *parent = 0);

protected:
    void run();

signals:
    void dataProcessed(double avg);

public slots:
    void processData(short *ptr);

private:
    int block_size;
};

#endif // PREPROCESSORTHREAD_H
