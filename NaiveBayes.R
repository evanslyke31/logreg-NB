df <- titanic

df$pclass <- factor(df$pclass)
df$survived <- factor(df$survived)
df <- df[!is.na(df$pclass),]
df <- df[!is.na(df$survived),]
df$age[is.na(df$age)] <- median(df$age,na.rm=T)

set.seed(1234)
train <- df[1:900,]
test <- df[901:nrow(df),]

start <- proc.time()

library(e1071)
library(caret)
nb1 <- naiveBayes(survived~pclass+sex+age, data=train)
nb1
p1 <- predict(nb1, newdata=test)
table(p1, test$survived)

acc1 <- mean(p1 == test$survived)                     # calculates the accuracy
print(paste("nb1 accuracy: ", acc1))
sens <- sensitivity(table(p1, test$survived)) # calculate sensitivity
print(paste("sensitivity: ",sens))
spec <- specificity(table(p1, test$survived)) #calculate specificity
print(paste("specificity: ", spec))
stop <- proc.time()
(stop-start)[3]

