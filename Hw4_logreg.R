library(HSAUR)
data(plasma)

glm1 <- glm(ESR~fibrinogen, data=plasma, family=binomial)
summary(glm1)

sigmoid <- function(z){
  1.0 / (1+exp(-z))
}

start <- proc.time()
weights <- c(1, 1)
data_matrix <- cbind(rep(1, nrow(plasma)), plasma$fibrinogen)
labels <- as.integer(plasma$ESR) - 1

learning_rate <- 0.001
for (i in 1:500000){
  prob_vector <- sigmoid(data_matrix %*% weights)
  error <- labels - prob_vector
  weights <- weights + learning_rate * t(data_matrix) %*% error
}
stop <- proc.time()
weights
(stop-start)[3]

attach(plasma)
par(mfrow=c(1,2))
plot(ESR, fibrinogen, data=plasma, main="Fibrinogen", varwidth=TRUE)
plot(ESR, globulin, data=plasma, main="Globulin", varwidth=TRUE)
cdplot(ESR~fibrinogen)
cdplot(ESR~globulin)

head(plasma)
summary(plasma)
mean(plasma$globulin, na.rm=TRUE)
mean(plasma$fibrinogen, na.rm=TRUE)
hist(plasma$fibrinogen)
hist(plasma$globulin)
cor(plasma[1:2], use="complete")
pairs(plasma[1:2])
plot(plasma$fibrinogen, plasma$globulin, pch=16, col="blue", cex=1.5,
     main="Airquality", xlab="Ozone", ylab="Temperature")    
