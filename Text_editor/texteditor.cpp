#include <QFile>
#include <QFileDialog>
#include <QTextEdit>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDockWidget>
#include <QTreeWidget>
#include <QPrinter>
#include <QPrintDialog>
#include <QFontDialog>
#include <QMdiSubWindow>

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

    mdi_area_ = new QMdiArea(this);
    mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->setCentralWidget(mdi_area_);

    QTreeView *tree = new QTreeView;
    tree->setModel(dirmodel);
    auto dock_wgt = new QDockWidget{"Dock widget", this};
    dock_wgt->setWidget(tree);
    addDockWidget(Qt::LeftDockWidgetArea, dock_wgt);


    connect(ui->actionNew, &QAction::triggered, this, &TextEditor::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &TextEditor::open);
    connect(ui->actionReadOnly, &QAction::triggered, this, &TextEditor::openForRead);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &TextEditor::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &TextEditor::exit);
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::about);
    connect(ui->actionSettings, &QAction::triggered, this, &TextEditor::settings);
    connect(&parameters, &Settings::signalLang, this, &TextEditor::slotLang);
    connect(ui->actionFont, &QAction::triggered, this, &TextEditor::selectFont);
    connect(ui->actionPrint, &QAction::triggered, this, &TextEditor::print);
    connect(ui->actioncopyFormat, &QAction::triggered, this, &TextEditor::copyFont);
    connect(ui->actionpasteFormat, &QAction::triggered, this, &TextEditor::pasteFont);
    connect(ui->actionLeftAlign, &QAction::triggered, this, &TextEditor::leftAlign);
    connect(ui->actionCenterAlign, &QAction::triggered, this, &TextEditor::centerAlign);
    connect(ui->actionRightAlign, &QAction::triggered, this, &TextEditor::rightAlign);
}


TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::currentWindow()
{
        QMdiArea* mdiArea = dynamic_cast<QMdiArea*>( centralWidget() );
        if(!mdiArea) return;

        QMdiSubWindow * subWindow = mdiArea->currentSubWindow();
        if(!subWindow) return;

        textEdit = dynamic_cast<QTextEdit*>(subWindow->focusWidget());
        if(!textEdit) return;
}

void TextEditor::newDocument()
{
    auto text_edit1 = new QTextEdit{};
    QString curFile = tr("document%1.txt").arg(sequenceNumber++);
    text_edit1->setWindowTitle(curFile + "[*]");
    QMdiSubWindow *mdiWindows = mdi_area_->addSubWindow(text_edit1);

    mdiWindows->show();
}

void TextEditor::open()
{
    currentWindow();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), 0, tr("Text files(*.txt)"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается открыть файл: ") + file.errorString());
        delete textEdit;
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    textEdit->setText(text);

    file.close();
}

void TextEditor::openForRead()
{
    currentWindow();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл только для чтения"), 0, tr("Текстовый файл(*.txt)"));
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается открыть файл: ") + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    textEdit->setReadOnly(true);
    textEdit->clear();
    file.open(QFile::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString text = in.readAll();
    textEdit->setText(text);
    file.close();
}

void TextEditor::save()
{
    currentWindow();
    QString fileName;
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, tr("Сохранить"), 0, tr("Текстовый файл(*.txt)"));
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается сохранить файл: ") + file.errorString());
        delete textEdit;
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = textEdit->toPlainText();

    out << text;
    file.close();
}

void TextEditor::saveAs()
{
    currentWindow();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить как"), 0, tr("Текстовый файл(*.txt)"));
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Внимание"), tr("Не удается сохранить файл: ") + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = textEdit->toPlainText();
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

void TextEditor::selectFont()
{
    currentWindow();
    QFont font = textEdit->textCursor().charFormat().font();
    QFontDialog fntDlg(font,this);
    bool b[] = {true};
    font = fntDlg.getFont(b);
    if (b[0])
    {
    QTextCharFormat fmt;
    fmt.setFont(font);
    textEdit->textCursor().setCharFormat(fmt);
    }
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

void TextEditor::print()
{

    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle("Print");
    if (dlg.exec() != QDialog::Accepted)
    return;

}

void TextEditor::copyFont()
{
    currentWindow();
    copiedFont = textEdit->textCursor().charFormat().font();
}

void TextEditor::pasteFont()
{
    currentWindow();
    QTextCharFormat fmt;
    fmt.setFont(copiedFont);
    textEdit->textCursor().setCharFormat(fmt);
}

void TextEditor::leftAlign()
{
    currentWindow();
    textEdit->setAlignment(Qt::AlignLeft);
}

void TextEditor::centerAlign()
{
    currentWindow();
    textEdit->setAlignment(Qt::AlignCenter);
}

void TextEditor::rightAlign()
{
    currentWindow();
    textEdit->setAlignment(Qt::AlignRight);
}

void TextEditor::justifyAlign()
{
    currentWindow();
    textEdit->setAlignment(Qt::AlignJustify);
}
