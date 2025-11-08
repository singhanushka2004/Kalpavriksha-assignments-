#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateMatrix(int size, int *matrix) {
    srand(time(NULL));
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
           *(matrix + row*size + column) = rand() % 256;
        }
    }
}

void displayTheMatrix(int size, int *matrix) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            printf("%4d", *(matrix + row*size + column));
        }
        printf("\n");
    }
}

void rotateMatrix90Clockwise(int size, int *matrix) {
    for (int row = 0; row < size; row++) {
        for (int column = row + 1; column < size; column++) {
            int* firstValue = matrix + row*size + column;
            int* secondValue = matrix + column*size + row;
            int temp = *firstValue;
            *firstValue = *secondValue;
            *secondValue = temp;
        }
        int* left = matrix + row*size;
        int* right = matrix + row*size + (size - 1);
        while (left < right) {
            int temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
    }
}

void Smoothing(int size, int *matrix) {
    int *tempRow = (int *)malloc(size * sizeof(int)); 

    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            int sum = 0, count = 0;
            for (int windowRow = -1; windowRow <= 1; windowRow++) {
                for (int windowCol = -1; windowCol <= 1; windowCol++) {
                    int neighborRow = row + windowRow;
                    int neighborCol = column + windowCol;
                    if (neighborRow >= 0 && neighborRow < size &&
                        neighborCol >= 0 && neighborCol < size) {
                        sum += *(matrix + neighborRow*size + neighborCol);
                        count++;
                    }
                }
            }
            *(tempRow + column) = sum / count;  
        }
        for (int column = 0; column < size; column++) {
            *(matrix + row*size + column) = *(tempRow + column);
        }
    }
    free(tempRow);
}


int main() {
    int sizeOfMatrix = 0;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &sizeOfMatrix);

    if (sizeOfMatrix < 2 || sizeOfMatrix > 10) {
        printf("Invalid size of matrix! (2 ≤ Size Of Matrix ≤ 10)\n");
        return 1;
    }

    int *matrix = malloc(sizeOfMatrix * sizeOfMatrix * sizeof(int));
    printf("\nOriginal Randomly Generated Matrix:\n");
    generateMatrix(sizeOfMatrix, matrix);
    displayTheMatrix(sizeOfMatrix, matrix);

    rotateMatrix90Clockwise(sizeOfMatrix, matrix);
    printf("\nMatrix after 90° Clockwise Rotation:\n");
    displayTheMatrix(sizeOfMatrix, matrix);

    Smoothing(sizeOfMatrix, matrix);
    printf("\nMatrix after Applying 3×3 Smoothing Filter:\n");
    displayTheMatrix(sizeOfMatrix, matrix);
    
    free(matrix); 
    return 0;
}