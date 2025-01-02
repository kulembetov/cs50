#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // prompt user for height between 1 and 8
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // loop through each row
    for (int i = 0; i < height; i++)
    {
        // print spaces for left pyramid
        for (int j = 0; j < height - i - 1; j++)
        {
            printf(" ");
        }

        // print hashes for left pyramid
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // print gap
        printf("  ");

        // print hashes for right pyramid
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // move to the next row
        printf("\n");
    }

    return 0;
}
