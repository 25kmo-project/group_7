#ifndef CHOOSECARD_H
#define CHOOSECARD_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class ChooseCard;
}

/**
 * @class ChooseCard
 * @brief Ikkuna, jossa käyttäjä valitsee debit- tai credit-kortin dual-kortilla.
 *
 * Tämä dialogi avataan, kun käyttäjällä on dual-kortti ja hänen täytyy valita,
 * kummalla puolella (debit/credit) hän haluaa jatkaa. Dialogi lähettää valinnan
 * signaalina takaisin MainWindowille.
 */
class ChooseCard : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Luo ChooseCard-dialogin.
     * @param parent Vanhempi QWidget, yleensä nullptr.
     */
    explicit ChooseCard(QWidget *parent = nullptr);

    /**
     * @brief Tuhoaa ChooseCard-olion ja vapauttaa käyttöliittymän.
     */
    ~ChooseCard();

    /**
     * @brief Asettaa käyttäjänimen, joka välitetään tilitietoikkunalle.
     * @param newUsername Käyttäjän ID merkkijonona.
     */
    void setUsername(const QString &newUsername);

    /**
     * @brief Asettaa tokenin, jota käytetään backend-kutsuihin.
     * @param newChooseCard Token (JWT tms.) tavutaulukossa.
     */
    void setChooseCard(const QByteArray &newChooseCard);

signals:
    /**
     * @brief Lähetetään, kun käyttäjä valitsee debit- tai credit-kortin.
     * @param type Valittu korttityyppi ("debit" tai "credit").
     */
    void cardSelected(QString type);

    /**
     * @brief Lähetetään, kun käyttäjä haluaa palata takaisin edelliseen näkymään.
     */
    void backRequested();

private slots:
    /**
     * @brief Käsittelee debit-painikkeen painalluksen.
     */
    void btnDEBITClicked();

    /**
     * @brief Käsittelee credit-painikkeen painalluksen.
     */
    void btnCREDITClicked();

    /**
     * @brief Käsittelee takaisin-painikkeen painalluksen.
     */
    void btnBackClicked();

private:
    Ui::ChooseCard *ui;   ///< Käyttöliittymäolio
    QString username;     ///< Käyttäjän ID merkkijonona
    QByteArray token;     ///< Backend-token (JWT)

    /**
     * @brief Avaa tilitietoikkunan annetulla JSON-datalla.
     * @param obj Backendiltä saatu tilitieto-objekti.
     */
    void openAccount(const QJsonObject &obj);
};

#endif // CHOOSECARD_H
