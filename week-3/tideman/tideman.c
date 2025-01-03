#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

typedef struct
{
    int winner;
    int loser;
} pair;

string candidates[MAX];
int preferences[MAX][MAX];
bool locked[MAX][MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool creates_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("maximum number of candidates is %i\n", MAX);
        return 2;
    }

    // store each candidate's name
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // initialize the locked graph with all false (no edges)
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("number of voters: ");

    // collect votes from each voter
    for (int i = 0; i < voter_count; i++)
    {
        // stores the voter's preferences
        int ranks[candidate_count];

        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("rank %i: ", j + 1);
            if (!vote(j, name, ranks))
            {
                printf("invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // if the name matches, store the candidate's index at the specified rank
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// update preferences based on the voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // increment the preference for the higher-ranked candidate
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// add all pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// sort pairs by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            int strength_i = preferences[pairs[i].winner][pairs[i].loser];
            int strength_j = preferences[pairs[j].winner][pairs[j].loser];

            // swap if the second pair is stronger
            if (strength_i < strength_j)
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
}

// check if locking a pair would create a cycle
bool creates_cycle(int winner, int loser)
{
    if (loser == winner)
    {
        // if the loser points back to the winner, there is a cycle
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] && creates_cycle(winner, i))
        {
            // recursively check for cycles
            return true;
        }
    }
    return false;
}

// lock pairs into the candidate graph without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!creates_cycle(pairs[i].winner, pairs[i].loser))
        {
            // lock the pair if no cycle is created
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// print the winner
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool source = true;

        // check if the candidate has any incoming edges
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                // not a source if there is an incoming edge
                source = false;
                break;
            }
        }

        if (source)
        {
            // print the candidate with no incoming edges
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
