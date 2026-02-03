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

/**
 * @class MainWindow
 * @brief Sovelluksen pääikkuna, joka vastaa kirjautumisesta ja inaktiivisuuden hallinnasta.
 *
 * MainWindow hoitaa:
 * - käyttäjän kirjautumisen backendin kautta
 * - inaktiivisuusajastimet (10s ja 30s)
 * - hiiren ja näppäimistön tapahtumien kuuntelun
 * - debit/credit/dual-korttien käsittelyn
 * - tilitietoikkunan avaamisen onnistuneen kirjautumisen jälkeen
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Luo MainWindow-olion ja alustaa käyttöliittymän sekä ajastimet.
     * @param parent Vanhempi QWidget.
     *
     * Konstruktorissa:
     * - alustetaan UI
     * - luodaan 30s ja 10s inaktiivisuusajastimet
     * - yhdistetään signaalit login-nappiin ja Enter-painallukseen
     * - asennetaan event filter hiiren ja näppäimistön seuraamiseksi
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Tuhoaa MainWindow-olion ja vapauttaa käyttöliittymän.
     */
    ~MainWindow();

protected:
    /**
     * @brief Kuuntelee hiiren ja näppäimistön tapahtumia.
     * @param obj Tapahtuman kohdeobjekti.
     * @param event Tapahtuma (hiiri, näppäimistö).
     * @return true jos tapahtuma käsiteltiin, muuten false.
     *
     * Tämän avulla nollataan inaktiivisuusajastimet aina kun käyttäjä tekee jotain.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

    /**
     * @brief Sulkee koko sovelluksen, kun pääikkuna suljetaan.
     * @param event Sulkemistapahtuma.
     */
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QTimer *inactivityTimer;       ///< 30 sekunnin inaktiivisuusajastin
    QTimer *loginInactivityTimer;  ///< 10 sekunnin inaktiivisuusajastin

    /**
     * @brief Hakee käyttäjän tilitiedot ja avaa Accountinfo-ikkunan.
     * @param userId Kirjautuneen käyttäjän ID.
     * @param type Tilin tyyppi: "debit", "credit" tai "dual".
     *
     * Lähettää GET-pyynnön backendille ja luo Accountinfo-ikkunan,
     * jolle asetetaan token, käyttäjänimi ja tilitiedot.
     */
    void openAccountWindow(int userId, const QString &type);

private slots:
    /**
     * @brief Lähettää kirjautumispyynnön backendille.
     *
     * Kerää käyttäjän syöttämän korttinumeron ja PIN-koodin,
     * muodostaa JSON-datan ja lähettää POST-pyynnön ApiClientin kautta.
     *
     * Vastauksen valmistuttua kutsutaan loginAction().
     */
    void btnLoginSlot();

    /**
     * @brief Käsittelee backendiltä saadun kirjautumisvastauksen.
     * @param reply Backendin palauttama QNetworkReply.
     *
     * Toiminnot:
     * - tarkistaa virhekoodit (esim. -4078)
     * - tarkistaa löytyykö token
     * - näyttää virheilmoitukset epäonnistuneessa kirjautumisessa
     * - tallentaa tokenin ApiClientiin
     * - avaa oikean tilinäkymän (debit/credit/dual)
     */
    void loginAction(QNetworkReply *reply);

    /**
     * @brief Käsittelee dual-kortin valinnan (debit/credit).
     * @param type Käyttäjän valitsema korttityyppi.
     */
    void onCardSelected(QString type);

    /**
     * @brief Kutsutaan, kun 10 sekunnin inaktiivisuusajastin laukeaa.
     *
     * Tyhjentää kirjautumiskentät ja käynnistää ajastimen uudelleen.
     */
    void onLoginInactivityTimeout();

    /**
     * @brief Kutsutaan, kun 30 sekunnin inaktiivisuusajastin laukeaa.
     *
     * Sulkee kaikki muut ikkunat ja palauttaa sovelluksen alkutilaan
     * luomalla uuden MainWindow-instanssin.
     */
    void onInactivityTimeout();

    /**
     * @brief Sulkee pääikkunan, kun sulkupainiketta painetaan.
     */
    void on_pushButton_clicked();

};

#endif // MAINWINDOW_H
