//
// simpler support methods with no arrays to allow compilation by nvcc
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "matrix_support.h"

/**
 * Use the compatibility cost_matrix to find the total cost of an array of room assignments
 *
 * @param cost_matrix square symmetrical matrix giving the compatibility between pairs of persons
 *        where 1 is most compatible, and 10 is least compatible
 * @param num_persons the number of persons paired in rooms
 * @param rooms_array the array of rooms with initial (random) assignments of persons
 * @param num_rooms the number of rooms: always half the number of persons
 *
 * @return the sum of all of the compatibility scores over all rooms
 */
extern int compatibility_cost(int* cost_matrix, int num_persons, int* rooms_array, int num_rooms);

extern int next_room(int room_number, int num_rooms);

extern int first_occupant(int* rooms_array, int room_number);
extern int second_occupant(int* rooms_array, int room_number);

/**
 * Return a random float between 0 and 1
 *
 */
extern float random_float();

/**
 * Return a random counting number between 1 and max, inclusively
 * @param max
 * @return
 */
extern int random_count(int max);

/**
 * Randomly permutes all counting numbers from 1 to size through the full size of the array
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param array
 */
extern void randperm(int rows, int cols, int *array);

extern int *setup_test_rooms();
extern int *setup_test_expected_rooms();
extern int *setup_test_cost_matrix();
