var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var bank_user_Router= require('./routes/bank_user');
var account_Router= require('./routes/bank_account');
var card_Router= require('./routes/bank_card');
var kirjautuminen_router= require('./routes/bank_kirjautuminen');

var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);

console.log("bank_user_Router =", bank_user_Router);
console.log("account_Router =", account_Router);
console.log("card_Router =", card_Router);



app.use('/bank_user', bank_user_Router);
app.use('/bank_account', account_Router);
app.use('/bank_card', card_Router);
app.use('/bank_kirjautuminen', kirjautuminen_router);   



module.exports = app;
