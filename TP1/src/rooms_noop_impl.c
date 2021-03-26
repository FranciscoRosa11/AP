/**
 * Simple Sequential Implementation of the K-Means Lloyds Algorithm
 */
#include "config.h"
#include "log.h"
#include "matrix_support.h"
#include "rooms.h"

void run_rooms(int* cost_matrix, int num_persons, int* rooms_array, int num_rooms, struct metrics *metrics)
{
    INFO("NOOP run_rooms with num_persons=%d, num_rooms=%d, num_processes=%d", num_persons, num_rooms);
    print_matrix("Cost Matrix", num_persons, num_persons, cost_matrix);
    print_matrix("Rooms Array", num_rooms, 2, rooms_array);
    metrics->steps = 0;
    metrics->cost = 0;
}

