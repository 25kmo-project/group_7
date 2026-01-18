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
// Get account by user_id AND account_type (credit/debit)
router.get('/:user_id/:account_type', function(req, res) {
    const user_id = req.params.user_id;
    const account_type = req.params.account_type;

    bankAccountModel.getByUserAndType(user_id, account_type, function(err, result) {
        if (err) {
            console.log("SQL ERROR:", err);
            return res.status(500).json({ error: 'Database error' });
        }

        if (result.length === 0) {
            return res.status(404).json({ error: 'Account not found' });
        }

        return res.json(result[0]);
    });
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
            res.json({ message: 'Account added successfully', userId: result.insertId });
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
            res.json({ message: 'User updated successfully', userId: result.insertId });
        }
    });
});
// Delete a bank account
router.delete('/:id', function(req, res) {
    bankAccountModel.delete(req.params.id, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json({ message: 'Account deleted successfully' });
        }
    });
});

module.exports = router;