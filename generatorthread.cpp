#include "generatorthread.h"
#include <QMutexLocker>

GeneratorThread::GeneratorThread(int i, QObject *parent) :
    QThread(parent), interval(i), shouldStop(false)
{
    this->moveToThread(this);
}

void GeneratorThread::run()
{
    while (true)
    {
        QMutexLocker locker(&mutex);
        if (shouldStop)
        {
            locker.unlock();
            return;
        }
        else
        {
            locker.unlock();
        }

        emit newDataGenerated();
        sleep(interval);
    }
}

void GeneratorThread::stop()
{
    QMutexLocker locker(&mutex);
    shouldStop = true;
    locker.unlock();
}
