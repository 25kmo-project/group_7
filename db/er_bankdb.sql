-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema bank_db
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema bank_db
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `bank_db` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci ;
USE `bank_db` ;

-- -----------------------------------------------------
-- Table `bank_db`.`user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `bank_db`.`user` (
  `user_id` INT NOT NULL AUTO_INCREMENT,
  `first_name` VARCHAR(45) NOT NULL,
  `last_name` VARCHAR(45) NOT NULL,
  `address` VARCHAR(45) NOT NULL,
  `pin_hash` VARCHAR(255) NOT NULL,
  PRIMARY KEY (`user_id`))
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `bank_db`.`account`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `bank_db`.`account` (
  `account_id` INT NOT NULL AUTO_INCREMENT,
  `account_type` ENUM('debit', 'credit', 'dual') NOT NULL,
  `account_number` VARCHAR(45) NOT NULL,
  `balance` DECIMAL(12,2) NOT NULL DEFAULT '0.00',
  `credit_limit` DECIMAL(12,2) NOT NULL DEFAULT '0.00',
  `user_id` INT NOT NULL,
  PRIMARY KEY (`account_id`),
  UNIQUE INDEX `account_number` (`account_number` ASC) VISIBLE,
  INDEX `user_id` (`user_id` ASC) VISIBLE,
  CONSTRAINT `account_ibfk_1`
    FOREIGN KEY (`user_id`)
    REFERENCES `bank_db`.`user` (`user_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
AUTO_INCREMENT = 2
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `bank_db`.`card`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `bank_db`.`card` (
  `card_id` INT NOT NULL AUTO_INCREMENT,
  `card_number` VARCHAR(45) NOT NULL,
  `card_type` ENUM('debit', 'credit', 'dual') NOT NULL,
  `expiration_date` DATE NOT NULL,
  `card_pin_hash` VARCHAR(255) NOT NULL,
  `log_in_attempts` INT NOT NULL DEFAULT 3,
  `user_id` INT NOT NULL,
  PRIMARY KEY (`card_id`),
  UNIQUE INDEX `card_number` (`card_number` ASC) VISIBLE,
  INDEX `user_id` (`user_id` ASC) VISIBLE,
  CONSTRAINT `card_ibfk_1`
    FOREIGN KEY (`user_id`)
    REFERENCES `bank_db`.`user` (`user_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `bank_db`.`card_account`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `bank_db`.`card_account` (
  `card_id` INT NOT NULL,
  `account_id` INT NOT NULL,
  PRIMARY KEY (`card_id`, `account_id`),
  INDEX `account_id` (`account_id` ASC) VISIBLE,
  CONSTRAINT `card_account_ibfk_1`
    FOREIGN KEY (`card_id`)
    REFERENCES `bank_db`.`card` (`card_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `card_account_ibfk_2`
    FOREIGN KEY (`account_id`)
    REFERENCES `bank_db`.`account` (`account_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `bank_db`.`log`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `bank_db`.`log` (
  `log_id` INT NOT NULL AUTO_INCREMENT,
  `actions` ENUM('withdrawal', 'transfer', 'deposit') NOT NULL,
  `event_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `amount` DECIMAL(12,2) NOT NULL,
  `card_id` INT NULL DEFAULT NULL,
  `account_id` INT NOT NULL,
  `user_id` INT NULL,
  PRIMARY KEY (`log_id`),
  INDEX `account_id` (`account_id` ASC) VISIBLE,
  INDEX `user_id` (`user_id` ASC) VISIBLE,
  INDEX `card_id` (`card_id` ASC) VISIBLE,
  CONSTRAINT `log_ibfk_2`
    FOREIGN KEY (`account_id`)
    REFERENCES `bank_db`.`account` (`account_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `log_ibfk_3`
    FOREIGN KEY (`user_id`)
    REFERENCES `bank_db`.`user` (`user_id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `log_ibfk_4`
    FOREIGN KEY (`card_id`)
    REFERENCES `bank_db`.`card` (`card_id`)
    ON DELETE SET NULL
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;

USE `bank_db` ;

-- -----------------------------------------------------
-- procedure transfer
-- -----------------------------------------------------

DELIMITER $$
USE `bank_db`$$
CREATE PROCEDURE `transfer` (IN first_account INT, IN second_account INT, IN amount DECIMAL(12,2) )
BEGIN
DECLARE test1,test2 INT DEFAULT 0;

IF amount <= 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Amount must be positive';
    END IF;
    
  START TRANSACTION;
  
  UPDATE account SET balance=balance-amount WHERE account_id=first_account AND balance >= amount;
  SET test1=ROW_COUNT();
  
  UPDATE account SET balance=balance+amount WHERE account_id=second_account;
  SET test2=ROW_COUNT();
  
    IF (test1 > 0 AND test2 >0) THEN
      INSERT INTO log(account_id,actions,amount,event_time) VALUES(first_account,'withdrawal',amount,NOW());
      INSERT INTO log(account_id,actions,amount,event_time) VALUES(second_account,'deposit',amount,NOW());
      COMMIT;
    ELSE
      ROLLBACK;
  END IF;
  
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure withdraw
-- -----------------------------------------------------

DELIMITER $$
USE `bank_db`$$
CREATE PROCEDURE withdraw (
    IN oma_account_id INT,
    IN maara_amount DECIMAL(12,2)
)
BEGIN

    DECLARE tilin_balance DECIMAL(12,2);
    DECLARE tilin_type ENUM('debit', 'credit');
    DECLARE tilin_credit_limit DECIMAL(12,2);

	IF maara_amount <= 0 THEN
			SIGNAL SQLSTATE '45000'
			SET MESSAGE_TEXT = 'Amount must be positive';
		END IF;

    START TRANSACTION;


    SELECT balance, account_type, credit_limit
    INTO tilin_balance, tilin_type, tilin_credit_limit
    FROM account
    WHERE account_id = oma_account_id
    FOR UPDATE;


    IF tilin_balance IS NULL THEN
        ROLLBACK;
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Tiliä ei löydy';
    END IF;


    IF tilin_type = 'debit' AND (tilin_balance - maara_amount) < 0 THEN
        ROLLBACK;
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Debit-tili ei voi mennä miinukselle';
    END IF;


    IF tilin_type = 'credit' AND (tilin_balance - maara_amount) < -tilin_credit_limit THEN
        ROLLBACK;
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Luottoraja ylittyy';
    END IF;


    UPDATE account
    SET balance = balance - maara_amount
    WHERE account_id = oma_account_id;


    INSERT INTO log(account_id, actions, amount, event_time)
    VALUES(oma_account_id, 'withdrawal', maara_amount, NOW());


    COMMIT;
END$$

DELIMITER ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
