const express = require('express');
const router = express.Router();
const bankLogModel = require('../models/bank_log_model');

// Get all log entries
router.get('/', function (request, response) {
    bankLogModel.getAll(function (err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result);
        }
    });
});

// Get one log entry
router.get('/:id', function (request, response) {
    bankLogModel.getOne(request.params.id, function (err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result[0]);
        }
    });
});

// Add new log entry
router.post('/', function (request, response) {
    bankLogModel.add(request.body, function (err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result);
        }
    });
});

// Update log entry
router.put('/:id', function (request, response) {
    bankLogModel.update(request.body, request.params.id, function (err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result);
        }
    });
});

// Delete log entry
router.delete('/:id', function (request, response) {
    bankLogModel.delete(request.params.id, function (err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result);
        }
    });
});

router.post('/transfer', function (request, response) {
    const { first_account, second_account, amount } = request.body;

    bankLogModel.transfer(first_account, second_account, amount, function (err, result) {
        if (err) {
            response.json(err);
        } else {
            response.json(result);
        }
    });
});
router.post('/withdraw', function (request, response) { // Uusi reitti nostoa varten
    const { oma_account_id, maara_amount } = request.body; //Pyyntö sisältää nää kentät
    bankLogModel.withdraw(oma_account_id, maara_amount, function (err, result) { //Kutsuu withdraw funktiota modelissa
        console.log("MYSQL ERROR:", err); //Debuggausta varte nämä
        console.log("MYSQL RESULT:", result); //Debuggausta varte nämä
        if (err) {
            response.json(err);
        } else {
            response.json(result);
        }
    });
});
router.post('/deposit', function (request, response) {
    const {my_account_id, amount } = request.body;
    bankLogModel.deposit(my_account_id, amount, function (err, result) {
        if (err)    {
            console.log('MYSQL ERROR:', err);
            response.json(err);
        }else {
            response.json(result);
        }
    });
});
router.get('/paginated/:account_id', function (request, response) {
    const account_id = request.params.account_id;
    const limit = request.query.limit || 10;
    const offset = request-query.offset || 0;
    bankLogModel.getAccountLogs(account_id, limit, offset, function (err, result) {
        if (err){
            response.json(err);
        }else {
            response.json(result);
        }
    });
});

module.exports = router;
