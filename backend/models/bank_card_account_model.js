const db = require('../database');
const { add } = require('./bank_account_model');

const bankCardAccount = {
    getAll:function(callback) {
        return db.query('SELECT * FROM card_account', callback);
    },
    getOne:function(card_id, callback) {
        return db.query('SELECT * FROM card_account WHERE card_id = ?', [card_id], callback);
    },
    add:function(card_account, callback) {
        // Get card_user_id
        db.query('SELECT user_id FROM card WHERE card_id = ?', [card_account.card_id],
            (err, card_result) => {
            if (err) {
                return callback(err, null);
            }
            if (card_result.length === 0) 
                return callback(new Error('korttia ei löydy'), null);


                // Get account_user_id
                db.query('SELECT user_id FROM account WHERE account_id = ?', [card_account.account_id],
                    (err, account_result) => {
                    if (err) {
                        return callback(err, null);
                    } if (account_result.length === 0)
                        return callback(new Error('tiliä ei löydy'), null);

                    if (card_result[0].user_id !== account_result[0].user_id) {
                        return callback(new Error('kortin ja tilin käyttäjät eivät täsmää'));
                    }

                return db.query('INSERT INTO card_account (card_id, account_id) VALUES (?, ?)',
                [card_account.card_id, card_account.account_id], callback);
    });
        });
    },
    delete:function(card_id, callback) {
        return db.query('DELETE FROM card_account WHERE card_id = ?', [card_id], callback);
    },
    
}

module.exports = bankCardAccount;