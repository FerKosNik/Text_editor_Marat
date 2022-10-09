#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTranslator>

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
    connect(ui->actionReadOnly, &QAction::triggered, this, &TextEditor::openForRead);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &TextEditor::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &TextEditor::exit);
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::about);
    connect(ui->actionRussian, &QAction::triggered, this, &TextEditor::onRussian);
    connect(ui->actionEnglish, &QAction::triggered, this, &TextEditor::onEnglish);
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
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", 0, tr("Text files(*.txt)"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Внимание", "Не удается открыть файл: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void TextEditor::openForRead()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл только для чтения", 0, tr("Text files(*.txt)"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Внимание", "Не удается открыть файл: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->clear();
    file.open(QFile::ReadOnly | QIODevice::Text);
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
        fileName = QFileDialog::getSaveFileName(this, "Сохранить");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Внимание", "Не удается сохранить файл: " + file.errorString());
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
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Внимание", "Не удается сохранить файл: " + file.errorString());
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
   QString hlp;
   QString buttonName = ui->actionAbout->text();
   if (buttonName == "Справка") hlp = ":/help/helpRU.txt";
   else hlp = ":/help/helpENG.txt";
   QFile file(hlp);
   QString msg;
   if (file.open(QFile::ReadOnly)) {
       msg = file.readAll();
       if (buttonName == "Справка") QMessageBox::about(this, "Справка", msg);
       else QMessageBox::about(this, "About", msg);
   }
   else {
       QMessageBox::warning(this, "Внимание", "Отсутствует файл справки");
       return;
   }
   file.close();

}

void TextEditor::onRussian()
{
    switchLanguage("ru");
}

void TextEditor::onEnglish()
{
    switchLanguage("en");
}

void TextEditor::switchLanguage(const QString &language)
{
    QTranslator translator;
    translator.load(":/QtLanguage_" + language);
    qApp->installTranslator(&translator);

    ui->retranslateUi(this);
}
