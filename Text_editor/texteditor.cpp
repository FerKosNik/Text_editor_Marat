#include <QFile>
#include <QFileDialog>
#include <QTextEdit>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>
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

    mdi_area_ = new QMdiArea(this);
    mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->setCentralWidget(mdi_area_);

    //Creating Tree with file directories
    sPath = "C:/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    tree = new QTreeView;
    tree->setModel(dirmodel);
    dock_wgt = new QDockWidget{this};
    dock_wgt->setWidget(tree);

    //Adding into dock widget search line with button
    searchEdit = new QLineEdit ;
    startFindButton = new QPushButton(this);
    startFindButton->setText(tr("Find"));
    QWidget *searchArea = new QWidget(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(searchEdit, 0, 0, 1, 3);
    layout->addWidget(startFindButton, 0, 5);
    searchArea->setLayout(layout);
    dock_wgt->setTitleBarWidget(searchArea);
    QString searchedPart = searchEdit->text();
    tree->keyboardSearch(searchedPart);
    addDockWidget(Qt::LeftDockWidgetArea, dock_wgt);
    connect(startFindButton, SIGNAL(clicked()), this, SLOT(findFileSlot()));

    //adding dock widget with info area
    auto info_dock_wgt = new QDockWidget{this};
    infoText = new QTextEdit(this);
    info_dock_wgt->setTitleBarWidget(infoText);
    addDockWidget(Qt::BottomDockWidgetArea, info_dock_wgt);

    //Creating new thread
    controllerl = new Controller(this);
    statusLabel = new QLabel(this);
    QStatusBar *statusBar = this->statusBar();
    statusBar->addWidget(statusLabel);
    connect(controllerl, SIGNAL(changFindPath(QString)), this,
    SLOT(changStatusLabel(QString)));
    connect(controllerl, SIGNAL(genPathOfFile(QString)), this,
    SLOT(printFindFile(QString)));
    connect(controllerl, SIGNAL((newFind())), infoText, SLOT(clear()));


    //Main commands
    connect(ui->actionNew, &QAction::triggered, this, &TextEditor::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &TextEditor::open);
    connect(ui->actionReadOnly, &QAction::triggered, this, &TextEditor::openForRead);
    connect(ui->actionSave, &QAction::triggered, this, &TextEditor::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &TextEditor::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &TextEditor::exit);
    //About
    connect(ui->actionAbout, &QAction::triggered, this, &TextEditor::about);
    //Settings
    connect(ui->actionSettings, &QAction::triggered, this, &TextEditor::settings);
    connect(&parameters, &Settings::signalLang, this, &TextEditor::slotLang);
    //Print
    connect(ui->actionPrint, &QAction::triggered, this, &TextEditor::printCurrentText);
    //Fonts
    connect(ui->actionFont, &QAction::triggered, this, &TextEditor::selectFont);
    connect(ui->actioncopyFormat, &QAction::triggered, this, &TextEditor::copyFont);
    connect(ui->actionpasteFormat, &QAction::triggered, this, &TextEditor::pasteFont);
    //Alignments
    connect(ui->actionLeftAlign, &QAction::triggered, this, &TextEditor::leftAlign);
    connect(ui->actionCenterAlign, &QAction::triggered, this, &TextEditor::centerAlign);
    connect(ui->actionRightAlign, &QAction::triggered, this, &TextEditor::rightAlign);
    //Date and time
    connect(ui->actionDateTime, &QAction::triggered, this, &TextEditor::addDateTime);
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

void TextEditor::printCurrentText()
{
    currentWindow();
    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle("Print");
    if (dlg.exec() != QDialog::Accepted) return;
    textEdit->print(&printer);
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

void TextEditor::addDateTime()
{
    currentWindow();
    QDateTime dateTime = QDateTime::currentDateTime();
    QTime time = dateTime.time();
    QDate date = dateTime.date();
    const QString month[] = {"", " января " , " февраля ", " марта ",
                                " апреля ", " мая ", " июня ",
                                " июля ", " августа ", " сентября ",
                                " октября ", " ноября ", " декабря ",};
    QString dstr = QString::number(date.day()) + month[date.month()] + QString::number(date.year()) +' '
                   + "Время: " + QString::number(time.hour()) + ":" + QString::number(time.minute());

    textEdit->textCursor().insertText(dstr);
}

void TextEditor::findFileSlot()
{
    QString linesearch = searchEdit->text();
    QModelIndex index = tree->currentIndex();
    QString curDir = dirmodel->filePath(index);
    if (curDir == "") return;
    if (linesearch.length() == 0) return;
    controllerl->startFind(curDir, linesearch);
}

void TextEditor::changStatusLabel(QString line)
{
    statusLabel->setText(line);
}

void TextEditor::printFindFile(QString str)
{
    infoText->append(str);
}

