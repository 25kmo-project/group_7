const express = require('express');
const router = express.Router();
const bank_user = require('../models/bank_user_model'); 
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');

// Kirjautumisreitti
router.post('/', function(request, response) {

    // Tarkistetaan että user_id ja pin on annettu
    if (request.body.user_id && request.body.pin) {

        const user_id = request.body.user_id;
        const pin = request.body.pin;

        // Haetaan hashattu PIN tietokannasta
        bank_user.check_password(user_id, function(err, result) {

            if (err) {
                return response.json(err.errno);
            }

            // Löytyikö käyttäjä?
            if (result.length > 0) {

                // Verrataan syötettyä PIN-koodia hashattuun PIN:iin
                bcrypt.compare(pin, result[0].pin_hash, function(err, compareResult) {

                    const token = generateAccessToken(user_id);

                    if (compareResult) {
                        response.setHeader('Content-Type', 'application/json');
                        response.json({
                            success: true,
                            message: "Login OK",
                            user_id: user_id,
                            token: token
                        });
                    } else {
                        console.log("Väärä PIN");
                        response.json({ "message": "tunnus ja salasana eivät täsmää" });
                    }
                });

            } else {
                console.log("Käyttäjää ei ole");
                response.json({ "message": "Tunnus ja salasana eivät täsmää" });
            }
        });

    } else {
        console.log("user_id tai pin puuttuu");
        response.json({ "message": "Tunnus ja salasana eivät täsmää" });
    }
});


function generateAccessToken(user_id) {
    return jwt.sign({ user_id }, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;
