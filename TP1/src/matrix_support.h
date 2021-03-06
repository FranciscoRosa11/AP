#ifndef MATRIX_SUPPORT_H
#define MATRIX_SUPPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * Calculate number of bytes in an integer matrix of rows x cols
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 */
extern int matrix_size(int rows, int cols);

/**
 * Equivalent of matrix[i, j] for compilers that do not like indexes
 * @param matrix pointer to matrix array
 * @param i row index
 * @param j col index
 * @param cols number of cols in the matrix
 * @return value at matrix row i, column j
 */
extern int *offset(int *matrix, int i, int j, int cols);

/**
 * Allocate a new matrix
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @return a pointer to the matrix
 */
extern int *new_matrix(int rows, int cols);

/**
 * Fill the given matrix with a random integer that is >= rand_min and <= rand_max
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param matrix pre-allocated two dimensional array of ints for the matrix
 * @param rand_min the minimum random number (inclusive)
 * @param rand_max the maximum random number (inclusive)
 */
extern void fill_matrix_random(int rows, int cols, int *matrix, int rand_min, int rand_max);

/**
 * Compare two matrices and return true if they have the same elements
 *
 * @param rows number of rows in the matrices
 * @param cols number of columns in the matrices
 * @param matrix1 matrix to compare
 * @param matrix2 matrix to compare
 * @return true if they're equal, false if not
 */
extern bool matrices_equal(int rows, int cols, int *matrix1, int* matrix2);

extern void fill_matrix_constant(int rows, int cols, int *matrix, int constant);


extern void write_matrix(FILE *out, char *label, char sep, int rows, int cols, int *matrix);


/**
 * Print the matrix of to stdout
 *
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param matrix two dimensional array of ints for the matrix
 */
extern void print_matrix(char *label, int rows, int cols, int *matrix);
extern void debug_matrix(char *label, int rows, int cols, int *matrix);
#endif