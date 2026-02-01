const db = require('../database');

const bank_log = {
    getAll: function (callback) {
        return db.query("SELECT log_id, actions, amount, account_id, user_id, card_id, DATE_FORMAT(event_time, '%d.%m.%Y %H:%i:%s') AS event_time FROM log", callback);
    },
    getOne: function (log_id, callback) {
        return db.query("SELECT log_id, actions, amount, account_id, user_id, card_id, DATE_FORMAT(event_time, '%d.%m.%Y %H:%i:%s') AS event_time FROM log WHERE log_id = ?",
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
    transfer: function (first_account_id, second_account_id, amount, callback) {
        db.query("CALL transfer(?, ?, ?)",
            [first_account_id, second_account_id, amount],
            callback);
    },
    withdraw: function (oma_account_id,maara_amount, callback) { //Rahaa otetaan tililtä
        db.query("CALL withdraw(?, ?)", //Kutsutaan tietokannan withdraw -proseduuria
            [oma_account_id, maara_amount], //Annetaan proseduurille tilin id ja määrä
            callback); //Suoritetaan callback funktio
    },
    deposit: function (my_account_id, amount, callback) {
        db.query("CALL deposit(?, ?)",
            [my_account_id, amount],
            callback);
    },
    getAccountLogs: function (account_id, limit, offset, callback) {
        db.query("SELECT * FROM log WHERE account_id = ? ORDER BY date DESC LIMIT ? OFFSET ?",
            [account_id, parseInt(limit), parseInt(offset)],
            callback);
    }


};
module.exports = bank_log;