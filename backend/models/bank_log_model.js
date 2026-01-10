const db = require('../database');
const bcrypt = require('bcryptjs');

const bank_log = {
    getAll: function (callback) {
        return db.query("SELECT * FROM log", callback);
    },
    getOne: function (log_id, callback) {
        return db.query("SELECT * FROM log WHERE log_id = ?",
            [log_id], callback);
    },
    add: function (log, callback) {
        return db.query("INSERT INTO log (actions, event_time, amount, account_id, user_id, card_id) VALUES (?,?,?,?,?,?)",
            [log.actions, log.event_time, log.amount, log.account_id, log.user_id, log.card_id], callback);
    },
    update: function (log, callback) {
        return db.query("UPDATE log SET actions = ?, event_time = ?, amount = ? WHERE log_id = ?",
            [log.actions, log.event_time, log.amount, log_id], callback);
    },
    delete: function (log_id, callback) {
        return db.query("DELETE * FROM log WHERE log_id = ?",
            [log_id], callback);
    },
}

module.exports = bank_log;