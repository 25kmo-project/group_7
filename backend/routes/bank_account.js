//console.log("bank_account.js loaded!");

const express = require('express');
const router = express.Router();
const bankAccountModel = require('../models/bank_account_model');
// Get all bank accounts
router.get('/', function(req, res,) {
    bankAccountModel.getAll(function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result);
        }
    }
    );
});
// Get a single bank account by ID
router.get('/:id', function(req, res) {
    bankAccountModel.getOne(req.params.id, function(err, result) {
        if (err) {
            res.send    (err);
        } else {
            res.json(result[0]);
        }
    }
    );
});

// Add a new bank account   
router.post('/', function(req, res) {
    bankAccountModel.add(req.body, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result);
        }
    });
});

// Update an existing bank account
router.put('/:id', function(req, res) {
    const updatedAccount = req.body;
    bankAccountModel.update(req.params.id, updatedAccount, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result);
        }
    });
});
// Delete a bank account
router.delete('/:id', function(req, res) {
    bankAccountModel.delete(req.params.id, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result);
        }
    });
});

module.exports = router;