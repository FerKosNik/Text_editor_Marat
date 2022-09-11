#ifndef THREADFINDER_H
#define THREADFINDER_H

#include <QObject>
#include <QThread>

class ThreadFinder: public QThread
{
    Q_OBJECT
public:
    explicit ThreadFinder(QString dir, QString File, QObject *parent = nullptr);
    void findStop() {toWork = false;}
protected:
    void run();
private:
    QString dir;
    QString file;
    bool toWork;
    signals:
    void stopedThread();
    void writeFoundPath(QString);
    void findFile(QString);
};
#endif // THREADFINDER_H

