install.packages("class")    #installing needed packages
install.packages("caret")
install.packages("RWeka")
install.packages("e1071")
install.packages("lattice")
install.packages("ggplot2")
data<-read.csv(file.choose(),sep=',',header=TRUE) #use file.choose() to choose the .csv file conveniently
data<-data[,-1]
data<-data[,-1]
n=nrow(data)                  #read data and omit "rank" and "entity" column
divideDataset<-function(seed) #function to divide original dataset into training set and test set 
{
  set.seed(seed)
  trainindex<-sample(1:n,round(n*0.8))
  trainset<-data[trainindex,]
  testset<-data[-trainindex,]
  output<-list(train=trainset,test=testset)
  return(output)
}                           
dataset1<-divideDataset(1000) #set 5 different rand-num generator seeds to generate 5 different
dataset2<-divideDataset(2000) #pairs of training set and test set
dataset3<-divideDataset(3000)
dataset4<-divideDataset(4000)
dataset5<-divideDataset(5000) 
library("class")              #loading needed libraries
library("caret")
library("RWeka")
library("e1071")
library("lattice")
library("ggplot2")

myKnn<-function(dataset)     #use KNN for prediction
{
  trainset=dataset$train
  testset=dataset$test
  train_labels <- trainset$Continent
  test_labels <- testset$Continent
  test_pred_labels<-knn(trainset[,-4], testset[,-4], train_labels, k=8)
  confusionMatrix(test_pred_labels,test_labels)
}

myRipper<-function(dataset)  #use Ripper for prediction
{
  trainset=dataset$train
  testset=dataset$test
  train_labels <- trainset$Continent
  test_labels <- testset$Continent
  classifier<-JRip(trainset$Continent~., trainset)
  test_pred_labels=predict(classifier, newdata = testset)
  confusionMatrix(test_pred_labels,testset$Continent)
}

myC45<-function(dataset)    #use C4.5 for prediction
{
  trainset=dataset$train
  testset=dataset$test
  train_labels <- trainset$Continent
  test_labels <- testset$Continent
  classifier<-J48(as.factor(trainset$Continent)~., trainset)
  test_pred_labels=predict(classifier, newdata = testset)
  confusionMatrix(test_pred_labels,testset$Continent)
}

mySVM<-function(dataset)    #use SVM for prediction
{
  trainset=dataset$train
  testset=dataset$test
  train_labels <- trainset$Continent
  test_labels <- testset$Continent
  classifier<-svm(as.factor(trainset$Continent)~., trainset)
  test_pred_labels=predict(classifier, newdata = testset)
  confusionMatrix(test_pred_labels,testset$Continent)
}

myKnn(dataset1)
myRipper(dataset1)
myC45(dataset1)
mySVM(dataset1)


