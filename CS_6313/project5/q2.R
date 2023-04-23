#########
# SETUP #
#########
set.seed(222) # for reproducibility

alpha <- 0.05
num_sims <- 5000


##########
# PART A #
##########
n <- 5
lambda <- 0.01
mu <- 1/lambda # true population mean

coverage_z <- coverage_pb <- rep(0, num_sims) # Monte Carlo simulation

for (i in 1:num_sims) {
  
  x <- rexp(n, lambda) # generate a sample from the population
  
  # compute sample statistics
  x_bar <- mean(x)
  std <- sd(x)
  
  # compute large-sample z-interval
  z_val <- qnorm(1 - alpha/2)
  std_err <- std / sqrt(n)
  z_int <- c(x_bar - z_val * std_err, x_bar + z_val * std_err)
  
  # compute percentile bootstrap method interval
  pb_int <- quantile(replicate(1000, mean(sample(x, replace=TRUE))), 
                     probs = c(alpha/2, 1-alpha/2))
  
  # check if true population mean is contained in each interval
  coverage_z[i] <- (z_int[1] <= mu) & (mu <= z_int[2])
  coverage_pb[i] <- (pb_int[1] <= mu) & (mu <= pb_int[2])
}

# compute coverage probabilities
coverage_z_prob <- mean(coverage_z)
coverage_pb_prob <- mean(coverage_pb)

# print results
cat("n =", n, "lambda =", lambda, "\n")
cat("Large-sample z-interval coverage probability =", coverage_z_prob, "\n")
cat("Parametric percentile bootstrap method interval coverage probability =", coverage_pb_prob, "\n")


##########
# PART B #
##########

# define the combinations of (n, λ)
n_vals <- c(5, 10, 30, 100)
lambda_vals <- c(0.01, 0.1, 1, 10)

# initialize matrix to store the coverage probabilities
coverage_mat <- matrix(NA, nrow = length(n_vals) * length(lambda_vals), 
                          ncol = 4, 
                          dimnames = list(NULL, c("n", "lambda", "coverage_z_prob", "coverage_pb_prob")))

# Monte Carlo simulation for each combination of (n, λ)
k <- 1

for (n in n_vals) {
  for (lambda in lambda_vals) {
    
    mu <- 1/lambda # true population mean
    
    coverage_z <- coverage_pb <- rep(0, num_sims)
    for (i in 1:num_sims) {
      
      # generate a sample from the population
      x <- rexp(n, lambda)
      
      # compute sample statistics
      x_bar <- mean(x)
      std <- sd(x)
      
      # compute large-sample z-interval
      z_val <- qnorm(1 - alpha/2)
      std_err <- std / sqrt(n)
      z_int <- c(x_bar - z_val * std_err, x_bar + z_val * std_err)
      
      # compute bootstrap percentile method interval
      pb_int <- quantile(replicate(1000, mean(sample(x, replace=TRUE))), 
                         probs = c(alpha/2, 1-alpha/2))
      
      # check if true population mean is contained in each interval
      coverage_z[i] <- (z_int[1] <= mu) & (mu <= z_int[2])
      coverage_pb[i] <- (pb_int[1] <= mu) & (mu <= pb_int[2])
    }
    
    # compute coverage probabilities
    coverage_z_prob <- mean(coverage_z)
    coverage_pb_prob <- mean(coverage_pb)
    
    # store the results in the matrix
    coverage_mat[k, ] <- c(n, lambda, coverage_z_prob, coverage_pb_prob)
    k <- k + 1
  }
}

# print the results
print(coverage_mat)