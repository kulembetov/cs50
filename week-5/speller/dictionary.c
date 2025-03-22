#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// node structure for hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// hash table size (prime number for better distribution)
const unsigned int N = 10007;

// tracks total words in dictionary
unsigned int word_count = 0;

// hash table array
node *table[N];

// checks if word exists in dictionary
bool check(const char *word)
{
    unsigned int index = hash(word);

    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// generates hash value for a word
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_value += tolower(word[i]);
    }
    return hash_value % N;
}

// loads dictionary into hash table
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // initialize hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // word buffer
    char word[LENGTH + 1];

    // read and store words
    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }

        strcpy(n->word, word);
        unsigned int index = hash(word);

        // add to front of linked list
        n->next = table[index];
        table[index] = n;
        word_count++;
    }

    fclose(file);
    return true;
}

// returns total word count
unsigned int size(void)
{
    return word_count;
}

// frees all allocated memory
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}