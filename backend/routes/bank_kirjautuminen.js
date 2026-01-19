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
        bank_user.check_password_and_logins(card_number, function(err, result) {

            if (err) {
                return response.json(err.errno);
            }

            // Löytyikö ?
            if (result.length > 0) {

                const login_attempts = result[0].log_in_attempts;
                if (login_attempts <= 0) {
                    console.log("Kaikki yritykset käytetty");
                    return response.json({ "message": "Kirjautuminen estetty" });
                }
                
                // Verrataan syötettyä PIN-koodia hashattuun PIN:iin
                bcrypt.compare(pin, result[0].card_pin_hash, function(err, compareResult) {

                    if (compareResult) {
                        bank_user.reset_login_attempts(card_number, function(err, resetResult) {
                            if (err) {
                                return response.json(err.errno);
                            }
                        
                        const token = generateAccessToken(result[0].user_id); //Käytetään korttiin liitettyä user_id:t
                        response.setHeader('Content-Type', 'application/json');
                        response.json({
                            success: true,
                            message: "Login OK",
                            user_id: result[0].user_id, // Palautetaan user_id
                            card_type: result[0].card_type, // Palautetaan card_type
                            token: token
                        });
                    });
                    } else {
                        console.log("Väärä PIN");
                        //vähennetään login_attempts yhdellä ja muutetaan tietokantaan
                        const attempts = login_attempts - 1;
                        bank_user.update_logins(card_number, attempts, function(err, updateResult) {
                            if (err) {
                                return response.json({ "message": "tunnus ja salasana eivät täsmää" });
                            }
                            if (attempts <= 0) {
                                    return response.json({ "message": "Kortilla kirjautuminen estetty" });
                                }
                            if (attempts === 1) {
                                return response.json({ 
                                    "message": `Tunnus ja salasana eivät täsmää. 1 yritys jäljellä.`
                            });   
                            } else {
                                return response.json({ 
                                    "message": `Tunnus ja salasana eivät täsmää. ${attempts} yritystä jäljellä.`
                        });
                    }
                    
                });
            }
        });
    } else {
        console.log("Kortin numero tai PIN puuttuu");
        return response.json({ "message": "tunnus ja salasana eivät täsmää" });
    }
    });
}
});


function generateAccessToken(user_id) {
    return jwt.sign({ user_id }, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;
