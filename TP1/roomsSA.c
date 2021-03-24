#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

/*
    Assign rooms to students in a random way

    n = length of input matrix
    room = randpermutation of [1..n]
    room = reshape(room, [], 2);
*/

int cost = 0;
int i = 0;
for(i = 0; i <= n/2; i++)
    cost = cost; // + inputMatrix(room(i,1),room(i,2))

i = 0;
int steps = 0, T = 1;

while(i < 100) {
    steps = steps + 1;
    int c = rand() % (n/2) + 1;
    int d;

    if(c == (n/2))
        d = 1;
    else
        d = c + 1;

    int delta;

    //delta = difference in cost 
    //delta = D(room(c,1).....) + ....

    if(delta < 0 || exp(-delta / T) >= (rand() % 0 + 1)) { // not really sure on this rand() % 0 + 1 
        //swap room(c1,1) and room(d1,1)
        //room([c,d], 1) = room([d,c], 1);
        cost = cost + delta;
        i = 0;
    } else {
        i = i+1;
    }

    T = 0.999*T;
}

}