#include "listmodel.h"
#include "database.h"

ListModel::ListModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    this->updateModel();
}

QVariant ListModel::data(const QModelIndex & index, int role) const {

    int columnId = role - Qt::UserRole - 1;
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> ListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TaskRole] = "task";
    roles[DeadlineRole] = "deadline";
    roles[ProgressRole] = "progress";
    return roles;
}

void ListModel::updateModel()
{
    this->setQuery("SELECT id, " TABLE_TASK ", " TABLE_DEADLINE ", " TABLE_PROGRESS " FROM " TABLE);
}

int ListModel::getId(int row)
{
    return this->data(this->index(row, 0), IdRole).toInt();
}

int ListModel::countRow()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM " TABLE " ;");
    query.exec();
    query.first();

    return query.value(0).toInt();
}

void ListModel::exportCSV()
{
    QFile csvFile("C:/example/excelExample.csv");

    if(csvFile.open( QIODevice::WriteOnly ))
    {
        QTextStream textStream( &csvFile );
        QStringList stringList;

        for( int row = 0; row < this->countRow(); row++ )
        {
            stringList.clear();
            for( int column = 0; column < 4; column++ )
            {
                stringList << this->data(this->index(row, column), Qt::UserRole + 1 + column).toString();
            }
            textStream << stringList.join( ';' )+"\n";
        }
        csvFile.close();
    }
}
