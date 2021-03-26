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
 * Return a random float between 0 and 1
 */
float random_float()
{
    return rand() / (float) RAND_MAX;
}

/**
 * Return a random counting number between 1 and max, inclusively
 * @param max the maximum random number to return
 * @return
 */
int random_count(int max)
{
    return (rand() % max) + 1;
}

/**
 * Randomly permutes all counting numbers from 1 to size through the full size of the array
 * @param rows number of rows in the matrix
 * @param cols number of columns in the matrix
 * @param array
 */
void randperm(int rows, int cols, int *array)
{
    int size = rows * cols;
    // fill matrix with counting numbers 1 - size of matrix + 1
    for (int i = 0; i < size; i++) {
        array[i] = i + 1;
    }

    // Randomly permute
    for (int i = 0; i < size; i++) {
        int j, t;
        j = rand() % (size-i) + i;
        t = array[j]; array[j] = array[i]; array[i] = t; // Swap i and j
    }
}

/**
 * Returns the index of the room after this one, or the first room if this is the last.
 * The index starts at zero and ends at num_rooms - 1 to be useful for array indices
 * @param num_rooms total number of rooms
 * @param room_number zero indexed number of the current room
 * @return the array index of the next room
 */
int next_room(int room_number, int num_rooms)
{
    int next_room = room_number + 1;
    if (next_room == num_rooms) {
        next_room = 0;
    }
    return next_room;
}

int first_occupant(int* rooms_array, int room_number)
{
    return *offset(rooms_array, room_number, 1, 2);
}

int second_occupant(int* rooms_array, int room_number)
{
    return *offset(rooms_array, room_number, 2, 2);
}

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
int compatibility_cost(int* cost_matrix, int num_persons, int* rooms_array, int num_rooms)
{
    int cost = 0;
    for (int i = 0; i < num_rooms; i++) {
        int indexroom1 = *offset(rooms_array, i, 1, 2); // person 1 index in room i
        int indexroom2 = *offset(rooms_array, i, 2, 2); // person 2 index in room i
        cost = cost + *offset(cost_matrix, indexroom1, indexroom2, num_persons);
    }
    return cost;
}
