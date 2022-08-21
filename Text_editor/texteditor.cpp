#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>


#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor)
{

    ui->setupUi(this);
    sPath = "C:/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    ui->treeView->setModel(dirmodel);
    this->setCentralWidget(ui->textEdit);
    installEventFilter(this);

    connect(ui->actionNew, &QAction::triggered, this, &TextEditor::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &TextEditor::open);
    connect(ui->actionReadOnly, &QAction::triggered, this, &TextEditor::openForRead);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &TextEditor::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &TextEditor::exit);
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::about);
    connect(ui->actionSettings, &QAction::triggered, this, &TextEditor::settings);
    connect(&parameters, &Settings::signalLang, this, &TextEditor::slotLang);
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), 0, tr("Text files(*.txt)"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается открыть файл: ") + file.errorString());
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл только для чтения"), 0, tr("Text files(*.txt)"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается открыть файл: ") + file.errorString());
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
        fileName = QFileDialog::getSaveFileName(this, tr("Сохранить"));
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается сохранить файл: ") + file.errorString());
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить как"));
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается сохранить файл: ") + file.errorString());
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
       QMessageBox::about(this, tr("Справка"), msg);
   }
   else {
       QMessageBox::warning(this, tr("Внимание"), tr("Отсутствует файл справки"));
       return;
   }
   file.close();

}

void TextEditor::switchLanguage(const QString &language)
{

    translator.load(":/QtLanguage_" + language);
    qApp->installTranslator(&translator);

    ui->retranslateUi(this);
}

void TextEditor::settings()
{
    parameters.show();
}

void TextEditor::slotLang(const QString &language)
{
    switchLanguage(language);
}

bool TextEditor::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this && event->type() == QEvent::KeyPress) {
        auto key_event = static_cast<QKeyEvent*>(event);
        if (key_event->key() == Qt::Key_S && key_event->modifiers() == Qt::CTRL) {
            save();
            return true;
        }
        if (key_event->key() == Qt::Key_O && key_event->modifiers() == Qt::CTRL) {
            open();
            return true;
        }
        if (key_event->key() == Qt::Key_X && key_event->modifiers() == Qt::CTRL) {
            exit();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}
