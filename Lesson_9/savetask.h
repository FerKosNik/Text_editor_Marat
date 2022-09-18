#ifndef SAVETASK_H
#define SAVETASK_H

#include <QObject>
#include <QFile>

class SaveTask : public QObject
{
    Q_OBJECT
public:
    explicit SaveTask();
    virtual ~SaveTask();
    Q_INVOKABLE
    void loadTasks();
    Q_INVOKABLE
    void getNextTask();
    Q_INVOKABLE
    void writeNewInformation(QString taskName, QString deadLine, QString curProgress);
private:
    QFile *file;
signals:
    void initEnd(bool succed);
    void loadTask(QString taskName, QString deadLine, QString curProgress);
};

#endif // SAVETASK_H
