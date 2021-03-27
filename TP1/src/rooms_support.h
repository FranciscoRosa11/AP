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

/**
 * Use the compatibility cost_matrix to find the cost of a single room
 * @param room_number the number of the room whose cost we calculate and return
 * @param cost_matrix square symmetrical matrix giving the compatibility between pairs of persons
 *        where 1 is most compatible, and 10 is least compatible
 * @param num_persons the total number of persons paired in rooms
 * @param rooms_array the array of rooms with initial (random) assignments of persons
 *
 * @return the the cost for just this room
 */
extern int room_cost(int room_number, int *cost_matrix, int num_persons, int *rooms_array);

/**
 * Finds which room the given person is in and stores the room number and position (0 or 1)
 * of their occupancy in that room
 * @param person person to find
 * @param rooms_array rooms
 * @param num_rooms number of rooms
 * @param room_number pointer to a room number
 * @param position pointer to the position
 */
extern void find_person(int person, int* rooms_array, int num_rooms, int *room_number, int *position);

extern int delta_cost_for_swap(int c, int d, int *cost_matrix, int num_persons, int *rooms_array, int num_rooms);

extern int next_room(int room_number, int num_rooms);

extern int first_occupant(int *rooms_array, int room_number);
extern int second_occupant(int *rooms_array, int room_number);


/**
 * Swaps occupants between rooms and positions
 * @param rooms_array rooms array
 * @param first_room first room to swap from/to
 * @param first_pos position of person in first room
 * @param other_room next room to swap from/to
 * @param other_pos position of the person in the other room
 * @param cost_matrix the cost matrix
 * @param num_persons total number of persons
 * @return the delta in total cost this will cause
 */
extern int swap_occupants(int *rooms_array, int first_room, int first_pos, int other_room, int other_pos,
                          int *cost_matrix, int num_persons);
extern int swap_first_occupant(int *rooms_array, int first_room, int next_room, int *cost_matrix, int num_persons);

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
extern int random_int(int max);

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
