const express = require('express');
const router = express.Router();
const bank_user = require('../models/bank_user_model'); 
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');

// Kirjautumisreitti
router.post('/', function(request, response) {

    // Tarkistetaan että card_number ja pin on annettu
    if (request.body.card_number && request.body.pin) {

        const card_number = request.body.card_number;
        const pin = request.body.pin;

        // Haetaan hashattu PIN card-taulun tietokannasta
        bank_user.check_password(card_number, function(err, result) {

            if (err) {
                return response.json(err.errno);
            }

            // Löytyikö ?
            if (result.length > 0) {

                // Verrataan syötettyä PIN-koodia hashattuun PIN:iin
                bcrypt.compare(pin, result[0].card_pin_hash, function(err, compareResult) {

                    if (compareResult) {
                        const token = generateAccessToken(result[0].user_id); //Käytetään korttiin liitettyä user_id:t
                        response.setHeader('Content-Type', 'application/json');
                        response.json({
                            success: true,
                            message: "Login OK",
                            user_id: result[0].user_id, // Palautetaan user_id
                            token: token
                        });
                    } else {
                        console.log("Väärä PIN");
                        response.json({ "message": "tunnus ja salasana eivät täsmää" });
                    }
                });

            } else {
                console.log("Käyttäjää ei ole");
                response.json({ "message": "tunnus ja salasana eivät täsmää" });
            }
        });

    } else {
        console.log("Kortin numero tai PIN puuttuu");
        response.json({ "message": "tunnus ja salasana eivät täsmää" });
    }
});


function generateAccessToken(user_id) {
    return jwt.sign({ user_id }, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;
