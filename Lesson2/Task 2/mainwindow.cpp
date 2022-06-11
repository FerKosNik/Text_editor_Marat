#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    QString path = "D:/Icons/";
    ui_->setupUi(this);

    ui_->listView->setModel(&model_);
    ui_->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui_->listView->setDragEnabled(true);
    ui_->listView->viewport()->setAcceptDrops(true);
    ui_->listView->setDropIndicatorShown(true);
    ui_->listView->setDragDropMode(QAbstractItemView::InternalMove);


    model_.appendRow(new QStandardItem(QIcon(path + "c++.png"),"C++"));
    model_.appendRow(new QStandardItem(QIcon(path + "java.png"),"Java"));
    model_.appendRow(new QStandardItem(QIcon(path + "Python.png"),"Python"));
    model_.appendRow(new QStandardItem(QIcon(path + "C#.png"),"C#"));
    model_.appendRow(new QStandardItem(QIcon(path + "PHP.png"),"PHP"));
    model_.appendRow(new QStandardItem(QIcon(path + "JavaScript.png"),"JavaScript"));

    connect(ui_->removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveButton);
    connect(ui_->addButton, &QPushButton::clicked, this, &MainWindow::onAddButton);



    QObject :: connect (this, SIGNAL (itemDoubleClicked (QStandardItem*)), this, SLOT ());
    on_checkBox_stateChanged(ui_->checkBox->isChecked());




    setWindowTitle("Lang list");
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::onRemoveButton()
{
    auto selection_model = ui_->listView->selectionModel();
    if (selection_model->hasSelection()) {
        auto cur_ind = selection_model->currentIndex();
        if (cur_ind.isValid()) {
            model_.removeRow(cur_ind.row());
        }
    }
}

void MainWindow::onAddButton()
{
    QString path = "D:/Icons/";
    if (!ui_->langNameEdit->text().isEmpty()) {
        model_.appendRow(new QStandardItem(QIcon(path + "default.png"), ui_->langNameEdit->text()));
        ui_->langNameEdit->clear();
    }
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1)ui_->listView->setViewMode(QListView::IconMode);
    else ui_->listView->setViewMode(QListView::ListMode);
}

