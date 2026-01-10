const express = require('express');
const router = express.Router();
const bankLogModel = require('../models/bank_log_model');

router.get('/',
    function(request, response){
        bankLogModel.getAll(function(err, dbResult){
            if (err){
                response.json(err);
            }else{
                response.json(dbResult);
            }
        })
    }
);

router.get('/:id',
    function(request,response){
        bankLogModel.getOne(request.params.log, function(err, dbResult){
            if (err){
                response.json(err);
            }else{
                response.json(dbResult);
            }
        })
    }
);

router.post('/',
    function(request, response){
        bankLogModel.add(request.body, function(err, dbResult){
            if (err){
                response.json(err);
            }else{
                response.json(dbResult);
            }
        })
    }
);

router.put('/:id',
    function(request, response){
        bankLogModel.put(request.params.id, function(err, dbResult){
            if (err){
                response.json(err);
            }else{
                response.json(dbResult);
            }
        })
    }
);

router.delete('/:id',
    function(request, response){
        bankLogModel.delete(request.params.id, function(err, dbResult){
            if (err){
                response.json(err);
            }else{
                response.json(dbResult);
            }
        })
    }
);

module.exports = router;
