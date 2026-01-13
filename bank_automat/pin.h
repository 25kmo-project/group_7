#ifndef PIN_H
#define PIN_H

#include <QDialog>

namespace Ui {
class Pin;
}

class Pin : public QDialog
{
    Q_OBJECT

public:
    explicit Pin(QWidget *parent = nullptr);
    ~Pin();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Pin *ui;
};

#endif // PIN_H
