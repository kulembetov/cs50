#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// to compute the score of a word
int compute_score(string word);

int main(void)
{
    // get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // compute scores for both players' words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // determine and print the winner based on scores
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// to compute the score of a word
int compute_score(string word)
{
    int score = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // check if the character is a letter
        if (isalpha(word[i]))
        {
            // convert the letter to uppercase
            // subtract 'A' to determine the letter's position in the alphabet
            // add the corresponding point value to the total score
            int index = toupper(word[i]) - 'A';
            score += POINTS[index];
        }
    }

    return score;
}
