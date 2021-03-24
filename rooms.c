#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

    /*
        Assign rooms

        n = length(matrix D) --> Translate

        room = reshape(randperm(n), [], 2) --> Reshapes the dimension of input array
                                               Translate this.
    */

    int cost;

    int n; // n = length of the matrix, to be added. 

    int i = 0, steps = 0;

    while( i < 100) {
        steps = steps + 1;

        int c, d;
        c = rand() % (n/2) + 1;

        if(c == (n/2))
            d = 1;
        else
            d = c+1;
        
        int delta;
        //compute delta = D(room(c,1)....)....

        if(delta < 0) {
            //swap room(c1,1) and room(d1,1)
            //room([c,d],1) = room([d,c],1)
            //Translate this
            cost = cost + delta;
            i = 0;
        } else {
            i = i+1;
        }
    }
}