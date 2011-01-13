#ifndef GENERATORTHREAD_H
#define GENERATORTHREAD_H

#include <QThread>
#include <QMutex>

class GeneratorThread : public QThread
{
    Q_OBJECT
public:
    explicit GeneratorThread(int i, QObject *parent = 0);
    void stop();

signals:
    void newDataGenerated();

public slots:

protected:
    void run();

private:
    int interval;
    bool shouldStop;
    QMutex mutex;

};

#endif // GENERATORTHREAD_H
