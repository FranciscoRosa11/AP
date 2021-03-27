#include <stdio.h>
#include "config.h"
#include "rooms.h"
#include "matrix_support.h"
#include "rooms_support.h"
#include "math.h"

void run_rooms(int *cost_matrix, int num_persons, float temp, int *rooms_array, int num_rooms, struct metrics *metrics)
{
    int n = num_persons * num_persons; //equivalent of n = length(cost_matrix)

    // Compute the value of cost for the initial distribution
    int cost = compatibility_cost(cost_matrix, num_persons, rooms_array, num_rooms);

    // Initialize algorithm
    int i = 0;
    int steps = 0;
    float T = temp;

    // loop until there are 100 iterations without a swap
//    #pragma omp parallel default(none) private(i) shared(steps, num_rooms, rooms_array, cost_matrix, num_persons)
    for(;i < 100;) {
        steps = steps + 1;

        DEBUG("Starting step %d. Steps without swap: %d", steps, i);
        int c;
        c = random_count(num_rooms) - 1; // index of random room
        int d = next_room(c, num_rooms); // index of the next room

        int delta;

        /*
            Compute the values of room(c1,1), room(c1,2) etc etc
        */
        int roomC1 = first_occupant(rooms_array, c) - 1;
        int roomC2 = second_occupant(rooms_array, c) - 1;
        int roomD1 = first_occupant(rooms_array, d) - 1;
        int roomD2 = second_occupant(rooms_array, d) - 1;
        int costC1D2 = *offset(cost_matrix, roomC1, roomD2, num_persons);
        int costD1C2 = *offset(cost_matrix, roomD1, roomC2, num_persons);
        int costC1C2 = *offset(cost_matrix, roomC1, roomC2, num_persons);
        int costD1D2 = *offset(cost_matrix, roomD1, roomD2, num_persons);

        delta = costC1D2 + costD1C2 - costC1C2 - costD1D2;
        DEBUG("room (%d)=[%d, %d]  cost=%d", c, roomC1, roomC2, costC1C2);
        DEBUG("next (%d)=[%d, %d]  cost=%d", d, roomD1, roomD2, costD1D2);
        DEBUG("swapC(%d)=[%d, %d]  cost=%d", c, roomC1, roomD2, costC1D2);
        DEBUG("swapD(%d)=[%d, %d]  cost=%d", d, roomD1, roomC2, costD1C2);
        DEBUG("delta %d = %d + %d - %d - %d", delta, costC1D2, costD1C2, costC1C2, costD1D2);

        // if the swap is an improvement ( delta < 0 ) or if the delta
        // is small enough such that e^-(delta/T) > some random number between 0 and 1,
        // then swap
        float delta_on_T = (float)delta / T;
        float exp_minus_delta_on_T = expf(-1.f * delta_on_T);
        float threshold = random_float();
        DEBUG("delta=%d   T=%f  delta/T=%f  exp=%f, threshold=%f,  cost=%d",
              delta, T, delta_on_T, exp_minus_delta_on_T, threshold, cost);
        if (delta < 0 || exp_minus_delta_on_T > threshold) {
            //swap room(c1,1) and room(d1,1)
            int temp = rooms_array[roomC1];
            rooms_array[roomC1] = rooms_array[roomD1];
            rooms_array[roomD1] = temp;
            cost = cost + delta;
            i = 0; // restart the count
            DEBUG("Delta = %d:  swapping person %d in room %d with neighbour %d from room %d", delta, roomC1, c, roomD1, d);
        } else {
            i = i + 1; // count non-swap steps
        }

        T = 0.999 * T; // reduce the temperature (annealing)
    }
    metrics->steps = steps;
    metrics->cost = cost;
}
