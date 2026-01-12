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
        bcrypt.hash(card.card_pin_hash, 10, function(err, hash) {
        return db.query('INSERT INTO card (card_number, card_type, expiration_date, user_id, card_pin_hash) VALUES (?, ?, ?, ?, ?)',
            [card.card_number, card.card_type, card.expiration_date, card.user_id, hash], callback);
    });
    },
    update:function(card_id, card, callback){
        bcrypt.hash(card.card_pin_hash, 10, function(err, hash) {
            return db.query('UPDATE card SET card_number = ?, card_type = ?, expiration_date = ?, card_pin_hash = ? WHERE card_id = ?',
                [card.card_number, card.card_type, card.expiration_date, hash, card_id], callback);
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