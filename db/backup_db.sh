#!/bin/bash

# 1. Määritellään polut
APP_DIR="/opt/bank-app"
BACKUP_DIR="$APP_DIR/backups"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
DB_CONTAINER="bank_db"

# 2. Siirrytään sovelluskansioon
cd $APP_DIR

# 3. Ladataan ympäristömuuttujat .env-tiedostosta
if [ -f .env ]; then
    # Tämä komento poimii .env-tiedostosta muuttujat skriptin käyttöön
    export $(grep -v '^#' $APP_DIR/.env | xargs)
else
    echo "Virhe: Tiedostoa $APP_DIR/.env ei löydy."
    exit 1
fi

# 4. Varmistetaan, että backup-kansio on olemassa
mkdir -p $BACKUP_DIR

# 5. Suoritetaan mysqldump
# Käytetään .env-tiedostosta ladattuja muuttujia $MYSQL_ROOT_PASSWORD ja $DB_NAME
docker exec $DB_CONTAINER mysqldump -u root -p"$MYSQL_ROOT_PASSWORD" "$DB_NAME" > "$BACKUP_DIR/backup_$TIMESTAMP.sql"

# 6. Tarkistetaan onnistuiko varmuuskopiointi (koko > 0)
if [ -s "$BACKUP_DIR/backup_$TIMESTAMP.sql" ]; then
    echo "Varmuuskopiointi onnistui: backup_$TIMESTAMP.sql"
    # 7. Poistetaan yli 7 päivää vanhat varmuuskopiot
    find $BACKUP_DIR -type f -name "*.sql" -mtime +7 -delete
else
    echo "Virhe: Varmuuskopiotiedosto on tyhjä. Tarkista tietokannan tila."
    rm "$BACKUP_DIR/backup_$TIMESTAMP.sql"
    exit 1
fi