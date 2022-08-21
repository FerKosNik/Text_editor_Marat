#include "texteditor.h"
#include "settings.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleSheetFile(":/styleSheets/Integrid.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    qApp->setStyleSheet(styleSheet);
    TextEditor w;
    w.show();
    return a.exec();
}
