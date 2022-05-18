#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_commandLinkButton_clicked()
{
    double x1, x2;
    double a = ui->inputNumberEdit->text().toDouble();
    double b = ui->inputNumberEdit_2->text().toDouble();
    double c = ui->inputNumberEdit_3->text().toDouble();
    if(a == 0) ui->ResultSquare->setText("Нет корней, перемеенная 'a' не может быть равна 0.");
    double D = qPow(b,2) - 4 * a * c;
    if(D < 0) ui->ResultSquare->setText("Квадратное уравнение действительных корней не имеет.");
    else if(D == 0) {
        x1 = (-b + qSqrt(D)) / 2 * a;
        ui->ResultX1->setText(QString::number(x1));
        ui->ResultX2->setText(QString::number(x1));
    }
    else{
        x1 = (-b + qSqrt(D)) / 2 * a;
        x2 = (-b - qSqrt(D)) / 2 * a;
        ui->ResultX1->setText(QString::number(x1));
        ui->ResultX2->setText(QString::number(x2));
    }
}

