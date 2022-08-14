#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

private slots:
    void newDocument();

    void open();

    void openForRead();

    void save();

    void saveAs();

    void exit();

    void about();

    void onRussian();

    void onEnglish();

    void switchLanguage(const QString &language);

private:
    Ui::TextEditor *ui;
    QString currentFile;
};
#endif // TEXTEDITOR_H
