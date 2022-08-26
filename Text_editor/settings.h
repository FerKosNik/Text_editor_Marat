#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QTranslator>
#include <QFile>
#include <QKeyEvent>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:

    void onRussian();

    void onEnglish();

    void switchLanguage(const QString &language);

    void on_languageBox_activated(int index);

    void on_styleBox_activated(int index);

private:
    Ui::Settings *ui;
    QTranslator translator;
    QString styleSheet;
signals:
    void signalLang(const QString &language);
};

#endif // SETTINGS_H
