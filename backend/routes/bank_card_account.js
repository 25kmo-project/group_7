const express = require('express');
const router = express.Router();
const bankAccountModel = require('../models/bank_card_account_model');

// Get all bank card accounts
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
// Add a new bank card account
router.post('/', function(req, res) {
    bankAccountModel.add(req.body, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result);
        }
    });
});
// Get a single bank card account by card ID
router.get('/:id', function(req, res) {
    bankAccountModel.getOne(req.params.id, function(err, result) {
        if (err) { 
            res.send(err);
        } else {
            res.json(result);
        }
    }
    );
});
module.exports = router;