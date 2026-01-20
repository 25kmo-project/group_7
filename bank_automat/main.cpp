#include "mainwindow.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile("styles.qss");  // Jos tiedosto on projektikansiossa
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);  // Aseta koko appille – vaikuttaa kaikkiin ikkunoihin
        qDebug() << "QSS loaded OK!";
    } else {
        qDebug() << "QSS load failed:" << styleFile.errorString();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
