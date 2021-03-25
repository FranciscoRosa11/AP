/**
 * Simple Sequential Implementation of the K-Means Lloyds Algorithm
 */
#include "config.h"
#include "log.h"
#include "matrix_support.h"

void run_rooms(int* cost_matrix, int num_persons, int* rooms_array, int num_rooms, int num_processes)
{
    INFO("NOOP run_rooms with num_persons=%d, num_rooms=%d, num_processes=%d", num_persons, num_rooms, num_processes);
    print_matrix("Cost Matrix", num_persons, num_persons, cost_matrix);
    print_matrix("Rooms Array", num_rooms, 2, rooms_array);

}

void finalize_rooms()
{
    INFO("NOOP finalize_rooms");
}
