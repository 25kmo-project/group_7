const db = require('../database');

const bank_log = {
    getAll: function (callback) {
        return db.query("SELECT * FROM log", callback);
    },
    getOne: function (log_id, callback) {
        return db.query("SELECT * FROM log WHERE log_id = ?",
            [log_id], callback);
    },
    add: function (log, callback) {
        return db.query("INSERT INTO log (actions, amount, account_id, user_id, card_id) VALUES (?,?,?,?,?)",
            [log.actions, log.amount, log.account_id, log.user_id, log.card_id], callback);
    },
    update: function (log, log_id, callback) {
        console.log(log.actions, typeof log.actions);
        console.log(log.amount, typeof log.amount);
        console.log(log_id, typeof log_id);

        return db.query("UPDATE log SET actions = ?, amount = ? WHERE log_id = ?",
            [log.actions, log.amount, log_id], callback);
    },
    delete: function (log_id, callback) {
        return db.query("DELETE FROM log WHERE log_id = ?",
            [log_id], callback);
    },
}

module.exports = bank_log;