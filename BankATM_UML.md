@startuml
title Bank ATM Backend 

skinparam linetype ortho
skinparam monochrome true



class User {
  +user_id
  +first_name
  +last_name
  +address
  +pin_hash
  --
  +CRUD operations
  +check_password_and_logins
  +update_logins
  +reset_login_attempts
}

class Account {
  +account_id
  +account_type  // debit/credit
  +account_number
  +balance
  +credit_limit
  +user_id
  --
  +CRUD operations
  +getByUserAndType
}

class Card {
  +card_id
  +card_number
  +card_type  // debit/credit/dual
  +expiration_date
  +user_id
  +card_pin_hash
  +log_in_attempts
  --
  +CRUD operations
  +check_pin
}

class CardAccount {
  +card_id
  +account_id
  --
  +CRUD operations
  +add (with validation)
}

class Log {
  +log_id
  +actions
  +amount
  +account_id
  +user_id
  +card_id
  +event_time
  --
  +CRUD operations
  +transfer
  +withdraw
  +deposit
  +getAccountLogs
}

' Suhteet: User omistaa Accountit ja Cardit, Cardit linkitetty Accountteihin, Log viittaa kaikkiin
User "1" -- "0..*" Account : omistaa
User "1" -- "0..*" Card : omistaa
Card "1" -- "0..*" Account : linkitetty { CardAccount } kautta
Log ..> Account : viittaa
Log ..> User : viittaa
Log ..> Card : viittaa

' Reitit tiivistetysti nuotteina
note "Reitit User, Account, Card, CardAccount, Log ja Login (JWT)" as RoutesNote

@enduml