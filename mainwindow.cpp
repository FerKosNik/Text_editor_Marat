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


void MainWindow::on_commandLinkButton_2_clicked()
{
    double c = 0;
    double a = ui->inputNumberEdit_4->text().toDouble();
    double b = ui->inputNumberEdit_5->text().toDouble();
    double gradOrRad = ui->inputNumberEdit_6->text().toDouble();
    if(a <= 0 || b <= 0 || gradOrRad <= 0) ui->ResultSide->setText("Исходные значения не могут быть меньше или равны 0.");
    else if(ui->GradSwitch->isChecked()) {
        c = qSqrt(qPow(b,2)+qPow(a,2) - 2 * b * a * qCos(gradOrRad));
        ui->ResultSide->setText(QString::number(c));
    }
    else if(ui->RadSwitch->isChecked()) {
        c = qSqrt(qPow(b,2)+qPow(a,2) - (2 * b * a * gradOrRad));
        ui->ResultSide->setText(QString::number(c));
    }

}


void MainWindow::on_Concatenation_clicked()
{
    QString a = ui->FirstTextEdit->toPlainText();
    ui->SecondTextEdit->insertPlainText(' ' + a);
}


void MainWindow::on_Change_clicked()
{
    QString b = ui->SecondTextEdit->toPlainText();
    ui->FirstTextEdit->setPlainText(b);
}


void MainWindow::on_invertToHtml_clicked()
{
    if(ui->FirstWindow->isChecked()) ui->ThirdTextEdit->setHtml("<font color='blue'><b><i><s>" + ui->FirstTextEdit->toPlainText() + "</s></i></b></font>");
    else if(ui->SecondWindow->isChecked()) ui->ThirdTextEdit->setHtml("<font color='green'><b><i><u>" + ui->SecondTextEdit->toPlainText() + "</u></i></b></font>");
}

