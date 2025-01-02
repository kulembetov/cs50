#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// function prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // get input text from the user
    string text = get_string("Text: ");

    // count letters, words, and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // calculate L (letters per 100 words) and S (sentences per 100 words)
    float L = (letters / (float) words) * 100;
    float S = (sentences / (float) words) * 100;

    // calculate coleman-liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;

    // print the grade level
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", (int) round(index));
    }

    return 0;
}

// count the number of letters in the text
int count_letters(string text)
{
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }

    return count;
}

// count the number of words in the text
int count_words(string text)
{
    int count = 1; // because words are separated by spaces

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i]))
        {
            count++;
        }
    }

    return count;
}

// count the number of sentences in the text
int count_sentences(string text)
{
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}
