set.seed(123) # for reproducibility
n = 10000 # number of replications

# simulate n random points in a unit square
x = runif(n, min = 0, max = 1)
y = runif(n, min = 0, max = 1)

# calculate the distance of each point from the center (0.5, 0.5)
d = sqrt((x - 0.5)^2 + (y - 0.5)^2)

# count the number of points inside the circle (distance < 0.5)
count = sum(d < 0.5)

# estimate the probability of a point falling inside the circle
p = count / n

# use the relation between π and the probability to estimate π
pi_estimate = 4 * p

# print the estimate of π
print(pi_estimate)