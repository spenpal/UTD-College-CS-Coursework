# Read the dataset
data <- read.csv("roadrace.csv")


### PART A ###

# Create a table of Maine vs. Away
maine_table <- table(data$Maine)

# Create a bar plot
barplot(maine_table, 
        main = "Number of Runners from Maine vs. Away", 
        xlab = "Location", 
        ylab = "Count")

# Summary statistics for the Maine variable
data$Maine <- factor(data$Maine, levels = c("Maine", "Away"))
summary(data$Maine)


### PART B ###

# subset the data by Maine and Away
data_maine <- subset(data, Maine == "Maine")
data_away <- subset(data, Maine == "Away")

# Convert "Time (minutes)" column to numeric
data_maine$Time..minutes. <- as.numeric(data_maine$Time..minutes.)
data_away$Time..minutes. <- as.numeric(data_away$Time..minutes.)

# create histograms of the runners' times
hist(data_maine$Time..minutes., 
     breaks = seq(25, 155, by = 5), 
     col = "blue", 
     main = "Histogram of Runners' Times - Maine", 
     xlab = "Time (minutes)")
hist(data_away$Time..minutes., 
     breaks = seq(25, 155, by = 5), 
     col = "red", 
     main = "Histogram of Runners' Times - Away", 
     xlab = "Time (minutes)")

# Summary statistics for the Maine group
summary(data_maine$Time..minutes.)
mean(data_maine$Time..minutes.)
sd(data_maine$Time..minutes.)
range(data_maine$Time..minutes.)
median(data_maine$Time..minutes.)
IQR(data_maine$Time..minutes.)

# Summary statistics for the Away group
summary(data_away$Time..minutes.)
mean(data_away$Time..minutes.)
sd(data_away$Time..minutes.)
range(data_away$Time..minutes.)
median(data_away$Time..minutes.)
IQR(data_away$Time..minutes.)


### PART C ###

# Create boxplots of times for Maine and Away groups
boxplot(data_maine$Time..minutes., 
        data_away$Time..minutes.,
        names=c("Maine", "Away"), 
        xlab="Group", 
        ylab="Time (minutes)",
        main = "Boxplots of Runner's Times by Location")


### PART D ###

# Subset data by gender
data_male <- subset(data, Sex == "M")
data_female <- subset(data, Sex == "F")

# Create side-by-side boxplots
boxplot(data_male$Age, 
        data_female$Age, 
        names = c("Male", "Female"), 
        xlab = "Gender", 
        ylab = "Age", 
        main = "Boxplot of Runner's Age by Gender")
