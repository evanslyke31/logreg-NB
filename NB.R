
# Jay Patel, Erik Van Slyke
# CS 4375 Homework 4
# Naive Bayes


df <- read.csv("titanic_project.csv", header = TRUE) # Read the csv file

df$pclass <- factor(df$pclass) # Make class as factor
df$survived <- factor(df$survived) # make survived as factor
df <- df[!is.na(df$pclass),]
df <- df[!is.na(df$survived),]
df$age[is.na(df$age)] <- median(df$age,na.rm=T)

start <- proc.time() # start the timer

# define train and test dataset
set.seed(1234)
train <- df[1:900,]
test <- df[901:nrow(df),]

library(e1071)
library(caret)
nb1 <- naiveBayes(survived~pclass+sex+age, data=train) # make the naive bayes model
nb1
p1 <- predict(nb1, newdata=test) # predict nb1 using test data
table(p1, test$survived)
confusionMatrix(p1, test$survived)   # output confusion matrix with all information
acc1 <- mean(p1 == test$survived)                     # calculates the accuracy
print(paste("nb1 accuracy: ", acc1 * 100, "%"))
sensitivity <- sensitivity(table(p1, test$survived)) # calculate sensitivity
print(paste("sensitivity: ",sensitivity))
specificity <- specificity(table(p1, test$survived)) #calculate specificity
print(paste("specificity: ", specificity))

stop <- proc.time()
(stop-start)[3]   # output time elapsed

# data exploration
head(df)
summary(df)
# show cd plots
par(mfrow=c(1,2))
cdplot(survived~age)
cdplot(survived~sex)
cdplot(survived~pclass)

# make survived and pclass as numeric to make histogram
df$survived = as.numeric(df$survived) 
df$pclass = as.numeric(df$pclass)
hist(df$age)
hist(df$sex)
hist(df$pclass)
hist(df$survived)
cor(df[1:4], use="complete") # output the coorelation
pairs(df[1:4])

# write train test to csv for c++ part
write.csv(train, file = "NB_train.csv", row.names = FALSE)      
write.csv(test, file = "NB_test.csv", row.names = FALSE)         

