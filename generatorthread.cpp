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
        mutex.lock();
        if (shouldStop)
        {
            mutex.unlock();
            return;
        }
        else
        {
            mutex.unlock();
        }

        emit newDataGenerated();
        sleep(interval);
    }
}

void GeneratorThread::stop()
{
    QMutexLocker locker(&mutex);
    shouldStop = true;
}
