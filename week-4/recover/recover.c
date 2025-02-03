#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// size of each block we read from the file
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // check if the user provided exactly one arg
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // open the forensic img file for reading
    // fopen returns a pointer to the file, so we use FILE* to store it
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // buffer to store each block
    uint8_t buffer[BLOCK_SIZE];
    // pointer to the jpeg file we are writing
    FILE *img = NULL;
    // to store filenames like "000.jpg"
    char filename[8];
    // keeps track of how many jpeg files we have created
    int file_count = 0;

    // read 512-byte blocks from the forensic image
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // check if the block starts a new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // if an image file is already open, close it first
            if (img != NULL)
            {
                fclose(img);
            }

            // create a new filename and open a new file for writing
            sprintf(filename, "%03d.jpg", file_count++);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create file.\n");
                fclose(card);
                return 1;
            }
        }

        // if we have an open jpeg file, write the current block to it
        if (img != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }

    // cleanup: close any open file handles
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);

    return 0;
}
