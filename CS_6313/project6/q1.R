########
# DATA #
########
prostate <- read.csv("prostate_cancer.csv")
summary(prostate)

boxplot(prostate$psa, main="Dist of PSA levels")
boxplot(log(prostate$psa), main="Dist of PSA levels")

psa <- log(prostate$psa)
cancervol <- prostate$cancervol
age <- prostate$age
benpros <- prostate$benpros
vesinv <- prostate$vesinv
weight <- prostate$weight
capspen <- prostate$capspen
gleason <- prostate$gleason

plot(prostate$psa)
plot(prostate$psa ~ prostate$cancervol)
plot(prostate$psa ~ prostate$weight)
plot(prostate$psa ~ prostate$age)
plot(prostate$psa ~ prostate$benpros)
plot(prostate$psa ~ prostate$vesinv)
plot(prostate$psa ~ prostate$capspen)
plot(prostate$psa ~ prostate$gleason)

############
# MODELING #
############
vesinv <- as.factor(prostate$vesinv)
full <- lm(psa ~ cancervol + weight + age + benpros + vesinv + capspen + gleason)

# Forward Selection
null <- lm(psa ~ 1)
reduced <- step(null, scope=list(lower=null, upper=full), direction="forward")

# Exponential Search
prostate_copy = prostate[,-1]
all_models <- regsubsets(psa~., data = prostate_copy)




############
# ANALYSIS #
############
summary(full)

anova(reduced, full)
summary(reduced)

summary(all_models)
summary(all_models)$adjr2
best_adjr2_model = lm(psa ~ cancervol + vesinv + gleason)
summary(best_adjr2_model)


###############
# ASSUMPTIONS #
###############
# Linearity
plot(reduced$fitted.values, resid(reduced))
plot(reduced, which=1)

# Independence [FAILED]
plot(prostate$subject, resid(reduced))

# Normality
qqnorm(resid(reduced))
qqline(resid(reduced))
plot(reduced, which=2)

# Homoscedasticity (Equal Variances)
plot(reduced$fitted.values, sqrt(abs(resid(reduced))))
plot(reduced, which=3)

# Outliers
plot(reduced, which=5)


##############
# PREDICTION #
##############
newdata <- data.frame(cancervol = mean(prostate$cancervol),
                      weight = mean(prostate$weight),
                      age = mean(prostate$age),
                      benpros = mean(prostate$benpros),
                      vesinv = names(which.max(table(prostate$vesinv))),
                      capspen = mean(prostate$capspen),
                      gleason = mean(prostate$gleason))
exp(predict(reduced, newdata))
