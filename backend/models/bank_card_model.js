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
        return db.query('INSERT INTO card (card_number, card_type, expiration_date, user_id) VALUES (?, ?, ?, ?)',
            [card.card_number, card.card_type, card.expiration_date, card.user_id], callback);
    },
    update:function(card_id, card, callback){   
        return db.query('UPDATE card SET card_number = ?, card_type = ?, expiration_date = ? WHERE card_id = ?',
            [card.card_number, card.card_type, card.expiration_date, card_id], callback);
    },
    delete:function(card_id, callback){
        return db.query('DELETE FROM card WHERE card_id = ?', [card_id], callback);
    },

}
module.exports = card;   