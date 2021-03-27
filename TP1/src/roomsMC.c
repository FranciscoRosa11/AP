#include <stdio.h>
#include "config.h"
#include "rooms.h"
#include "matrix_support.h"
#include "rooms_support.h"
#include "math.h"

/**
 * Monte Carlo Implementation:
 * This uses the base algorithm: With each iteration a room is chosen at random and its first
 * occupant tentatively swapped with first occupant of the next room. If the swap would reduce
 * the overall cost, then the swap is made, otherwise it is not.
 * After a certain number of iterations with no swaps, the assignments are considered stable and
 * the algorithm exits.
 */

void initialize_algorithm(struct config *config)
{
    // NOOP
}

void start_iteration()
{
    // NOOP
}

void end_iteration()
{
    // NOOP
}

/**
 * Swap only if the swap is an improvement ( delta < 0 )
 */
bool should_swap(int delta)
{
    return delta < 0;
}

