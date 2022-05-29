#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "parsetext.h"

namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    private slots:
    void on_plainTextEdit_textChanged();
    // текста
    private:
    Ui::MainWindow *ui;
    ParseText *parseText;
};

#endif // MAINWINDOW_H
