const db = require('../database');
console.log("UUSI MODEL LADATTU"); //Debuggausta varten

const bankCardAccount = {

    // Hakee kaikki kortti–tili -liitokset
    getAll: function(callback) {
        return db.query('SELECT * FROM card_account', callback);
    },

    // Hakee yhden kortin kaikki tilit
    getOne: function(card_id, callback) {
        return db.query('SELECT * FROM card_account WHERE card_id = ?', [card_id], callback);
    },

    // Lisää uuden kortti–tili -liitoksen
    add: function(card_account, callback) {

        
        // 1. Haetaan kortin tiedot (omistaja + korttityyppi)
        
        db.query(
            'SELECT user_id, card_type FROM card WHERE card_id = ?',
            [card_account.card_id],
            (err, card_result) => {

                if (err) return callback(err);
                if (card_result.length === 0)
                    return callback(new Error('korttia ei löydy'));

                const cardUser = card_result[0].user_id;     // Kortin omistaja
                const cardType = card_result[0].card_type;   // debit / credit / dual

                
                // 2. Haetaan tilin tiedot (omistaja + tilityyppi)
                
                db.query(
                    'SELECT user_id, account_type FROM account WHERE account_id = ?',
                    [card_account.account_id],
                    (err, account_result) => {

                        if (err) return callback(err);
                        if (account_result.length === 0)
                            return callback(new Error('tiliä ei löydy'));

                        const accountUser = account_result[0].user_id;     // Tilin omistaja
                        const accountType = account_result[0].account_type; // debit / credit

                        
                        // 3. Varmistetaan että kortti ja tili kuuluvat samalle käyttäjälle
                        
                        if (cardUser !== accountUser)
                            return callback(new Error('kortin ja tilin käyttäjät eivät täsmää'));

                        
                        // 4. Haetaan kaikki tilit, jotka ovat jo liitettynä tähän korttiin ja tarkistaa säännöt
                        
                        
                        db.query(
                            `SELECT a.account_type 
                             FROM card_account ca
                             JOIN account a ON ca.account_id = a.account_id
                             WHERE ca.card_id = ?`,
                            [card_account.card_id],
                            (err, linkedRows) => {

                                if (err) return callback(err);

                                // Onko kortilla jo debit-tili?
                                const hasDebit = linkedRows.some(r => r.account_type === 'debit');

                                // Onko kortilla jo credit-tili?
                                const hasCredit = linkedRows.some(r => r.account_type === 'credit');

                                
                                // 5. Korttityypin logiikka
                                

                                
                                // DEBIT-KORTIN SÄÄNNÖT
                                
                                // - voi liittää vain debit-tilin
                                // - voi olla vain yksi tili
                                if (cardType === 'debit') {
                                    if (accountType !== 'debit')
                                        return callback(new Error('debit-korttiin voi liittää vain debit-tilin'));
                                    if (hasDebit)
                                        return callback(new Error('debit-kortilla voi olla vain yksi tili'));
                                }

                                
                                // CREDIT-KORTIN SÄÄNNÖT
                               
                                // - voi liittää vain credit-tilin
                                // - voi olla vain yksi tili
                                if (cardType === 'credit') {
                                    if (accountType !== 'credit')
                                        return callback(new Error('credit-korttiin voi liittää vain credit-tilin'));
                                    if (hasCredit)
                                        return callback(new Error('credit-kortilla voi olla vain yksi tili'));
                                }

                                
                                // DUAL-KORTIN SÄÄNNÖT
                                
                                // - voi liittää yhden debit-tilin
                                // - voi liittää yhden credit-tilin
                                // - ei kahta debit-tiliä
                                // - ei kahta credit-tiliä
                                if (cardType === 'dual') {
                                    if (accountType === 'debit' && hasDebit)
                                        return callback(new Error('dual-kortilla voi olla vain yksi debit-tili'));
                                    if (accountType === 'credit' && hasCredit)
                                        return callback(new Error('dual-kortilla voi olla vain yksi credit-tili'));
                                }

                                
                                // 6. Kaikki tarkistukset läpäisty → tehdään liitos
                                
                                return db.query(
                                    'INSERT INTO card_account (card_id, account_id) VALUES (?, ?)',
                                    [card_account.card_id, card_account.account_id],
                                    callback
                                );
                            }
                        );
                    }
                );
            }
        );
    },

    // Poistaa kaikki liitokset tietylle kortille
    delete: function(card_id, callback) {
        return db.query('DELETE FROM card_account WHERE card_id = ?', [card_id], callback);
    },
};

module.exports = bankCardAccount;
