const db = require('../database');
console.log("UUSI MODEL LADATTU");

const bankCardAccount = {

    getAll: function(callback) {
        return db.query('SELECT * FROM card_account', callback);
    },

    getOne: function(card_id, callback) {
        return db.query('SELECT * FROM card_account WHERE card_id = ?', [card_id], callback);
    },

    add: function(card_account, callback) {

        // 1. Haetaan kortti
        db.query(
            'SELECT user_id, card_type FROM card WHERE card_id = ?',
            [card_account.card_id],
            (err, card_result) => {

                if (err) return callback(err);
                if (card_result.length === 0)
                    return callback(new Error('korttia ei löydy'));

                const cardUser = card_result[0].user_id;
                const cardType = card_result[0].card_type;

                // 2. Haetaan tili
                db.query(
                    'SELECT user_id, account_type FROM account WHERE account_id = ?',
                    [card_account.account_id],
                    (err, account_result) => {

                        if (err) return callback(err);
                        if (account_result.length === 0)
                            return callback(new Error('tiliä ei löydy'));

                        const accountUser = account_result[0].user_id;
                        const accountType = account_result[0].account_type;

                        // 3. Käyttäjien täsmäys
                        if (cardUser !== accountUser)
                            return callback(new Error('kortin ja tilin käyttäjät eivät täsmää'));

                        // 4. Haetaan kortin nykyiset tilit
                        db.query(
                            `SELECT a.account_type 
                             FROM card_account ca
                             JOIN account a ON ca.account_id = a.account_id
                             WHERE ca.card_id = ?`,
                            [card_account.card_id],
                            (err, linkedRows) => {

                                if (err) return callback(err);

                                const hasDebit = linkedRows.some(r => r.account_type === 'debit');
                                const hasCredit = linkedRows.some(r => r.account_type === 'credit');

                                // 5. Säännöt
                                if (cardType === 'debit') {
                                    if (accountType !== 'debit')
                                        return callback(new Error('debit-korttiin voi liittää vain debit-tilin'));
                                    if (hasDebit)
                                        return callback(new Error('debit-kortilla voi olla vain yksi tili'));
                                }

                                if (cardType === 'credit') {
                                    if (accountType !== 'credit')
                                        return callback(new Error('credit-korttiin voi liittää vain credit-tilin'));
                                    if (hasCredit)
                                        return callback(new Error('credit-kortilla voi olla vain yksi tili'));
                                }

                                if (cardType === 'dual') {
                                    if (accountType === 'debit' && hasDebit)
                                        return callback(new Error('dual-kortilla voi olla vain yksi debit-tili'));
                                    if (accountType === 'credit' && hasCredit)
                                        return callback(new Error('dual-kortilla voi olla vain yksi credit-tili'));
                                }

                                // 6. INSERT
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

    delete: function(card_id, callback) {
        return db.query('DELETE FROM card_account WHERE card_id = ?', [card_id], callback);
    },
};

module.exports = bankCardAccount;
