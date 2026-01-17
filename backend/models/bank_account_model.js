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
        return db.query('INSERT INTO account (account_type, account_number, balance, credit_limit, user_id) VALUES (?, ?, ?, ?, ?)',
            [account.account_type, account.account_number, account.balance, account.credit_limit, account.user_id], callback);
    },
    update:function(account_id, account, callback) {   
        return db.query('UPDATE account SET account_type = ?, account_number = ?, balance = ?, credit_limit = ? WHERE account_id = ?',
            [account.account_type, account.account_number, account.balance, account.credit_limit, account_id], callback);
    },
    getByUserAndType: function(user_id, account_type, callback) {
    return db.query(
        'SELECT * FROM account WHERE user_id = ? AND account_type = ?',
        [user_id, account_type],
        callback
    );
},

    delete:function(account_id, callback) {
        return db.query('DELETE FROM account WHERE account_id = ?', [account_id], callback);
    },  
}

module.exports = account;