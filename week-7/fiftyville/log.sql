-- Keep a log of any SQL queries you execute as you solve the mystery.

-- First, let's examine the database structure
.tables

-- Let's look at the crime scene reports for July 28, 2024 on Humphrey Street
SELECT description FROM crime_scene_reports 
WHERE year = 2024 AND month = 7 AND day = 28 
AND street = 'Humphrey Street';

-- Let's check the interviews conducted on the day of the crime
SELECT name, transcript FROM interviews 
WHERE year = 2024 AND month = 7 AND day = 28;

-- Based on the interviews, let's check the bakery security logs
SELECT hour, minute, activity, license_plate 
FROM bakery_security_logs 
WHERE year = 2024 AND month = 7 AND day = 28 
AND hour = 10 AND minute BETWEEN 15 AND 25;

-- Let's check ATM transactions on Leggett Street
SELECT account_number, amount 
FROM atm_transactions 
WHERE year = 2024 AND month = 7 AND day = 28 
AND atm_location = 'Leggett Street' 
AND transaction_type = 'withdraw';

-- Let's get the names of people who made withdrawals
SELECT name 
FROM people 
JOIN bank_accounts ON people.id = bank_accounts.person_id 
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number 
WHERE atm_transactions.year = 2024 
AND atm_transactions.month = 7 
AND atm_transactions.day = 28 
AND atm_transactions.atm_location = 'Leggett Street' 
AND atm_transactions.transaction_type = 'withdraw';

-- Let's check phone calls made on the day of the crime
SELECT caller, receiver, duration 
FROM phone_calls 
WHERE year = 2024 AND month = 7 AND day = 28 
AND duration < 60;

-- Let's get the names of people who made short phone calls
SELECT p1.name AS caller, p2.name AS receiver 
FROM phone_calls pc 
JOIN people p1 ON pc.caller = p1.phone_number 
JOIN people p2 ON pc.receiver = p2.phone_number 
WHERE pc.year = 2024 AND pc.month = 7 AND pc.day = 28 
AND pc.duration < 60;

-- Let's check the earliest flight out of Fiftyville on July 29
SELECT f.id, f.origin_airport_id, f.destination_airport_id, f.hour, f.minute 
FROM flights f 
WHERE f.year = 2024 AND f.month = 7 AND f.day = 29 
ORDER BY f.hour, f.minute 
LIMIT 1;

-- Let's get the destination city of the earliest flight
SELECT city 
FROM airports 
WHERE id = (
    SELECT destination_airport_id 
    FROM flights 
    WHERE year = 2024 AND month = 7 AND day = 29 
    ORDER BY hour, minute 
    LIMIT 1
);

-- Let's get the passengers on the earliest flight
SELECT p.name 
FROM people p 
JOIN passengers ps ON p.passport_number = ps.passport_number 
JOIN flights f ON ps.flight_id = f.id 
WHERE f.id = (
    SELECT id 
    FROM flights 
    WHERE year = 2024 AND month = 7 AND day = 29 
    ORDER BY hour, minute 
    LIMIT 1
);

-- Let's find the thief by combining all the clues:
-- 1. Made a withdrawal at Leggett Street
-- 2. Made a short phone call
-- 3. Left the bakery between 10:15 and 10:25
-- 4. Was on the earliest flight out
SELECT p.name 
FROM people p 
JOIN bank_accounts ba ON p.id = ba.person_id 
JOIN atm_transactions atm ON ba.account_number = atm.account_number 
JOIN phone_calls pc ON p.phone_number = pc.caller 
JOIN bakery_security_logs bsl ON p.license_plate = bsl.license_plate 
JOIN passengers ps ON p.passport_number = ps.passport_number 
JOIN flights f ON ps.flight_id = f.id 
WHERE atm.year = 2024 AND atm.month = 7 AND atm.day = 28 
AND atm.atm_location = 'Leggett Street' 
AND atm.transaction_type = 'withdraw' 
AND pc.year = 2024 AND pc.month = 7 AND pc.day = 28 
AND pc.duration < 60 
AND bsl.year = 2024 AND bsl.month = 7 AND bsl.day = 28 
AND bsl.hour = 10 AND bsl.minute BETWEEN 15 AND 25 
AND f.id = (
    SELECT id 
    FROM flights 
    WHERE year = 2024 AND month = 7 AND day = 29 
    ORDER BY hour, minute 
    LIMIT 1
);

