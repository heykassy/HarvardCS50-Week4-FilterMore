#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Loop around the pixels, get the average of all three colors in each pixel and set each to the average number obtained
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Loop around the pixels in each row. Set two variables, one to start on the far left and another to star at the far right and make them swap their values
    for (int i = 0; i < height; i++)
    {
        int swap = width - 1;
        int j = 0;
        while (swap > j)
        {
            RGBTRIPLE image_temp = image[i][j];
            image[i][j] = image [i][swap];
            image [i][swap] = image_temp;
            swap--;
            j++;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a temporary copy of image to use the original values without modification and iterate through image copying each pixel to "copy"
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float count = 0;
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    if ((x >= 0 && x < height) && (y >= 0 && y < width))
                    {
                        sumRed += copy[x][y].rgbtRed;
                        sumGreen += copy[x][y].rgbtGreen;
                        sumBlue += copy[x][y].rgbtBlue;
                        count++;
                    }
                }
                image[i][j].rgbtRed = round(sumRed / count);
                image[i][j].rgbtGreen = round(sumGreen / count);
                image[i][j].rgbtBlue = round(sumBlue / count);
            }
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a temporary copy of image to use the original values without modification and iterate through image copying each pixel to "copy"
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }


    int gx_grid[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gy_grid[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumGxRed = 0;
            int sumGxGreen = 0;
            int sumGxBlue = 0;
            int sumGyRed = 0;
            int sumGyGreen = 0;
            int sumGyBlue = 0;
            for (int x = i - 1, x2 = 0; x <= i + 1 && x2 < 3; x++, x2++)
            {
                for (int y = j - 1, y2 = 0; y <= j + 1 && y2 < 3; y++, y2++)
                {
                    if ((x >= 0 && x < height) && (y >= 0 && y < width))
                    {
                        sumGxRed = sumGxRed + (copy[x][y].rgbtRed * gx_grid[x2][y2]);
                        sumGxGreen = sumGxGreen + (copy[x][y].rgbtGreen * gx_grid[x2][y2]);
                        sumGxBlue = sumGxBlue + (copy[x][y].rgbtBlue * gx_grid[x2][y2]);

                        sumGyRed = sumGyRed + (copy[x][y].rgbtRed * gy_grid[x2][y2]);
                        sumGyGreen = sumGyGreen + (copy[x][y].rgbtGreen * gy_grid[x2][y2]);
                        sumGyBlue = sumGyBlue + (copy[x][y].rgbtBlue * gy_grid[x2][y2]);

                    }
                }
            }
            int red = round(sqrt((sumGxRed * sumGxRed) + (sumGyRed * sumGyRed)));
            int green = round(sqrt((sumGxGreen * sumGxGreen) + (sumGyGreen * sumGyGreen)));
            int blue = round(sqrt((sumGxBlue * sumGxBlue) + (sumGyBlue * sumGyBlue)));

            image[i][j].rgbtRed = (red > 255) ? 255 : red;
            image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
        }
    }
    return;
}
