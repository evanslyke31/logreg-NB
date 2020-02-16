
# Jay Patel, Erik Van Slyke
# CS 4375 Homework 4
# logistic regression


library(HSAUR)   # load HSAUR library to get plasma data
data(plasma)

# make logistic model for ESR and fibrinogen using plasma data
glm1 <- glm(ESR~fibrinogen, data=plasma, family=binomial)  
summary(glm1)

# define the sigmoid fumction
sigmoid <- function(z){
  1.0 / (1+exp(-z))
}

start <- proc.time()   # start timer

# set up weight vector, label vector, and data matrix
weights <- c(1, 1)     # initialize w0 and w1 to 1
data_matrix <- cbind(rep(1, nrow(plasma)), plasma$fibrinogen)
labels <- as.integer(plasma$ESR) - 1

learning_rate <- 0.001
# Gradient Descent from Scratch. Three steps per iteration.
for (i in 1:500000){
  prob_vector <- sigmoid(data_matrix %*% weights)
  error <- labels - prob_vector
  weights <- weights + learning_rate * t(data_matrix) %*% error
}
stop <- proc.time() # stop timer
weights   # ouput the weights
(stop-start)[3]

# graph to show LogOdds is a Linear Combination of the Parameters
plasma_log_odds <- cbind(rep(1, 32), plasma$fibrinogen) %*% weights
plot(plasma$fibrinogen, plasma_log_odds, col=plasma$ESR)
abline(weights[1], weights[2])

# data exploration

par(mfrow=c(1,2))
plot(ESR, fibrinogen, data=plasma, main="Fibrinogen", varwidth=TRUE)
plot(ESR, globulin, data=plasma, main="Globulin", varwidth=TRUE)
cdplot(ESR~fibrinogen)
cdplot(ESR~globulin)

head(plasma)
summary(plasma)
mean(plasma$globulin, na.rm=TRUE)      # mean for globulin
mean(plasma$fibrinogen, na.rm=TRUE)    # mean for fibrinogen
hist(plasma$fibrinogen)
hist(plasma$globulin)
cor(plasma[1:2], use="complete") # output the coorelation
pairs(plasma[1:2])
plot(plasma$fibrinogen, plasma$globulin, pch=16, col="blue", cex=1.5,
     main="Airquality", xlab="Ozone", ylab="Temperature")    
