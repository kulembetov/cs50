#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open input file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open output file.\n");
        fclose(input);
        return 1;
    }

    float factor = atof(argv[3]);

    // copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);

    // process audio samples
    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        int32_t temp = buffer * factor;
        if (temp > INT16_MAX)
            buffer = INT16_MAX;
        else if (temp < INT16_MIN)
            buffer = INT16_MIN;
        else
            buffer = (int16_t) temp;

        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    fclose(input);
    fclose(output);

    return 0;
}
