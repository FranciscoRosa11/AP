//
// simpler support methods with no arrays to allow compilation by nvcc
//
#include <stdio.h>
#include <stdlib.h>
#include "matrix_support.h"
#include "log.h"
#include "config.h"
#include "rooms_support.h"

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
 * Finds which room the given person is in and stores the room number and position (0 or 1)
 * of their occupancy in that room
 * @param person person to find
 * @param rooms_array rooms
 * @param num_rooms number of rooms
 * @param room_number pointer to a room number
 * @param position pointer to the position
 */
void find_person(int person, int* rooms_array, int num_rooms, int *room_number, int *position)
{
    for (int room = 0; room < num_rooms; room++) {
        for (int pos = 0; pos < PERSONS_PER_ROOM; pos++) {
            if (person == *offset(rooms_array, room, pos, PERSONS_PER_ROOM)) {
                // found them
                *room_number = room;
                *position = pos;
                return;
            }
        }
    }
    // no where - this should never happen
    ERROR("The person %d was nowhere to be found - this should never happen", person);
}

/**
 * Swaps first occupants between rooms
 * @param rooms_array rooms array
 * @param first_room first room to swap from/to
 * @param next_room next room to swap from/to
 * @return the delta in total cost this will cause
 */
int swap_first_occupant(int *rooms_array, int first_room, int next_room, int *cost_matrix, int num_persons)
{
    return swap_occupants(rooms_array, first_room, 0, next_room, 0,
                          cost_matrix, num_persons);
}

/**
 * Swaps occupants between rooms and positions
 * @param rooms_array rooms array
 * @param first_room first room to swap from/to
 * @param first_pos position of person in first room
 * @param other_room next room to swap from/to
 * @param other_pos position of the person in the other room
 *
 * @return the delta in total cost this will cause
 */
int swap_occupants(int *rooms_array, int first_room, int first_pos, int other_room, int other_pos,
                   int *cost_matrix, int num_persons)
{
    // get the total cost before of the two rooms
    int before_cost = room_cost(first_room, cost_matrix, num_persons, rooms_array) +
            room_cost(other_room, cost_matrix, num_persons, rooms_array);

    // save the first occupant while their place is taken
    int occupant_first_room = *offset(rooms_array, first_room, first_pos, PERSONS_PER_ROOM);
    // move the occupant from the other room into this room
    *offset(rooms_array, first_room, first_pos, PERSONS_PER_ROOM) =
            *offset(rooms_array, other_room, other_pos, PERSONS_PER_ROOM);
    // now move the saved occupant into the other ones place
    *offset(rooms_array, other_room, other_pos, PERSONS_PER_ROOM) = occupant_first_room;

    int after_cost = room_cost(first_room, cost_matrix, num_persons, rooms_array) +
                      room_cost(other_room, cost_matrix, num_persons, rooms_array);
    int delta = after_cost - before_cost;
    DEBUG("Delta %d after swapping room %d:%d x %d:%d", delta, first_room, first_pos, other_room, other_pos);
    return delta;
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
    for (int room = 0; room < num_rooms; room++) {
        cost = cost + room_cost(room, cost_matrix, num_persons, rooms_array);
    }
    return cost;
}

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
int room_cost(int room_number, int *cost_matrix, int num_persons, int *rooms_array)
{
    int first = first_occupant(rooms_array, room_number); // person 0 index in room room_number
    int second = second_occupant(rooms_array, room_number); // person 1 index in room room_nuber
    return *offset(cost_matrix, first, second, num_persons);
}

/**
 * Compute the cost change that will result from swapping the first occupant between 2 rooms
 * @param c the first room
 * @param d the 2nd room
 * @param cost_matrix the cost matrix
 * @param num_persons total number of people
 * @param rooms_array array of all rooms
 * @param num_rooms total number of rooms
 * @return postitive or negative integer indicating the change in cost
 */
int delta_cost_for_swap(int c, int d, int *cost_matrix, int num_persons, int *rooms_array, int num_rooms)
{
    int roomC1 = first_occupant(rooms_array, c);
    int roomC2 = second_occupant(rooms_array, c);
    int roomD1 = first_occupant(rooms_array, d);
    int roomD2 = second_occupant(rooms_array, d);
    int costC1D2 = *offset(cost_matrix, roomC1, roomD2, num_persons);
    int costD1C2 = *offset(cost_matrix, roomD1, roomC2, num_persons);
    int costC1C2 = *offset(cost_matrix, roomC1, roomC2, num_persons);
    int costD1D2 = *offset(cost_matrix, roomD1, roomD2, num_persons);

    int delta = costC1D2 + costD1C2 - costC1C2 - costD1D2;
    TRACE("\ndelta calc: room c (%d)=[%d, %d]  cost=%d", c, roomC1, roomC2, costC1C2);
    TRACE("delta calc: room d (%d)=[%d, %d]  cost=%d", d, roomD1, roomD2, costD1D2);
    TRACE("delta calc: swapC(%d)=[%d, %d]  cost=%d", c, roomC1, roomD2, costC1D2);
    TRACE("delta calc: swapD(%d)=[%d, %d]  cost=%d", d, roomD1, roomC2, costD1C2);
    TRACE("delta calc: delta %d = %d + %d - %d - %d\n", delta, costC1D2, costD1C2, costC1C2, costD1D2);
    return delta;
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
                    cost = MAX_COST;
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

