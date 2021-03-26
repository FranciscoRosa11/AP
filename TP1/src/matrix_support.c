//
// simpler support methods with no arrays to allow compilation by nvcc
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "matrix_support.h"
#include "log.h"

/**
 * Calculate number of bytes in an integer matrix of rows x cols
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 */
int matrix_size(int rows, int cols) {
    return rows * cols * sizeof(int);
}

/**
 * Equivalent of matrix[i, j] for compilers that do not like indexes
 * @param matrix pointer to matrix array
 * @param i row index
 * @param j col index
 * @param cols number of cols in the matrix
 * @return value at matrix row i, column j
 */
int *offset(int *matrix, int i, int j, int cols)
{
    return matrix + (i * cols) + j;
}

/**
 * Allocate a new matrix
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @return a pointer to the matrix
 */
int *new_matrix(int rows, int cols) {
    int *matrix = (int *)malloc(matrix_size(rows, cols));
    return matrix;
}

/**
 * Fill the given matrix with a random integer that is >= rand_min and <= rand_max and make sure the matrix is symmetric
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param matrix pre-allocated two dimensional array of ints for the matrix
 * @param rand_min the minimum random number (inclusive)
 * @param rand_max the maximum random number (inclusive)
 */
void fill_matrix_random(int rows, int cols, int *matrix, int rand_min, int rand_max)
{
    int range = rand_max - rand_min + 1;
    for (int i = 0; i < rows; ++i) {
        for (int j = i; j < cols; ++j) {
            int random_number = (rand() % range) + rand_min;
            *offset(matrix, i, j, cols) = random_number;
            *offset(matrix, j, i, cols) = random_number;
        }
    }
}

/**
 * Fill the given matrix with a constant integer
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param matrix pre-allocated two dimensional array of ints for the matrix
 */
void fill_matrix_constant(int rows, int cols, int *matrix, int constant)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *offset(matrix, i, j, cols) = constant;
        }
    }
}

/**
 * Write the matrix out to the specified file pointer
 * @param out output file (or stream)
 * @param label heading for the table
 * @param sep separator between columns
 * @param rows number of rows
 * @param cols number of columns
 * @param matrix pointer to the start of the matrix
 */
void write_matrix(FILE *out, char *label, char sep, int rows, int cols, int *matrix)
{
    if (label != NULL) {
        fprintf(out, "\nMatrix %s (%dx%d)\n", label, (int)rows, (int)cols);
    }
    for (int i = 0; i < rows; ++i) {
        if (i > 0) {
            fprintf(out, "\n"); // break line between rows
        }
        for (int j = 0; j < cols; ++j) {
            if (sep > 0) {
                // format for csv - no whitespace
                if (j > 0) {
                    fprintf(out, "%c%3d", sep, *offset(matrix, i, j, cols));
                }
                else {
                    fprintf(out, "%3d", *offset(matrix, i, j, cols)); // first cell on row
                }
            }
            else {
                // format for neat alignment
                fprintf(out, "%3d  ", *offset(matrix, i, j, cols));
            }
        }
    }
    fprintf(out, "\n");
}

/**
 * Print the matrix of to stdout
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param matrix two dimensional array of ints for the matrix
 */
void print_matrix(char *label, int rows, int cols, int *matrix)
{
    write_matrix(stdout, label, -1, rows, cols, matrix);
    printf("\n");
}
