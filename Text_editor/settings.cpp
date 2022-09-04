#include "settings.h"
#include "ui_settings.h"


Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    installEventFilter(this);
}

Settings::~Settings()
{
    delete ui;
}


void Settings::onRussian()
{
    switchLanguage("ru");
}

void Settings::onEnglish()
{
    switchLanguage("en");
}

void Settings::switchLanguage(const QString &language)
{

    translator.load(":/QtLanguage_" + language);
    qApp->installTranslator(&translator);
    emit signalLang(language);
    ui->retranslateUi(this);
}


void Settings::on_languageBox_activated(int index)
{
    if(index == 0) onRussian();
    else onEnglish();
}


void Settings::on_styleBox_activated(int index)
{
    if(index==0)
    {
        QFile styleSheetFile(":/styleSheets/Integrid.qss");
        styleSheetFile.open(QFile::ReadOnly);
        styleSheet = QLatin1String(styleSheetFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
    else
    {
        QFile styleSheetFile(":/styleSheets/Diffnes.qss");
        styleSheetFile.open(QFile::ReadOnly);
        styleSheet = QLatin1String(styleSheetFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}

