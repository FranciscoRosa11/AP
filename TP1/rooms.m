function [room, cost, steps] = rooms(D)

% Randomly tries to find the best distribution of n students pair-wise 
% to n/2 rooms; D is a symmetric matrix, D(i,j)=D(j,i) represents how
% much students i and j dislike each other;
% room is an array with n/2 rows (rooms) and 2 colums (2 students per
% room); room(i,1) and room(i,2) identify the two occupants of ith room;
% cost is the sum of incompatibilities of a distribution 


% FIRST, assign rooms to students in a random way 
n=length(D);       % number of students 
% occupants of the ith room (i=1,...,n) are room(i,1) and room(i,2)
room=reshape(randperm(n),[],2);% random permutation of [1 2...n]

% Compute the value of cost for the initial distribution
cost=0;
for i=1:n/2
    cost=cost+D(room(i,1),room(i,2));
end    
     
i=0; steps=0;
while i < 100  % stop if no changes for 100 trials
% Randomly, choose a room and move one occupant to next room to minimize cost
    steps=steps+1; 
    c=randi(n/2); % randomly choose a room (integer between 1 and n/2) 
    % room d is next to room c; d=1 if c=n
    if c==n/2
        d=1;
    else
        d=c+1;
    end
    % Difference in cost,assuming first occupants in rooms c and d permute
    delta=D(room(c,1),room(d,2))+D(room(d,1),room(c,2))-...
          D(room(c,1),room(c,2))-D(room(d,1),room(d,2));
    % accept or discard new arrangement 
    if delta<0 
    % swap room(c1,1) and room(d1,1)
        room([c,d],1)=room([d,c],1); 
        cost=cost+delta; 
        i=0;
    else i=i+1;
    end
end
