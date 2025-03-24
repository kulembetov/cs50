from enum import Enum


class CardType(Enum):
    VISA = "VISA"
    MASTERCARD = "MASTERCARD"
    AMEX = "AMEX"
    INVALID = "INVALID"


def main():
    # get card number from user with validation
    while True:
        try:
            number = int(input("Number: "))
            if number > 0:
                break
        except ValueError:
            print(CardType.INVALID.value)

    # check if valid card number and get type
    if validate_checksum(number):
        print(get_card_type(number).value)
    else:
        print(CardType.INVALID.value)


def validate_checksum(number):
    total = 0
    position = 0

    # implement luhn's algorithm
    while number > 0:
        digit = number % 10

        # multiply every other digit by 2 starting from second-to-last
        if position % 2 == 1:
            digit *= 2
            if digit > 9:
                # sum digits if product is greater than 9
                digit = digit // 10 + digit % 10
        total += digit

        number //= 10
        position += 1

    return total % 10 == 0


def get_card_type(number):
    num_str = str(number)
    length = len(num_str)

    # check visa first (starts with 4)
    if length in [13, 16] and num_str[0] == "4":
        return CardType.VISA

    # check mastercard (starts with 51-55)
    if length == 16 and num_str[:2] in ["51", "52", "53", "54", "55"]:
        return CardType.MASTERCARD

    # check amex (starts with 34 or 37)
    if length == 15 and num_str[:2] in ["34", "37"]:
        return CardType.AMEX

    # if no match found
    return CardType.INVALID


if __name__ == "__main__":
    main()
