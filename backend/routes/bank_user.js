console.log("bank_user.js loaded!");

const express = require('express');
const router = express.Router();
const bankUserModel = require('../models/bank_user_model');
// Get all bank users
router.get('/', function(req, res,) {
    bankUserModel.getAll(function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result);
        }
    }
    );
});
// Get a single bank user by ID
router.get('/:id', function(req, res) {
    bankUserModel.getOne(req.params.id, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            res.json(result[0]);
        }
    });
});

// Add a new bank user
router.post('/', function(req, res) {
    const newUser = req.body;
    bankUserModel.add(req.body, function(err, result) {
        if (err) {
            res.send(err);
        } else {    
            res.json({ message: 'User added successfully', userId: result.insertId });
        }
    });
});
// Update an existing bank user
router.put('/:id', function(req, res) {
    const updatedUser = req.body;
    bankUserModel.update(req.params.id, updatedUser, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            console.log("PÄIVITETTY KÄYTTÄJÄ:", updatedUser);

            res.json(result);
        }
    });
});

// Delete a bank user
router.delete('/:id', function(req, res) {
    bankUserModel.delete(req.params.id, function(err, result) {
        if (err) {
            res.send(err);
        } else {
            console.log("POISTETTU KÄYTTÄJÄ ID:", req.params.id);
            res.json(result);
        }   
    });
});
module.exports = router;