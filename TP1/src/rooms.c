#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "rooms.h"
#include "matrix_support.h"
#include "rooms_support.h"
#include "math.h"

void run_rooms(int* cost_matrix, int num_persons, int* rooms_array, int num_rooms, int num_processes) {
    
    /*
        Assign rooms to students in a random way.
    */ 

    int n = num_persons * num_persons; //equivalent of n = length(cost_matrix)

    /*
        Compute the value of cost for the initial distribution
    */

    compatibility_cost(cost_matrix, num_persons, rooms_array, num_rooms);

    /*
        Initialize algorithm
    */

    i = 0;
    int steps = 0;

    while(i < 100) {
        steps = steps + 1;
        int c = random_count(num_rooms) - 1;
        int d;

        d = next_room(c, num_rooms);

        int delta;

        /*
            Compute the values of room(c1,1), room(c1,2) etc etc
        */
        int roomC1 = first_occupant(rooms_array, c);
        int roomC2 = second_occupant(rooms_array, c);
        int roomD1 = first_occupant(rooms_array, d);
        int roomD2 = second_occupant(rooms_array, d);
        int costC1D2 = *offset(cost_matrix, roomC1, roomD2, num_persons);
        int costD1C2 = *offset(cost_matrix, roomD1, roomC2, num_persons);
        int costC1C2 = *offset(cost_matrix, roomC1, roomC2, num_persons);
        int costD1D2 = *offset(cost_matrix, roomD1, roomD2, num_persons);

        delta = costC1D2 + costD1C2 - costC1C2 - costD1D2;

        if(delta < 0) {
            //swap room(c1,1) and room(d1,1)
            int temp = rooms_array[roomC1];
            rooms_array[roomC1] = rooms_array[roomD1];
            rooms_array[roomD1] = temp;

            cost = cost + delta;
            i = 0;
            DEBUG("Delta = %d:  swapping person %d in room %d with neighbour %d from room %d", delta, roomC1, c, roomD1, d);
        } else {
            i = i+1;
        }
    }

    metrics->steps = steps;
    metrics->cost = cost;
}