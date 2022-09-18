#include "savetask.h"
#include <QDataStream>


SaveTask::SaveTask() : file(nullptr)
{

}
SaveTask::~SaveTask()
{
    if (file) file->close();
}

void SaveTask::loadTasks()
{
    file = new QFile("./Tasks.bin", this); // Файл с данными
    // о городах
    if (!file) return;
    if (!file->open(QIODevice::ReadWrite))
    {
        emit initEnd(false);
        return;
    }
    emit initEnd(true);
}

void SaveTask::getNextTask()
{
    if (file->atEnd()) return;
    QDataStream stream(file);
    int len = 0;
    stream.readRawData((char*)&len, sizeof len);
    QByteArray b;
    b.resize(len);
    stream.readRawData(b.data(), len);
    QString taskName = QString::fromUtf8(b);
    stream.readRawData((char*)&len, sizeof len);
    b.resize(len);
    stream.readRawData(b.data(), len);
    QString deadLine = QString::fromUtf8(b);
    stream.readRawData((char*)&len, sizeof len);
    b.resize(len);
    stream.readRawData(b.data(), len);
    QString curProgress = QString::fromUtf8(b);
    emit loadTask(taskName, deadLine, curProgress);
}


void SaveTask::writeNewInformation(QString taskName, QString deadLine, QString curProgress)
{
    file = new QFile("./Tasks.bin", this);
    QDataStream stream(file);
    auto bytes = taskName.toUtf8();
    int len = bytes.length();
    stream.writeRawData((char*)&len, sizeof len);
    stream.writeRawData(bytes.data(), len);
    bytes = deadLine.toUtf8();
    len = bytes.length();
    stream.writeRawData((char*)&len, sizeof len);
    stream.writeRawData(bytes.data(), len);
    bytes = curProgress.toUtf8();
    len = bytes.length();
    stream.writeRawData((char*)&len, sizeof len);
    stream.writeRawData(bytes.data(), len);
}
