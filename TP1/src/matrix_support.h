//
// simpler support methods with no arrays to allow compilation by nvcc
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * Calculate number of cells in a matrix of n x n
 * @param n size (square) of matrix
 */
int matrix_size(int n) {
    return n * n * sizeof(int);
}

/**
 * Equivalent of matrix[i, j] for compilers that do not like indexes
 * @param matrix pointer to matrix array
 * @param i row index
 * @param j col index
 * @param n matrix size (square)
 * @return value at matrix row i, column j
 */
int *offset(int *matrix, int i, int j, int n) {
    return matrix + (i * n) + j;
}

/**
 * Allocate a new square matrix
 * @param n rows and columns in square matrix
 * @return a pointer to the matrix
 */
int *new_matrix(int n) {
    int *matrix = (int *)malloc(matrix_size(n));
    return matrix;
}

/**
 * Fill the given square matrix with a random value
 *
 * @param size number of rows (and columns) in the square matrix
 * @param matrix pre-allocated two dimensional array of ints for the matrix
 */
void fill_matrix_random(int size, int *matrix)
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            *offset(matrix, i, j, size) = ((int) rand()) / ((int) RAND_MAX);
        }
    }
}

void write_matrix(FILE *out, char *label, char sep, size_t size, int *matrix)
{
    if (label != NULL) {
        fprintf(out, "\nMatrix %s (%dx%d)\n", label, (int)size, (int)size);
    }
    for (int i = 0; i < size; ++i) {
        if (i > 0) {
            fprintf(out, "\n"); // break line between rows
        }
        for (int j = 0; j < size; ++j) {
            if (sep > 0) {
                // format for csv - no whitespace
                if (j > 0) {
                    fprintf(out, "%c%.3f", sep, *offset(matrix, i, j, size));
                }
                else {
                    fprintf(out, "%.3f", *offset(matrix, i, j, size)); // first cell on row
                }
            }
            else {
                // format for neat alignment
                fprintf(out, "%.3f  ", *offset(matrix, i, j, size));
            }
        }
    }
    fprintf(out, "\n");
}

/**
 * Print the matrix of to stdout
 *
 * @param size number of rows (and columns) in the square matrix
 * @param matrix two dimensional array of ints for the matrix
 */
void print_matrix(char *label, int size, int *matrix)
{
    write_matrix(stdout, label, -1, size, matrix);
    printf("\n");
}
