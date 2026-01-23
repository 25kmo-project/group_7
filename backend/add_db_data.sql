INSERT INTO `user` (first_name, last_name, address, pin_hash) 
VALUES ('Aku', 'Ankka', 'Paratiisitie 13', 'salasana'),
('Roope', 'Ankka', 'Rahasäiliö', 'salasana'),
('Hessu', 'Hopo', 'Paratiisitie 11', 'salasana'),
('Minni', 'Hiiri', 'Paratiisitie 14', 'salasana'),
('Iines', 'Ankka', 'Paratiisitie 13', 'salasana');
INSERT INTO `account` (account_type, account_number, balance, credit_limit, user_id) 
VALUES ('debit', 'FI12 3456', '3000', '0', '1'),
('credit', 'FI12 9999', '0', '5000', '1'),
('dual', 'FI12 8888', '1000', '2000', '1'),
('debit', 'FI98 7654', '50000', '0', '2'),
('credit', 'FI98 1111', '-500', '10000', '2'),
('debit', 'FI55 1234', '1500', '0', '3'),
('credit', 'FI55 4321', '0', '3000', '3'),
('debit', 'FI77 7777', '800', '0', '4'),
('debit', 'FI66 6666', '1200', '0', '5');
INSERT INTO `card` (card_number, card_type, expiration_date, card_pin_hash, log_in_attempts, user_id) 
VALUES 
('1111', 'debit', '2025-12-31', '1234', 3, 1),
('2222', 'credit', '2026-06-30', '1234', 3, 1),
('3333', 'dual', '2027-03-31', '1234', 3, 1),
('4444', 'debit', '2025-08-15', '1234', 3, 2),
('5555', 'credit', '2024-12-31', '1234', 2, 2),
('6666', 'debit', '2026-05-20', '1234', 3, 3),
('7777', 'credit', '2025-11-30', '1234', 3, 3),
('8888', 'debit', '2026-02-28', '1234', 3, 4),
('9999', 'debit', '2026-07-31', '1234', 3, 5);
INSERT INTO `card_account` (card_id, account_id) 
VALUES 
(1, 1),
(2, 2),
(3, 1),
(3, 2),
(4, 4),
(5, 5),
(6, 6),
(7, 7),
(8, 8),
(9, 9);

CALL withdraw(1, 100.00);
CALL withdraw(2, 200.00);
CALL withdraw(4, 500.00);
CALL withdraw(6, 75.00);
CALL withdraw(8, 50.00);
CALL withdraw(9, 30.00);
CALL transfer(1, 8, 150.00);
CALL transfer(4, 6, 300.00);
CALL transfer(6, 9, 100.00);
CALL transfer(8, 1, 50.00);
CALL withdraw(2, 150.00);
CALL withdraw(5, 1000.00);
CALL transfer(6, 8, 25.00);
CALL transfer(9, 6, 40.00);