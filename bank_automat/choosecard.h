#ifndef CHOOSECARD_H
#define CHOOSECARD_H

#include <QDialog>

namespace Ui {
class ChooseCard;
}

class ChooseCard : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCard(QWidget *parent = nullptr);
    ~ChooseCard();
signals:
    //void cardSelected(QString type); //Välittää tiedon onko credit vai debit


private slots:
    void on_btn_Credit_clicked();

    void on_btn_Debit_clicked();

private:
    Ui::ChooseCard *ui;
};

#endif // CHOOSECARD_H
