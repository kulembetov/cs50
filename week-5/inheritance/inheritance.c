// simulate genetic inheritance of blood type
#define _DEFAULT_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// person struct with parents and blood type alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // seed random number generator
    srandom(time(0));

    // create and print family tree, then free memory
    person *p = create_family(GENERATIONS);
    print_family(p, 0);
    free_family(p);
}

// create new person and their ancestors recursively
person *create_family(int generations)
{
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        return NULL;
    }

    // create parents and inherit alleles if not first generation
    if (generations > 1)
    {
        // recursively create parents
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        // inherit random allele from each parent
        new_person->alleles[0] = parent0->alleles[random() % 2];
        new_person->alleles[1] = parent1->alleles[random() % 2];
    }
    else
    {
        // first generation has no parents and random alleles
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;
}

// recursively free family tree memory
void free_family(person *p)
{
    if (p == NULL)
    {
        return;
    }

    free_family(p->parents[0]);
    free_family(p->parents[1]);
    free(p);
}

// print family tree with proper indentation
void print_family(person *p, int generation)
{
    if (p == NULL)
    {
        return;
    }

    // indent based on generation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // print person with appropriate generation label
    if (generation == 0)
    {
        printf("child (generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("parent (generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("great-");
        }
        printf("grandparent (generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // recursively print parents
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// return random blood type allele (A, B, or O)
char random_allele()
{
    int r = random() % 3;
    if (r == 0)
    {
        return 'a';
    }
    else if (r == 1)
    {
        return 'b';
    }
    else
    {
        return 'o';
    }
}