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

router.post('/', function(req, res){
    bankCardModel.add(req.body, function(err, result){
        if(err){
            res.send(err);
        } else {
            res.json(result);
        }
    });
});

router.put('/:id', function(req, res){
    bankCardModel.update(req.params.id, req.body, function(err, result){
        if(err){
            res.send(err);
        } else {
            res.json(result);
        }   
    });
});
router.delete('/:id', function(req, res){
    bankCardModel.delete(req.params.id, function(err, result){
        if(err){
            res.send(err);
        } else {
            res.json(result);
        }
    });
});


        


module.exports = router;
