const db = require('../database');
const bcrypt = require('bcryptjs');

const card = {
    getAll:function(callback){
        return db.query('SELECT * FROM card', callback);
    },
    getOne:function(card_id, callback){
        return db.query('SELECT * FROM card WHERE card_id = ?', [card_id], callback);
    },
    add:function(card,callback){
        const pin = card.pin;
        if (!/^\d{4}$/.test(pin)) {
            return callback(new Error('PIN pitää olla 4-numeroinen'), null);
        }

        bcrypt.hash(pin, 10, function(err, hash) {
            if (err) {
                return callback(err, null);
            }
            return db.query('INSERT INTO card (card_number, card_type, expiration_date, user_id, card_pin_hash) VALUES (?, ?, ?, ?, ?)',
            [card.card_number, card.card_type, card.expiration_date, card.user_id, hash], callback);
        });
    },
    update:function(card_id, card, callback){
        const pin = card.card_pin_hash;
        if (!/^\d{4}$/.test(pin)) {
            return callback(new Error('PIN pitää olla 4-numeroinen'), null);
        }
        bcrypt.hash(card.card_pin_hash, 10, function(err, hash) {
            if (err) {
                return callback(err, null);
            }
            return db.query('UPDATE card SET card_number = ?, card_type = ?, expiration_date = ?, card_pin_hash = ?, log_in_attempts = ? WHERE card_id = ?',
                [card.card_number, card.card_type, card.expiration_date, hash, card.log_in_attempts, card_id], callback);
        });
    },
    delete:function(card_id, callback){
        return db.query('DELETE FROM card WHERE card_id = ?', [card_id], callback);
    },
    check_pin:function(card_id, callback){
        return db.query('SELECT card_pin_hash FROM card WHERE card_id = ?', [card_id], callback);
    }

};
module.exports = card;   