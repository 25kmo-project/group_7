const db = require('../database');
//const bcrypt = require('bcryptjs');

const account = {
    getAll:function(callback) {
        return db.query('SELECT * FROM account', callback);
       // db.query('SELECT * FROM bank_accounts', callback);
    },
    getOne:function(account_id, callback) {
        return db.query('SELECT * FROM account WHERE account_id = ?', [account_id], callback);
    },
    add:function(account, callback) {

        if (account.balance == null) { 
        account.balance = 0; }

        if (account.account_type === 'credit' && account.credit_limit == null) { //Asettaa credit_limitin oletusarvoksi 5000€
            account.credit_limit = 5000;                                        //jos credit account eikä credit_limit määritetty
        }
        return db.query('INSERT INTO account (account_type, account_number, balance, credit_limit, user_id) VALUES (?, ?, ?, ?, ?)',
            [account.account_type, account.account_number, account.balance, account.credit_limit, account.user_id], callback);
    },
    update:function(account_id, account, callback) {   
        return db.query('UPDATE account SET account_type = ?, account_number = ?, balance = ?, credit_limit = ? WHERE account_id = ?',
            [account.account_type, account.account_number, account.balance, account.credit_limit, account_id], callback);
    },
    delete:function(account_id, callback) {
        return db.query('DELETE FROM account WHERE account_id = ?', [account_id], callback);
    },  
}

module.exports = account;