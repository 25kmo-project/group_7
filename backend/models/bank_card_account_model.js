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
        return db.query('INSERT INTO card_account (card_id, account_id) VALUES (?, ?)',
            [card_account.card_id, card_account.account_id], callback);
    },
    delete:function(card_id, callback) {
        return db.query('DELETE FROM card_account WHERE card_id = ?', [card_id], callback);
    },
    
}

module.exports = bankCardAccount;