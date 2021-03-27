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
    metrics->algo_name = "Greedy_Person";
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

    // create an array of zeros, one for each person, and set them to
    // 1 when that person has already found their best match
    int *taken = new_matrix(num_persons, 1);
    fill_matrix_constant(num_rooms, 1, taken, 0);

    // loop over all people
    for(int person = 0; person < num_persons; person++) {
        int person_room, person_position;
        find_person(person, rooms_array, num_rooms, &person_room, &person_position);
        steps = steps + 1;
        DEBUG("Starting step %d", steps);
        // Start with the first person and find their mate
        TRACE("Checking all persons for best match for person %d (in room %d)", person, person_room);
        int best_match_cost = MAX_COST;
        int best_match = -1; // start with best match being no match
        for (int other_person = 0; other_person < num_persons; other_person++) {
            if (other_person != person) { // never match with themselves
                TRACE("Checking person %d for a match for person %d", other_person, person);
                // skip the other person it if is already taken
                if (taken[other_person] == 0) {
                    int match_cost = *offset(cost_matrix, person, other_person, num_persons);
                    if (match_cost < best_match_cost) {
                        best_match_cost = match_cost;
                        best_match = other_person;
                        DEBUG("Found new best match for person %d : person %d with cost %d",
                              person, other_person, best_match_cost);
                    } else {
                        TRACE("Passing person %d because match cost %d is not better than current best: %d",
                              other_person, match_cost, best_match_cost);
                    }
                } else {
                    TRACE("Skipping person %d because they're already taken", other_person);
                }
            }
        }

        int other_room, other_position;
        if (best_match < 0) {
            WARN("Found no best match for person %d", person);
        }
        else {
            // find the other person and swap them
            find_person(best_match, rooms_array, num_rooms, &other_room, &other_position);
            // only swap if we don't aren't already in the same room
            // but mark this these as taken either way
            taken[person] = 1;
            taken[best_match] = 1;
            if (person_room != other_room) {
                DEBUG("MATCH: SWAP person %d in room %d at %d with person %d in room %d at %d",
                      person, person_room, person_position, best_match, other_room, other_position);
                debug_matrix("Rooms before swap", num_rooms, PERSONS_PER_ROOM, rooms_array);
                int delta = swap_occupants(rooms_array, person_room, person_position, other_room, other_position,
                                           cost_matrix, num_persons);
                debug_matrix("Rooms AFTER  swap", num_rooms, PERSONS_PER_ROOM, rooms_array);
                cost = cost + delta;
            } else {
                DEBUG("Not swapping because best match is already in the same room: %d", best_match);
            }
        }
    }
    metrics->steps = steps;
    metrics->cost = cost;
}
