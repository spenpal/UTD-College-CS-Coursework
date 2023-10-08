% Database
cases(a, 20).
cases(b, 39).
cases(c, 310).
cases(d, 307).
cases(e, 434).
cases(f, 200).

deaths(a, 0).
deaths(b, 2).
deaths(c, 4).
deaths(d, 6).
deaths(e, 23).
deaths(f, 24).

hospitalized(a, 5).
hospitalized(b, 6).
hospitalized(c, 22).
hospitalized(d, 55).
hospitalized(e, 79).
hospitalized(f, 80).
%-----------------------------------------------------------------------------------

% Part A
total_cases(X) :- findall(Y, cases(_, Y), L), sum_list(L, X).

% Part B
less_than_hundred_cases(X) :- cases(X, Y), Y < 100.

% Part C
mymax([], 0).
mymax([Head | Tail], Max) :-
    mymax(Tail, TailMax),
    Head > TailMax,
    Max is Head.
mymax([Head | Tail], Max) :-
    mymax(Tail, TailMax),
    Head =< TailMax,
    Max is TailMax.

mymin([], 0).
mymin([Head | Tail], Max) :-
    mymax(Tail, TailMax),
    Head =< TailMax,
    Max is Head.
mymin([Head | Tail], Max) :-
    mymax(Tail, TailMax),
    Head > TailMax,
    Max is TailMax.

highest_deaths(X) :- findall(Y, deaths(_, Y), L), mymax(L, Z), deaths(X, Z).
highest_cases(X) :- findall(Y, cases(_, Y), L), mymax(L, Z), cases(X, Z).

% Part D
more_than_100_cases_less_than_five_deaths(X) :- cases(X, A), deaths(X, B), B < 5, A > 100.

% Part E
largest_cases_or_death(X) :- highest_deaths(X).
largest_cases_or_death(X) :- highest_cases(X).

% Part F
lowest_deaths(X) :- findall(Y, deaths(_, Y), L), mymin(L, Z), deaths(X, Z).
lowest_cases(X) :- findall(Y, cases(_, Y), L), mymin(L, Z), cases(X, Z).
lowest_hospitalized(X) :- findall(Y, hospitalized(_, Y), L), mymin(L, Z), hospitalized(X, Z).
lowest(X) :- lowest_deaths(X), lowest_cases(X), lowest_hospitalized(X). 

% Part G
cases_A_B_C(X) :- cases(a, A), cases(b, B), cases(c, C), X is A + B + C.

% Part H
more_deaths_than_D(X) :- deaths(d, A), deaths(X, B), not(d == X), B > A.

% Part I, J
more_cases_and_deaths_than_D(X) :- deaths(d, A), deaths(X, B), not(d == X), cases(d, C), cases(X, D), B > A, C > D.
more_cases_or_deaths_than_D(X) :- more_deaths_than_D(X).
more_cases_or_deaths_than_D(X) :- cases(d, A), cases(X, B), not(d == X), A > B.

% Part K
average_deaths(X) :- findall(Z, deaths(y, Z), L), sum_list(L, A), length(L, B), X is A / B.

% Part L
more_than_20_less_than_60_hospitalized(X) :- hospitalized(X, Y), Y > 20, Y < 60.

% Part M
less_than_20_hospitalized(X) :- hospitalized(X, Y), Y < 20.

% Part N
no_deaths(X) :- deaths(X, 0).