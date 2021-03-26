#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rooms.h"
#include "matrix_support.h"

void run_rooms(int* cost_matrix, int num_persons, int* rooms_array, int num_rooms, int num_processes) {
    
    /*
        Assign rooms to students in a random way.
    */

    int n = num_persons * num_persons; //equivalent of n = length(cost_matrix)

    randperm(num_persons/2, num_persons/2, &rooms_array);

    /*
        Compute the value of cost for the initial distribution
    */

    int cost = 0;
    int i = 0;
    for(i = 0; i <= num_persons/2; i++) {
        int indexRoom1 = offset(&rooms_array, i, 1, 2);
        int indexRoom2 = offset(&rooms_array, i, 2, 2);

        cost = cost + offset(&cost_matrix, indexRoom1, indexRoom2, num_persons/2);
    } 

    /*
        Initialize algorithm
    */

    i = 0;
    int steps = 0, T = 1;

    while(i < 100) {
        steps = steps + 1;
        int c = rand() % (num_persons/2) + 1;
        int d;

        if(c == (num_persons/2))
            d = 1;
        else
            d = c + 1;

        int delta;
        int roomC1 = offset(&rooms_array, c, 1, 2);
        int roomC2 = offset(&rooms_array, c, 2, 2);
        int roomD1 = offset(&rooms_array, d, 1, 2);
        int roomD2 = offset(&rooms_array, d, 2, 2);
        int costC1D2 = offset(&cost_matrix, roomC1, roomD2, num_persons);
        int costD1C2 = offset(&cost_matrix, roomD1, roomC2, num_persons);
        int costC1C2 = offset(&cost_matrix, roomC1, roomC2, num_persons);
        int costD1D2 = offset(&cost_matrix, roomD1, roomD2, num_persons);

        delta = costC1D2 + costD1C2 - costC1C2 - costD1D2;

        if(delta < 0 || exp(-delta / T) >= (rand() % 0 + 1)) { // not really sure on this rand() % 0 + 1 
            //swap room(c1,1) and room(d1,1)
            int temp = rooms_array[roomC1];
            rooms_array[roomC1] = rooms_array[roomD1];
            rooms_array[roomD1] = temp;
            
            cost = cost + delta;
            i = 0;
        } else {
            i = i+1;
        }

        T = 0.999*T;
    }
}

int main() {

}