#ifndef GENERATORTHREAD_H
#define GENERATORTHREAD_H

#include <QThread>

class GeneratorThread : public QThread
{
    Q_OBJECT
public:
    explicit GeneratorThread(int i, QObject *parent = 0);

signals:
    void newDataGenerated();

public slots:

protected:
    void run();

private:
    int interval;

};

#endif // GENERATORTHREAD_H
