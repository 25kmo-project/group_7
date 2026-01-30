# Group 7 Project
 
 # 📘 Projektiraportti  
**Projektin nimi: Pankkiautomaatti**  
**Tekijät: Valtteri Sippala, Vili Virnes, Aleksi Jussila ja Santeri Rautio**  
**2026 kevät**  

---
(Käännetään jossain vaiheessa enkuksi?)
## 1. Johdanto


Tämä projekti toteuttaa pankkiautomaatin käyttöliittymän Qt:lla ja C++:lla. Sovellus kommunikoi REST‑rajapinnan kautta backendin kanssa ja mahdollistaa käyttäjälle kirjautumisen, tilitietojen tarkastelun sekä rahansiirrot.

(Lisää tähän joku kuva)

---

## 2. Projektin tavoitteet

- Toteuttaa toimiva käyttöliittymä pankkiautomaatille  
- Harjoitella Qt‑kehitystä ja C++‑ohjelmointia  
- Toteuttaa REST API ‑kutsut ja JSON‑datan käsittely  
- Rakentaa selkeä ja turvallinen kirjautumislogiikka  
- Toteuttaa nostojen, talletusten, tilisiirron ja tilitapahtumien näytöt.
- Harjoitella ryhmässä työskentelyä



---

## 3. Käytetyt teknologiat

| Teknologia | Käyttötarkoitus |
|-----------|-----------------|
| Qt       | Käyttöliittymä |
| C++       | Sovelluslogiikka |
| REST API  | Backend‑yhteydet |
| MySQL     | Tietokanta |
| JSON      | Datan siirto backendin ja frontendin välillä |

---
(Lisää tähän kuva)

## 4. Sovelluksen arkkitehtuuri

- MainWindow hoitaa kirjautumisen
- Debit / Credit- kortilla kirjautuessa aukeaa päävalikko  
- Yhdistelmäkortilla kirjautuessa avautuu "Valitse kortti" ikkuna. Jonka jälkeen voi valita kortin.
- Accountinfo näyttää tilitiedot  
- Withdraw ja Deposit hoitavat rahansiirrot  
- Data‑ikkuna näyttää käyttäjän henkilötiedot  
- Sovelluksella voi siirtää rahaa käyttäjän tilien välillä
- Sovelluksessa voi selata tilitapahtumia

(Lisää tähän kuva ER-kaaviosta)


### 4.1 Api - kutsut
- Qt:ssa API‑kutsut tehdään QNetworkAccessManager‑olion avulla. 
- Ensin luodaan QNetworkRequest ja asetetaan URL sekä otsikot. 
- Lähetetään pyyntö (get, post, put, delete)
- Odotetaan vastausta signaalilla finished

- (Tähän joku esim kuva koodista + kuva jostain pyynnöstä)


### 4.1 JSON - vastaukset
- ( Kirjoita JSON- vastauksista + kuva vastauksesta)


### 4.2 Ikkunoiden kommunikointi

- Kirjoita miten ikkunat kommunikoi. esim takaisin painike eli void backRequested();
---

## 5. Toiminnallisuudet

### 5.1 Kirjautuminen
- Käyttäjä syöttää kortin numeron ja PIN‑koodin  
- Sovellus lähettää POST‑pyynnön backendille  
- Backend palauttaa tokenin ja käyttäjän tiedot  
- Virhetilanteet näytetään käyttöliittymässä  

### 5.2 Tilitietojen näyttäminen
- Accountinfo hakee tilin tiedot automaattisesti  
- Näytetään: Tyyppi, tilinumero, saldo, luottoraja  

### 5.3 Nosto
- Käyttäjä valitsee summan tai syöttää sen itse  
- Sovellus tarkistaa summan kelpoisuuden  
- Backend estää debit‑tilin miinukselle menon  
- Onnistunut nosto päivittää saldon  
- Backend estää luoton ylittymisen

### 5.4 Talletus
- Käyttäjä syöttää talletettavan summan  
- Backend päivittää saldon  
- Accountinfo näyttää uuden saldon  

### 5.5 Rahan siirto
- Käyttäjä pystyy siirtämään rahaa tilinsä välillä.
- Backend hoitaa siirron
- Accountinfo päivittää saldon

### 5.6 Henkilötiedot
- Data‑ikkuna näyttää käyttäjän nimen, osoitteen ja muut tiedot  
- Tiedot haetaan backendistä GET‑pyynnöllä  

### 5.7 Tilitapahtumat
- Käyttäjä pystyy selamaan tilitapahtumia
- Tiedot haetaan backendistä log-taulusta.
---


## 6. Käyttöliittymä

- Kirjautumisikkuna sisältää kirjautumiskentät  
- Accountinfo näyttää tilitiedot ja toiminnot  
- Withdraw, Deposit, siirto ja tilitapahtumat ovat erillisiä toimintoja
- Valitsekortti-ikkuna avautuu yhdistelmäkortilla  

(Tähän kuvia käyttöliittymästä)

## 7. Lisäominaisuudet
- (Kuvaile lisäominaisuudet + kuva?)



## 7. Haasteita
(Jokainen kirjoittaa omia?)

- Aluksi oli haasteita ymmärtää kokonaiskuva projektista
- ER- Kaaviota piti korjata useita kertoja, ennekuin se tuli valmiiksi
- 


## 8. Testaus

## 9. Johtopäätökset

## 10. Tilakaavio 





