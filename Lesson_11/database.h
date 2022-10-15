#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define DATABASE_HOSTNAME   "NameDataBase"
#define DATABASE_NAME       "Name.db"

#define TABLE               "NameTable"
#define TABLE_TASK          "Task"
#define TABLE_DEADLINE      "Deadline"
#define TABLE_PROGRESS      "Progress"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();

private:
    QSqlDatabase    db;

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

public slots:
    bool inserIntoTable(const QVariantList &data);
    bool inserIntoTable(const QString &task, const QString &deadline, const QString &progress);
    bool removeRecord(const int id);
};

#endif // DATABASE_H
