#include "generatorthread.h"

GeneratorThread::GeneratorThread(int i, QObject *parent) :
    QThread(parent), interval(i)
{
    this->moveToThread(this);
}

void GeneratorThread::run()
{
    while(true)
    {
        emit newDataGenerated();
        sleep(interval);
    }
}
