#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    socket = new QUdpSocket(this);
        bool result =  socket->bind(QHostAddress::AnyIPv4, 45454);
        qDebug() << result;
        if(result)
        {
            qDebug() << "PASS";
        }
        else
        {
            qDebug() << "FAIL";
        }
        processPendingDatagrams();
        connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()),Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
}

void MainWindow::processPendingDatagrams()
 {
    qDebug() << "in !";
    QHostAddress sender;
    u_int16_t port;
    while (socket->hasPendingDatagrams())
    {
         QByteArray datagram;
         datagram.resize(socket->pendingDatagramSize());
         socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);
        qDebug() <<"Message From :: " << sender.toString();
        qDebug() <<"Port From :: "<< port;
        qDebug() <<"Message :: " << datagram;
    }
}
