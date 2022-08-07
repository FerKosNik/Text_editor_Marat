#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QListView>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QDir>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRemoveButton();
    void onAddButton();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui_;
    QStandardItemModel model_;
};
#endif // MAINWINDOW_H
