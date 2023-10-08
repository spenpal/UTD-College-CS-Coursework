delements([_,_|A], P) :- length(A, Y), delements(A, Y, P).
delements([X|A], Y, [X|B]) :- L is Y - 1, delements(A, L, B).
delements(_, Y, []) :- Y == 2.


