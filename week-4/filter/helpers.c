#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the average of the RGB values
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
}

// reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swap pixels horizontally
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE **temp = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++)
    {
        temp[i] = malloc(width * sizeof(RGBTRIPLE));
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;

            // iterate over 3x3 box
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di, nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumRed += image[ni][nj].rgbtRed;
                        sumGreen += image[ni][nj].rgbtGreen;
                        sumBlue += image[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // store the avg values in temp array
            temp[i][j].rgbtRed = round((float) sumRed / count);
            temp[i][j].rgbtGreen = round((float) sumGreen / count);
            temp[i][j].rgbtBlue = round((float) sumBlue / count);
        }
    }

    // copy temp values back into original image and free memory
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
        free(temp[i]);
    }
    free(temp);
}

// detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE **temp = malloc(height * sizeof(RGBTRIPLE *));
    for (int i = 0; i < height; i++)
    {
        temp[i] = malloc(width * sizeof(RGBTRIPLE));
    }

    // sobel operator kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRx = 0, sumGx = 0, sumBx = 0;
            int sumRy = 0, sumGy = 0, sumBy = 0;

            // iterate over 3x3 grid
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di, nj = j + dj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        int weightX = Gx[di + 1][dj + 1];
                        int weightY = Gy[di + 1][dj + 1];

                        sumRx += image[ni][nj].rgbtRed * weightX;
                        sumGx += image[ni][nj].rgbtGreen * weightX;
                        sumBx += image[ni][nj].rgbtBlue * weightX;

                        sumRy += image[ni][nj].rgbtRed * weightY;
                        sumGy += image[ni][nj].rgbtGreen * weightY;
                        sumBy += image[ni][nj].rgbtBlue * weightY;
                    }
                }
            }

            // compute final gradient magnitude, using fmin to prevent overflowing
            temp[i][j].rgbtRed = fmin(255, round(sqrt(sumRx * sumRx + sumRy * sumRy)));
            temp[i][j].rgbtGreen = fmin(255, round(sqrt(sumGx * sumGx + sumGy * sumGy)));
            temp[i][j].rgbtBlue = fmin(255, round(sqrt(sumBx * sumBx + sumBy * sumBy)));
        }
    }

    // copy temp values back to original image and free memory
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
        free(temp[i]);
    }
    free(temp);
}
