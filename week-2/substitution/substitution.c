#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool is_valid_key(string key);
string encrypt(string plaintext, string key);

int main(int argc, string argv[])
{
    // ensure exactly one arg is provided
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // retrieve the key from the args
    string key = argv[1];

    // validate the key
    if (!is_valid_key(key))
    {
        printf("Key must contain 26 unique alphabetic characters.\n");
        return 1;
    }

    // get plaintext from the user
    string plaintext = get_string("plaintext: ");

    // encrypt the plaintext using the key
    string ciphertext = encrypt(plaintext, key);

    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

bool is_valid_key(string key)
{
    // check if the key length is exactly 26
    if (strlen(key) != 26)
    {
        return false;
    }

    // track which letters have been seen
    bool seen[26] = {false};

    for (int i = 0; i < 26; i++)
    {
        // ensure each character is a letter
        if (!isalpha(key[i]))
        {
            return false;
        }

        // normalize the letter to uppercase and calculate its position in the alphabet
        int index = toupper(key[i]) - 'A';

        // check for duplicate letters in the key
        if (seen[index])
        {
            return false;
        }

        // mark the letter as seen
        seen[index] = true;
    }

    return true;
}

string encrypt(string plaintext, string key)
{
    string ciphertext = plaintext;

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            // check if the current character is lowercase
            bool is_lower = islower(plaintext[i]);

            // calculate the alphabet index for the current letter
            int index = toupper(plaintext[i]) - 'A';

            // find the corresponding letter in the key, preserving case
            char encrypted_char = is_lower ? tolower(key[index]) : toupper(key[index]);

            // replace the plaintext letter with the encrypted letter
            ciphertext[i] = encrypted_char;
        }
        else
        {
            // non-alphabetic characters are copied as-is
            ciphertext[i] = plaintext[i];
        }
    }

    return ciphertext;
}
