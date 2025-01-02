#include <cs50.h>
#include <stdio.h>

// validate card number using luhn's algorithm
bool validate_checksum(long number);
// count the number of digits in the card number
int count_digits(long number);
// determine the card type based on length and prefix
void check_card_type(long number);

int main(void)
{
    // get the credit card number from the user
    long card_number = get_long("Number: ");

    // check if the card number passes the checksum
    if (validate_checksum(card_number))
    {
        // determine and print the card type
        check_card_type(card_number);
    }
    else
    {
        // print invalid if checksum fails
        printf("INVALID\n");
    }

    return 0;
}

bool validate_checksum(long number)
{
    int sum = 0;
    bool multiply = false;

    // apply luhn's algorithm
    while (number > 0)
    {
        int digit = number % 10;

        if (multiply)
        {
            // multiply every other digit by 2 and sum the digits of the product
            int product = digit * 2;
            sum += (product / 10) + (product % 10);
        }
        else
        {
            // add digits not multiplied by 2
            sum += digit;
        }

        // toggle the multiply flag
        multiply = !multiply;
        // move to the next digit
        number /= 10;
    }

    // return true if the total modulo 10 is 0
    return (sum % 10 == 0);
}

int count_digits(long number)
{
    int count = 0;

    // count the number of digits in the number
    while (number > 0)
    {
        number /= 10;
        count++;
    }

    return count;
}

void check_card_type(long number)
{
    // get the number of digits in the card number
    int digits = count_digits(number);

    // extract the first two digits of the card number
    long prefix = number;
    while (prefix >= 100)
    {
        prefix /= 10;
    }

    // check for american express
    if ((digits == 15) && (prefix == 34 || prefix == 37))
    {
        printf("AMEX\n");
    }
    // check for mastercard
    else if ((digits == 16) && (prefix >= 51 && prefix <= 55))
    {
        printf("MASTERCARD\n");
    }
    // check for visa
    else if ((digits == 13 || digits == 16) && (prefix / 10 == 4))
    {
        printf("VISA\n");
    }
    // if no match, print invalid
    else
    {
        printf("INVALID\n");
    }
}
