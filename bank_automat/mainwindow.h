#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "environment.h"
#include "accountinfo.h"
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;



private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QTimer *inactivityTimer; // 30sek
    void openAccountWindow(int userId, const QString &type);

private slots:
    void btnLoginSlot();
    void loginAction(QNetworkReply *reply);
    void onCardSelected(QString type);
    //void onWithdrawReply(QNetworkReply *reply);

    void onInactivityTimeout();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
