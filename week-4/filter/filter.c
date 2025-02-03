#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[])
{
    // define allowable filters
    char *filters = "begr";

    // get filter flag and check validity
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        printf("Invalid filter.\n");
        return 1;
    }

    // ensure only one filter
    if (getopt(argc, argv, filters) != -1)
    {
        printf("Only one filter allowed.\n");
        return 2;
    }

    // ensure proper usage
    if (argc != optind + 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }

    // remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    // read infile's bitmapfileheader
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's bitmapinfoheader
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed bmp 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }

    // get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }

    // filter image
    switch (filter)
    {
        // blur
        case 'b':
            blur(height, width, image);
            break;

        // edges
        case 'e':
            edges(height, width, image);
            break;

        // grayscale
        case 'g':
            grayscale(height, width, image);
            break;

        // reflect
        case 'r':
            reflect(height, width, image);
            break;
    }

    // write outfile's bitmapfileheader
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's bitmapinfoheader
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // free memory for image
    free(image);

    // close files
    fclose(inptr);
    fclose(outptr);
    return 0;
}
