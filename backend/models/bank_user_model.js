const db = require('../database');
const bcrypt = require('bcryptjs');

const bank_user = {
    getAll:function(callback) {
        return db.query('SELECT * FROM user', callback);
    },
    getOne:function(user_id, callback) {
        return db.query('SELECT * FROM user WHERE user_id = ?', [user_id], callback);
    },
    add:function(user, callback) {
        bcrypt.hash(user.pin_hash, 10, function(err, hash) {
            return db.query('INSERT INTO user (first_name, last_name, address, pin_hash) VALUES (?, ?, ?, ?)',
                [user.first_name, user.last_name, user.address, hash], callback);
        });
    },
    update:function(user_id, user, callback) {
        bcrypt.hash(user.pin_hash, 10, function(err, hash) {
            return db.query('UPDATE user SET first_name = ?, last_name = ?, address = ?, pin_hash = ? WHERE user_id = ?',
                [user.first_name, user.last_name, user.address, hash, user_id], callback);
        });
    },
    delete:function(user_id, callback) {
        return db.query('DELETE FROM user WHERE user_id = ?', [user_id], callback);
    },
    check_password_and_logins:function(card_number, callback) {
    console.log("check_password() kutsuttu, card_number =", card_number);

    return db.query(
        'SELECT card_pin_hash, user_id, card_type, log_in_attempts FROM card WHERE card_number = ?',
        [card_number],
        function(err, result) {
            if (err) {
                console.log("SQL ERROR:", err);
                return callback(err);
            }

            console.log("SQL tulos:", result);
            return callback(null, result);
        }
    );
    },
    lock_card:function(card_number, callback) {
        let sql = `UPDATE card SET log_in_attempts = 0 WHERE card_number = ?`;
        db.query(sql, [card_number], function(err, result) {
            callback(err, result);
    });
    },
    update_logins:function(card_number, attempts, callback) {
        let sql = `UPDATE card SET log_in_attempts = ? WHERE card_number = ?`;
        db.query(sql, [attempts, card_number], function(err, result) {
            callback(err, result);
    });
    },
    reset_login_attempts:function(card_number, callback) {
    let sql = `UPDATE card 
               SET log_in_attempts = 3 
               WHERE card_number = ?`;
    db.query(sql, [card_number], function(err, result) {
            callback(null, result);
        });
    }
};


module.exports = bank_user;