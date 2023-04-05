# Define the log-likelihood function
neg_log_likelihood <- function(x, theta) {
  n <- length(x)
  sum_log_x <- sum(log(x))
  -(n * log(theta) - (theta + 1) * sum_log_x)
}

# Run optim to maximize the log-likelihood function
x <- c(21.72, 14.65, 50.42, 28.78, 11.23)
ml.est <- optim(par = 1, fn = neg_log_likelihood, x = x, method = "L-BFGS-B", lower = 0.001, upper = 10, hessian=TRUE)
theta_hat <- ml.est$par
hessian <- ml.est$hessian

# Compute the standard error and confidence interval
se <- sqrt(diag(solve(hessian)))
ci <- theta_hat + c(-1, 1) * 1.96 * c(se, se)

# Print the results
cat("Maximum likelihood estimate:", theta_hat, "\n")
cat("Standard error:", se, "\n")
cat("95% confidence interval:", ci, "\n")

qnorm(0.975)
