#ifndef DATA_H
#define DATA_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>

namespace Ui {
class Data;
}

class Data : public QDialog
{
    Q_OBJECT

public:
    explicit Data(QWidget *parent = nullptr);
    ~Data();

    void setTestData(const QByteArray &newTestData);

private:
    Ui::Data *ui;
    QByteArray testData;
};

#endif // DATA_H
