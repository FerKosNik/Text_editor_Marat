#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    connect(ui->actionNew, &QAction::triggered, this, &TextEditor::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &TextEditor::open);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &TextEditor::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &TextEditor::exit);
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::about);

#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::newDocument()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void TextEditor::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void TextEditor::save()
{
    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void TextEditor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void TextEditor::exit()
{
    QCoreApplication::quit();
}

void TextEditor::about()
{
   QMessageBox::about(this, tr("About MDI"),
                tr("The <b>Notepad</b> example demonstrates how to code a basic "
                   "text editor using QtWidgets"));

}

