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
