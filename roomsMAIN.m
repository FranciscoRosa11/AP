function roomsMAIN(n,procs)

% This is a simulation of a parallel execution on p processors of the code
% "rooms" which uses simulated annealing to distribute n students pair-wise 
% to n/2 rooms. Each processor executes the same code with different random
% numbers and, in general, will find different answers with different
% costs. The parallel solution is thus the answer with smaller cost.

% Generates D, symmetric, of integer numbers between 0 and 10
D=randi(10,n) % a random matrix (size n) of integers from 1 to 10
D=(D+D')/2; % symmetrix matrix of incompatibilities

for p=1:procs
    [~, cost(p), steps(p)]=rooms(D);
    [~, costSA(p) stepsSA(p)]=roomsSA(D);
end
steps
stepsSA
plot(cost,'o'), hold on, plot(costSA,'r+'), hold off
legend('cost of MC', 'cost of SA')
