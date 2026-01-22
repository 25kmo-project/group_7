#include "mainwindow.h"
#include <QFile>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QApplication>
#include <QFileInfo>
#include <QTimer>

QString loadStyleSheet(const QString &path) { QFile file(path); if (!file.open(QFile::ReadOnly | QFile::Text)) { qDebug() << "Failed to load QSS:" << file.errorString(); return ""; } return QString::fromUtf8(file.readAll()); }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString basePath = QCoreApplication::applicationDirPath() + "/../..";

    QString qssPath = basePath + "/styles.qss";

    qDebug() << "Absolute path:" << QFileInfo(qssPath).absoluteFilePath();
    qDebug() << "Exists:" << QFile::exists(qssPath);

    qDebug() << QFile::exists(qssPath);


    a.setStyleSheet(loadStyleSheet(qssPath));
    qDebug() << "Initial QSS loaded";

    // QFileSystemWatcher *watcher = new QFileSystemWatcher(); // watcher->addPath(qssPath); // QObject::connect(watcher, &QFileSystemWatcher::fileChanged, [&](const QString &path){ // qDebug() << "WATCHER TRIGGERED"; // QTimer::singleShot(100, [&](){ // a.setStyleSheet(loadStyleSheet(qssPath)); // watcher->addPath(qssPath); // }); // });


    MainWindow w;
    w.show();
    return a.exec();
}
