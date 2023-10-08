% Database
female(anneRoyal).
female(autumnPhillips).
female(beatriceOfYork).
female(camillaOfCornwall).
female(catherineOfCambridge).
female(charlotteOfCambridge).
female(dianaOfWales).
female(euginieOfYork).
female(isla).
female(lena).
female(louiseWindsor).
female(meghanOfSussex).
female(mia).
female(queenElizabeth).
female(sarahOfYork).
female(savannah).
female(sophieOfWessex).
female(zaraTindal).

male(andrewOfYork).
male(charlesOfWales).
male(edwardOfWessex).
male(georgeOfCambridge).
male(harryOfSussex).
male(louisOfCambridge).
male(philipOfEdinburgh).
male(williamOfCambridge).

married(philipOfEdinburgh, queenElizabeth).
married(queenElizabeth, philipOfEdinburgh).

parent(andrewOfYork, beatriceOfYork).
parent(andrewOfYork, euginieOfYork).
parent(anneRoyal, peterPhillips).
parent(anneRoyal, zaraTindall).
parent(camillaOfCornwall, harryOfSussex).
parent(camillaOfCornwall, williamOfCambridge).
parent(catherineOfCambridge, charlotteOfCambridge).
parent(catherineOfCambridge, georgeOfCambridge).
parent(catherineOfCambridge, louisOfCambridge).
parent(charlesOfWales, harryOfSussex).
parent(charlesOfWales, williamOfCambridge).
parent(dianaOfWales, harryOfSussex).
parent(dianaOfWales, williamOfCambridge).
parent(edwardOfWessex, jamesSevern).
parent(edwardOfWessex, louiseWindsor).
parent(harryOfSussex, archie).
parent(meghanOfSussex, archie).
parent(mikeTindall, lena).
parent(mikeTindall, mia).
parent(peterPhillip, isla).
parent(peterPhillip, savannah).
parent(philipOfEdinburgh, andrewOfYork).
parent(philipOfEdinburgh, anneRoyal).
parent(philipOfEdinburgh, charlesOfWales).
parent(philipOfEdinburgh, edwardOfWessex).
parent(queenElizabeth, andrewOfYork).
parent(queenElizabeth, anneRoyal).
parent(queenElizabeth, charlesOfWales).
parent(queenElizabeth, edwardOfWessex).
parent(williamOfCambridge, charlotteOfCambridge).
parent(williamOfCambridge, georgeOfCambridge).
parent(williamOfCambridge, louisOfCambridge).
parent(zaraTindall, lena).
parent(zaraTindall, mia).
%-------------------------------------------------------------------------------------------
% Part A
mother(X, Y) :- parent(X, Y), female(X).

% Part B
father(X, Y) :- parent(X, Y), male(X).

% Part C, D
sibling(X, Y) :- mother(A, X), mother(B, Y), father(C, X), father(D, Y), A==B, C==D.
brother(X, Y) :- sibling(X, Y), male(X), not(X==Y).
sister(X, Y) :- sibling(X, Y), female(X), not(X==Y).

% Part E, F
grandfather(X, Y) :- parent(A, Y), parent(X, A), male(X).
grandmother(X, Y) :- parent(A, Y), parent(X, A), female(X).

% Part G
children(X, Y):- parent(Y, X).

% Part H, I
step_child(X, Y) :- married(Y, B), parent(B, X), not(parent(Y, X)).
step_daughter(X, Y) :- step_child(X, Y), female(X).
step_son(X, Y) :- step_child(X, Y), male(X).

% Part J
great_grandfather(X, Y):- grandfather(A, Y), parent(X, A), male(X).

% Part K
num_females(X) :- findall(Y, female(Y), Z), count(Z, X).

% Part L, M, N
count([], 0).
count([H|T], Y) :- count(H, Z), count(T, A), Y is Z + A.
count(K, 1) :- atom(K).
num_children(X, Z) :- male(X), findall(K, parent(X, K), L1), count(L1, A), Z==A.
num_children(X, Z) :- female(X), findall(K, parent(X, K), L1), count(L1, A), Z==A.

only_one_child(X):- num_children(X, 1).
two_children(X) :- num_children(X, 2).
at_least_two_children(X) :- male(X), findall(K, parent(X, K), L1), count(L1, A), 1<A.
at_least_two_children(X) :- female(X), findall(K, parent(X, K), L1), count(L1, A), 1<A.