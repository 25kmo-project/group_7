const express = require('express');
const router = express.Router();
const bankAccountModel = require('../models/bank_card_account_model');

// Get all bank card accounts
router.get('/', function(req, res,) {
    bankAccountModel.getAll(function(err, result) {
        if (err) {
            return res.status(400).send(err.message);
        } else {
            res.json(result);
        }   
    }
    );
});
// Add a new bank card account
router.post('/', function(req, res) {
    bankAccountModel.add(req.body, function(err, result) {
        if (err) {
            return res.status(400).send(err.message);
        } else {
            res.json(result);
        }
    });
});
// Get a single bank card accounts by card ID
router.get('/:id', function(req, res) {
    bankAccountModel.getOne(req.params.id, function(err, result) {
        if (err) { 
            return res.status(400).send(err.message);
        } else {
            res.json(result);
        }
    })
});

// Delete a bank card account
router.delete('/:id', function(req, res) {
    bankAccountModel.delete(req.params.id, function(err, result) {
        if (err) {
            return res.status(400).send(err.message);
        } else {
            res.json(result);
        }
    });
});
module.exports = router;