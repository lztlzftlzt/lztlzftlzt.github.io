install.packages("class")    #installing needed packages
install.packages("lattice")
install.packages("caret")
install.packages("cluster")
install.packages("ggplot2")
install.packages("fpc")
install.packages("dbscan")
install.packages("hydroGOF")
data<-read.csv(file.choose(),sep=',',header=TRUE) #use file.choose() to choose the .csv file conveniently

library("class")              #loading needed libraries
library("cluster")
library("ggplot2")
library("lattice")
library("caret")
library("fpc")
library("dbscan")
library("hydroGOF")

myKmeans<-function(data)  #use K-means for clustering
{
  kc<-kmeans(data, 10);
  fitted(kc);
  kc;
  rmse(data,kc$cluster);
}
myKmeans(data)
