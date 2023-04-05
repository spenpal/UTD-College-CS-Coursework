# Load the data from the CSV file
data <- read.csv("VOLTAGE.csv")


# ----
# PART A

# Create histograms of the voltage readings for each location using the hist() function
par(mfrow = c(1, 2)) # create a 1x2 grid of plots
hist(data$voltage[data$location == 0], 
     col = "blue", 
     main = "Remote Location", 
     xlab = "Voltage Reading", 
     ylab = "Frequency")
hist(data$voltage[data$location == 1], 
     col = "red", 
     main = "Local Location", 
     xlab = "Voltage Reading", 
     ylab = "Frequency")

dev.off()

# create box plots
boxplot(voltage ~ location, 
        data = data, 
        xlab = "Location", 
        ylab = "Voltage", 
        main = "Voltage Readings by Location")
# add mean points
means <- tapply(data$voltage, data$location, mean)
points(x = 1:2, y = means, col = "red", pch = 20)


# ----
# PART B

# Perform F-test to check for equal population variances
var.test(voltage ~ location, data = data)

# Calculate the sample means and standard deviations for each location
mean_remote <- mean(data$voltage[data$location == 0])
sd_remote <- sd(data$voltage[data$location == 0])
mean_local <- mean(data$voltage[data$location == 1])
sd_local <- sd(data$voltage[data$location == 1])

# Calculate the pooled standard deviation
n_remote <- length(data$voltage[data$location == 0])
n_local <- length(data$voltage[data$location == 1])
s_pooled <- sqrt(((n_remote - 1) * sd_remote^2 + (n_local - 1) * sd_local^2) / (n_remote + n_local - 2))

# Calculate the standard error of the difference in means
se_diff <- s_pooled * sqrt(1/n_remote + 1/n_local)

# Calculate the t-statistic and the confidence interval
t_val <- qt(0.975, n_remote + n_local - 2)
ci <- c(mean_remote - mean_local - t_val * se_diff, mean_remote - mean_local + t_val * se_diff)

# Print the confidence interval
cat("95% Confidence Interval for the Difference in Means:", round(ci, 3))
