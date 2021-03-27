#include <stdio.h>
#include "config.h"
#include "rooms.h"
#include "matrix_support.h"
#include "rooms_support.h"
#include "math.h"

/**
 * Greedy Implementation:
 * Pairs every person with their best mate, keeping track of which mates are taken.
 */
void initialize_algorithm(struct config *config, struct metrics *metrics)
{
    metrics->algo_name = "Greedy";
}

/**
* Run the room assignment algorithm starting with a number of persons, a number of rooms (persons/2)
* an initial assignment of persons to rooms, and a a cost matrix that determines the compatibility
* between any two persons.
*
* @param cost_matrix square symmetrical matrix giving the compatibility between pairs of persons
*        where 1 is most compatible, and 10 is least compatible
* @param num_persons the number of persons paired in rooms
* @param rooms_array the array of rooms with initial (random) assignments of persons
* @param num_rooms the number of rooms: always half the number of persons
* @param metrics pointer to a structure to hold the cost and number of steps used (to return)
* @param stability_indicator number of times to iterate without a swap before determining that we are done
*/
void run_rooms(int *cost_matrix, int num_persons, int *rooms_array, int num_rooms, struct metrics *metrics,
               int stability_indicator)
{
    // Compute the value of cost for the initial distribution
    int cost = compatibility_cost(cost_matrix, num_persons, rooms_array, num_rooms);
    metrics->initial_cost = cost;
    INFO("Starting cost calculated as: %d", cost);

    // Initialize algorithm
    int steps = 0;

    // create an array of zeros, one for each room, and set them to
    // 1 when that room has already had their first occupant taken
    int *taken = new_matrix(num_rooms, 1);
    fill_matrix_constant(num_rooms, 1, taken, 0);

    // loop until there are 100 iterations without a swap
    for(int c = 0; c < num_rooms; c++) {
        steps = steps + 1;
        DEBUG("Starting step %d", steps);
        // Start with the first room and find get the 2nd occupant, and find the other
        // room that has the best first occupant for this room. (it may be the same one)
        int roomC2 = second_occupant(rooms_array, c);
        TRACE("Checking all rooms for best match for person %d in room %d", roomC2 , c);
        int best_match_cost = MAX_COST;
        int d = c; // start with best matching room being the same as the current room
        for (int other_room = 0; other_room < num_rooms; other_room++) {
            TRACE("Looking in room %d for a match for room %d", other_room, c);
            // skip the other room it if is already taken
            if (taken[other_room] == 0) {
                int roomD1 = first_occupant(rooms_array, other_room);
                int costD1C2 = *offset(cost_matrix, roomD1, roomC2, num_persons);
                if (costD1C2 < best_match_cost) {
                    best_match_cost = costD1C2;
                    d = other_room;
                    DEBUG("Found new best match for person %d in room %d: person %d in room %d with cost %d",
                          roomC2, c, roomD1, d, best_match_cost);
                }
                else {
                    TRACE("Passing room %d because match cost %d is not better than current best: %d",
                          other_room, costD1C2, best_match_cost);
                }
            }
            else {
                TRACE("Skipping room %d because its first occupant is already taken", other_room);
            }
        }
        int delta = delta_cost_for_swap(c, d, cost_matrix, num_persons, rooms_array, num_rooms);
        // d is now the room with the most compatible roommate for C2
        // only swap if we don't aren't already in the same room
        // but mark this room as taken either way
        taken[d] = 1;
        taken[c] = 1; // mark C as taken too, so that we don't swap back
        if (d != c) {
            DEBUG("SWAP: Swapping first occupants in rooms %d x %d", c, d);
            debug_matrix("Rooms before swap", num_rooms, PERSONS_PER_ROOM, rooms_array);
            swap_first_occupant(rooms_array, c, d);
            debug_matrix("Rooms AFTER  swap", num_rooms, PERSONS_PER_ROOM, rooms_array);
            cost = cost + delta;
        } else {
            DEBUG("Not swapping because best match is already in the same room: %d", d);
        }
    }
    metrics->steps = steps;
    metrics->cost = cost;
}
