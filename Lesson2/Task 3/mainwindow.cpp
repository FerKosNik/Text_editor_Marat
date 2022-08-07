#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QMessageBox>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(4);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "No" << "Имя компьютера" << "IP адрес" << "MAC адрес");
    //ui->tableWidget->verticalHeader()->setVisible(false);
    int count = 1;
    for(int i = 0; i <= ui->tableWidget->columnCount();i++)
    {
        for(int j = 0; j <= ui->tableWidget->rowCount();j++)
        {
            if(j == 0)
            {
                QTableWidgetItem *itm  = new QTableWidgetItem(tr("%1").arg(count));
                ui->tableWidget->setItem(i, j, itm);

            }
            if(j == 1)
            {
                QTableWidgetItem *itm  = new QTableWidgetItem(tr("PC №%1").arg(count));
                ui->tableWidget->setItem(i, j, itm);
            }
            if(j == 2)
            {
                QTableWidgetItem *itm  = new QTableWidgetItem(tr("192.168.0.%1").arg(count));
                ui->tableWidget->setItem(i, j, itm);
            }
            if(j == 3)
            {
                QTableWidgetItem *itm  = new QTableWidgetItem(tr("00:aa:00:64:c8:0%1").arg(count));
                ui->tableWidget->setItem(i, j, itm);
                count++;
            }

        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QList<QTableWidgetItem *> selList = ui->tableWidget->selectedItems();

        for (QTableWidgetItem *item  : selList) {
            item->setBackgroundColor(Qt::red);
            item->setSelected(false);
        }
}

