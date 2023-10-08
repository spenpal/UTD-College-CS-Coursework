% Edge Cases
elements([], 0).
elements(L, 1) :- atom(L).

% Normal Case
elements([A|B], K) :- elements(A, X), elements(B, Y), K is X + Y.
