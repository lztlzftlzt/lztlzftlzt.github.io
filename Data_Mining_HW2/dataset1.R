install.packages("class")    #installing needed packages
install.packages("lattice")
install.packages("caret")
install.packages("cluster")
install.packages("ggplot2")
install.packages("dbscan")
data<-read.csv(file.choose(),sep=',',header=TRUE) #use file.choose() to choose the .csv file conveniently
data1<-data[,-4]
labels <- data$cluster

library("class")              #loading needed libraries
library("cluster")
library("ggplot2")
library("lattice")
library("caret")
library("dbscan")

myHier<-function(data1)     #use Hierarchical clustering
{
  hc<-hclust(d=dist(data1, method="euclidean"), method="ward.D", members=NULL)
  plot(hc)
  rehc<-rect.hclust(hc,k=8)
  rhc<-cutree(hc, k=8)
  confusionMatrix(rhc,data$cluster)
}

myKmeans<-function(data1)  #use K-means for clustering
{
  kc<-kmeans(data1, 8);
  fitted(kc);
  plot(data1[c("x", "y", "z")], col = kc$cluster, pch = as.integer(data$cluster));
  points(kc$centers[,c("x", "y", "z")], col = 1:8, pch = 8, cex=2);
  confusionMatrix(kc$cluster,data$cluster)
}

myDen<-function(data1)    #use DBSCAN for clustering
{
  ds <- dbscan(x=dist(data1, method="euclidean"), eps=2.3, minPts=21, borderPoints=TRUE) 
  plot(data1[c("x", "y", "z")], col = ds$cluster, pch = as.integer(data$cluster));
  ds$cluster<-ds$cluster+1
  confusionMatrix(ds$cluster,data$cluster)
}

myGraph<-function(data1)    #use SNN for clustering
{
  k <- 13
  nn <- sNNclust(x=dist(data1, method="euclidean"), k = k, eps=9, minPts=8)
  table(as.vector(nn$shared))
  nn
  plot(data1[c("x", "y", "z")], col = nn$cluster, pch = as.integer(data$cluster));
}

myHier(data1)
myKmeans(data1)
myDen(data1)
myGraph(data1)