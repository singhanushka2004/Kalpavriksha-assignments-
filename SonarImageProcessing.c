#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateMatrix(int size, int matrix[size][size])
{
    srand(time(NULL));
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            matrix[row][column] = rand() % 256;
        }
    }
}

void displayMatrix(int size, int matrix[size][size])
{
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            printf("%4d", matrix[row][column]);
        }
        printf("\n");
    }
}

void reverseRows(int size, int matrix[size][size])
{
    for (int row = 0; row < size; row++)
    {
        int *left = *(matrix + row);
        int *right = *(matrix + row) + (size - 1);

        while (left < right)
        {
            int temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
}

void transposeMatrix(int size, int matrix[size][size])
{
    for (int row = 0; row < size; row++)
    {
        for (int column = row + 1; column < size; column++)
        {
            int *firstValue = *(matrix + row) + column;
            int *secondValue = *(matrix + column) + row;

            int temp = *firstValue;
            *firstValue = *secondValue;
            *secondValue = temp;
        }
    }
    reverseRows(size, matrix);
}

void applyFilter(int size, int matrix[size][size])
{
    int tempMatrix[size][size];
    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            int sum = 0, count = 0;

            for (int windowRow = -1; windowRow <= 1; windowRow++)
            {
                for (int windowCol = -1; windowCol <= 1; windowCol++)
                {
                    int neighborRow = row + windowRow;
                    int neighborCol = column + windowCol;

                    if (neighborRow >= 0 && neighborRow < size &&
                        neighborCol >= 0 && neighborCol < size)
                    {
                        sum += *(*(matrix + neighborRow) + neighborCol);
                        count++;
                    }
                }
            }

            *(*(tempMatrix + row) + column) = sum / count;
        }
    }

    for (int row = 0; row < size; row++)
    {
        for (int column = 0; column < size; column++)
        {
            *(*(matrix + row) + column) = *(*(tempMatrix + row) + column);
        }
    }
}

int main()
{
    int sizeOfMatrix = 0;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &sizeOfMatrix);

    if (sizeOfMatrix < 2 || sizeOfMatrix > 10)
    {
        printf("Invalid size of matrix.\n");
        return 1;
    }

    int matrix[sizeOfMatrix][sizeOfMatrix];
    printf("\nOriginal Randomly Generated Matrix:\n");
    generateMatrix(sizeOfMatrix, matrix);
    displayMatrix(sizeOfMatrix, matrix);

    printf("\nMatrix after 90 Clockwise Rotation:\n");
    displayMatrix(sizeOfMatrix, matrix);
    applyFilter(sizeOfMatrix, matrix);

    printf("\nMatrix after Applying 3*3 Smoothing Filter:\n");
    displayMatrix(sizeOfMatrix, matrix);

    return 0;
}



