const express = require('express');
const router = express.Router();
const bankCardModel = require('../models/bank_card_model');

router.get('/',function(req, res){
    bankCardModel.getAll(function(err, result){
        if(err){
            res.send(err);
        } else {
            res.json(result);
        }
    });
});

router.get('/:id', function(req, res){
    bankCardModel.getOne(req.params.id, function(err, result){
        if(err){
            res.send(err);
        } else {
            res.json(result[0]);
        }
    });
});

router.post('/', function(req, res) {
    bankCardModel.add(req.body, function(err, result) {
        if (err) {
            return res.status(400).json({ error: err.message }); // kun PIN ei ole 4-numeroinen
        }
        res.status(201).json({ message: 'Kortti lisätty onnistuneesti', id: result.insertId });
    });
});

router.put('/:id', function(req, res) {
    bankCardModel.update(req.params.id, req.body, function(err, result) {
        if (err) {
            return res.status(400).json({ error: err.message }); // kun PIN ei ole 4-numeroinen
        }
        if (result.affectedRows === 0) {
            return res.status(404).json({ error: 'Korttia ei löytynyt' });
        }
        res.json({ message: 'Kortti päivitetty' });
    });
});
router.delete('/:id', function(req, res){
    bankCardModel.delete(req.params.id, function(err, result){
        if(err){
            return res.status(500).json({ error: err.message });
        }
        if(result.affectedRows === 0){
            return res.status(404).json({ error: 'Korttia ei löytynyt' });
        }
            res.json({ message: 'Kortti poistettu' });
    });
});

module.exports = router;
