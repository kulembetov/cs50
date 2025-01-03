#include <cs50.h>
#include <stdio.h>
#include <string.h>

// max number of candidates
#define MAX 9

// candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// array of candidates
candidate candidates[MAX];

// number of candidates
int candidate_count;

// function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("usage: plurality [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("number of voters: ");

    // loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("vote: ");

        // check for invalid vote
        if (!vote(name))
        {
            printf("invalid vote.\n");
        }
    }

    print_winner(); // print the winner
}

// update vote totals given a new vote
bool vote(string name)
{
    // iterate over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // check if the input name matches a candidate's name
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++; // increment vote count
            return true;
        }
    }
    return false; // return false if no matching candidate is found
}

// print the winner (or winners) of the election
void print_winner(void)
{
    int max_votes = 0;

    // find the max vote count
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max_votes)
        {
            max_votes = candidates[i].votes;
        }
    }

    // print the name of the winner(s)
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}
