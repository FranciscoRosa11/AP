#include <stdio.h>
#include "config.h"
#include "rooms.h"
#include "matrix_support.h"
#include "rooms_support.h"
#include "math.h"

/**
 * Should be implemented to perform any activity needed at the start of each iteration of the random loop
 */
extern void start_iteration();

/**
 * Should be implemented to perform any activity needed at the end of each iteration of the random loop
 */
extern void end_iteration();

/**
 * Return true if the current rooms should be swapped based on the delta calculated
 */
extern bool should_swap(int delta);

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
    int i = 0;
    int steps = 0;

    // loop until there are 100 iterations without a swap
    for(;i < stability_indicator;) {
        start_iteration();
        steps = steps + 1;

        DEBUG("Starting step %d. Steps without swap: %d", steps, i);
        int c = random_int(num_rooms); // index of random room
        int d = next_room(c, num_rooms); // index of the next room

        int delta = delta_cost_for_swap(c, d, cost_matrix, num_persons, rooms_array, num_rooms);

        if (should_swap(delta)) {
            DEBUG("SWAP: Swapping first occupants in rooms %d x %d", c, d);
            debug_matrix("Rooms before swap", num_rooms, PERSONS_PER_ROOM, rooms_array);
            swap_first_occupant(rooms_array, c, d, cost_matrix, num_persons);
            debug_matrix("Rooms AFTER  swap", num_rooms, PERSONS_PER_ROOM, rooms_array);
            cost = cost + delta;
            i = 0; // restart the count
        } else {
            i = i + 1; // count non-swap steps
        }

        end_iteration();
    }
    metrics->steps = steps;
    metrics->cost = cost;
}
