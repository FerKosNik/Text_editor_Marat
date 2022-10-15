#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class ListModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TaskRole,
        DeadlineRole,
        ProgressRole
    };

    explicit ListModel(QObject *parent = 0);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

signals:

private:
    int countRow();
public slots:
    void exportCSV();
    void updateModel();
    int getId(int row);
};

#endif // LISTMODEL_H
