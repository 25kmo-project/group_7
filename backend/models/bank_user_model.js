const db = require('../database');
const bcrypt = require('bcryptjs');

const student = {
    getAll:function(callback) {
        return db.query('SELECT * FROM user', callback);
    },
    getOne:function(user_id, callback) {
        return db.query('SELECT * FROM user WHERE user_id = ?', [user_id], callback);
    },
    add:function(user, callback) {
        bcrypt.hash(user.pin_hash, 10, function(err, hash) {
            return db.query('INSERT INTO user (first_name, last_name, address, pin_hash, balance) VALUES (?, ?, ?, ?,)',
                [user.first_name, user.last_name, user.address, hash,], callback);
        });
    },
    update:function(user, user_id, callback) {
        bcrypt.hash(user.pin_hash, 10, function(err, hash) {
            return db.query('UPDATE user SET first_name = ?, last_name = ?, address = ?, pin_hash = ? WHERE user_id = ?',
                [user.first_name, user.last_name, user.address, hash, user_id], callback);
        });
    },
    delete:function(user_id, callback) {
        return db.query('DELETE FROM user WHERE user_id = ?', [user_id], callback);
    }
}

module.exports = student;