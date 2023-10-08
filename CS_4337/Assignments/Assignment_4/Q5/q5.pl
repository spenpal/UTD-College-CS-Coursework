color(red).
color(blue).
color(yellow).
color(green).

adjacent(X, Y) :- color(X), color(Y), X \= Y.

map(Austin, Brazoria, Chambers, Colorado, FortBend, Galveston, Harris, Liberty, Matagorda, Montgomery, Walker, Waller, Wharton) :-
    adjacent(Austin, Colorado), adjacent(Austin, FortBend), adjacent(Austin, Waller), adjacent(Austin, Wharton), 
    adjacent(Brazoria, FortBend), adjacent(Brazoria, Harris), adjacent(Brazoria, Galveston), adjacent(Brazoria, Wharton), adjacent(Brazoria, Matagorda),
    adjacent(Chambers, Liberty), adjacent(Chambers, Harris),
    adjacent(Colorado, Austin), adjacent(Colorado, Wharton),
    adjacent(FortBend, Austin), adjacent(FortBend, Waller), adjacent(FortBend, Harris), adjacent(FortBend, Brazoria), adjacent(FortBend, Wharton),
    adjacent(Galveston, Brazoria), adjacent(Galveston, Harris),
    adjacent(Harris, Montgomery), adjacent(Harris, Liberty), adjacent(Harris, Chambers), adjacent(Harris, Galveston), adjacent(Harris, Brazoria), adjacent(Harris, FortBend), adjacent(Harris, Waller),
    adjacent(Liberty, Montgomery), adjacent(Liberty, Harris), adjacent(Liberty, Chambers), 
    adjacent(Matagorda, Wharton), adjacent(Liberty, Brazoria),
    adjacent(Montgomery, Walker), adjacent(Montgomery, Waller), adjacent(Montgomery, Harris), adjacent(Montgomery, Liberty), 
    adjacent(Walker, Montgomery), 
    adjacent(Waller, Austin), adjacent(Waller, FortBend), adjacent(Waller, Harris), adjacent(Waller, Montgomery), 
    adjacent(Wharton, Colorado), adjacent(Wharton, Austin), adjacent(Wharton, FortBend), adjacent(Wharton, Brazoria), adjacent(Wharton, Matagorda).
