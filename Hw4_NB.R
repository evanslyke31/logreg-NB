df <- read.csv("titanic_project.csv", header = TRUE)

df$pclass <- factor(df$pclass)
df$survived <- factor(df$survived)
df <- df[!is.na(df$pclass),]
df <- df[!is.na(df$survived),]
df$age[is.na(df$age)] <- median(df$age,na.rm=T)

start <- proc.time()

set.seed(1234)
train <- df[1:900,]
test <- df[901:nrow(df),]

library(e1071)
library(caret)
nb1 <- naiveBayes(survived~pclass+sex+age, data=train)
nb1
p1 <- predict(nb1, newdata=test)
table(p1, test$survived)

acc1 <- mean(p1 == test$survived)                     # calculates the accuracy
print(paste("nb1 accuracy: ", acc1 * 100, "%"))
sensitivity <- sensitivity(table(p1, test$survived)) # calculate sensitivity
print(paste("sensitivity: ",sensitivity))
specificity <- specificity(table(p1, test$survived)) #calculate specificity
print(paste("specificity: ", specificity))

stop <- proc.time()
(stop-start)[3]

head(df)
summary(df)
attach(df)
par(mfrow=c(1,2))
cdplot(survived~age)
cdplot(survived~sex)
cdplot(survived~pclass)

df$survived = as.numeric(df$survived)
df$pclass = as.numeric(df$pclass)
hist(df$age)
hist(df$sex)
hist(df$pclass)
hist(df$survived)
cor(df[1:4], use="complete")
pairs(df[1:4])

write.csv(train, file = "NB_train.csv", row.names = FALSE)      
write.csv(test, file = "NB_test.csv", row.names = FALSE)         

