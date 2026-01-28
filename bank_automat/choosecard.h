#ifndef CHOOSECARD_H
#define CHOOSECARD_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class ChooseCard;
}

class ChooseCard : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCard(QWidget *parent = nullptr);
    ~ChooseCard();

    void setUsername(const QString &newUsername);
    void setChooseCard(const QByteArray &newChooseCard);

signals:
    void cardSelected(QString type);
    void backRequested();

private slots:
    void btnDEBITClicked();
    void btnCREDITClicked();
    void btnBackClicked();

private:
    Ui::ChooseCard *ui;

    QString username;
    QByteArray token;

    void openAccount(const QJsonObject &obj);
};

#endif // CHOOSECARD_H
