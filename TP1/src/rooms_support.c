//
// simpler support methods with no arrays to allow compilation by nvcc
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "matrix_support.h"
#include "log.h"
#include "config.h"

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
int random_int(int max)
{
    return (rand() % max);
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
    // fill matrix with numbers starting with zero
    for (int i = 0; i < size; i++) {
        array[i] = i;
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

int first_occupant(int *rooms_array, int room_number)
{
    int person = *offset(rooms_array, room_number, 0, PERSONS_PER_ROOM);
    TRACE("First  occupant of room %d = %d", room_number, person);
    return person;
}

int second_occupant(int* rooms_array, int room_number)
{
    int person = *offset(rooms_array, room_number, 1, PERSONS_PER_ROOM);
    TRACE("Second occupant of room %d = %d", room_number, person);
    return person;
}

/**
 * Swaps first occupants between rooms
 * @param rooms_array rooms array
 * @param first_room first room to swap from/to
 * @param next_room next room to swap from/to
 * @param occupant_number occupant number STARTS AT ZERO to swap between rooms
 */
void swap_first_occupant(int *rooms_array, int first_room, int next_room)
{
    int occupant_number = 0; // swap first occupant
    int occupant_first_room = *offset(rooms_array, first_room, occupant_number, PERSONS_PER_ROOM);
    *offset(rooms_array, first_room, occupant_number, 2) = *offset(rooms_array, next_room, occupant_number, PERSONS_PER_ROOM);
    *offset(rooms_array, next_room, occupant_number, 2) = occupant_first_room;
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
int compatibility_cost(int *cost_matrix, int num_persons, int *rooms_array, int num_rooms)
{
    int cost = 0;
    for (int i = 0; i < num_rooms; i++) {
        int first = first_occupant(rooms_array, i); // person 0 index in room i
        int second = second_occupant(rooms_array, i); // person 1 index in room i
        // persons start at zero
        cost = cost + *offset(cost_matrix, first, second, num_persons);
    }
    return cost;
}

/**
 * Sets up a constant initial cost matrix for for testing.
 * The setup is such that everyone likes everyone else except for p1 and p2 who hate each other
 *          p1   p2   p3   p4
 *     p1   0    1    1    1
 *     p2   1    0   10    1
 *     p3   1   10    0    1
 *     p4   1    1    1    0
 */
int *setup_test_cost_matrix()
{
    int rows = 4, cols = 4;
    int *matrix = new_matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = i; j < cols; ++j) {
            int cost = 0;
            if (i != j) {
                cost = 1;
                if (i == 0 && j == 1) {
                    cost = 10;
                }
            }
            *offset(matrix, i, j, cols) = cost;
            *offset(matrix, j, i, cols) = cost;
        }
    }
    return matrix;
}

/**
 * Sets up a constant initial room assignment for testing.
 *          p1   p2
 * room 0:   0   1
 * room 1:   2   3
 */
int *setup_test_rooms()
{
    int *matrix = new_matrix(2, PERSONS_PER_ROOM);
    for (int i = 0; i < 4; ++i) {
        *(matrix + i) = i;
    }
    return matrix;
}

/**
 * Sets up the expected results of the test.
 * Since person 1 hates person 2, the results are expected to be:
 *          p1   p2
 * room 0:   2   1
 * room 1:   0   3
 */
int *setup_test_expected_rooms()
{
    int *expected = new_matrix(2, PERSONS_PER_ROOM);
    expected[0] = 2;
    expected[1] = 1;
    expected[2] = 0;
    expected[3] = 3;
}

