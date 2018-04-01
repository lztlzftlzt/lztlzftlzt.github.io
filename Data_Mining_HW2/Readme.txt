Note: The dataset1.csv and dataset2.csv should be in the same folder as the R scripts.
dataset1.R is for the clustering of dataset1. And dataset2.R is for the clustering of dataset2.

For dataset1.R:
Step1: Open this R script in RStudio, copy the row 1-6 and paste in the Console then press enter, this will
install the 6 needed packages for this project: class, lattice, caret, cluster, ggplot2 and dbscan.
Step2: Copy the row 7-51 and paste in the Console then press enter, a prompt window would appear, choose dataset1.csv as the input file. 
row 11-16 will load the needed libraries into working space. 
Row 18-51 are 4 functions, each corresponds to an algorithm.
Row 53-56 are the usage of these functions.
Step3: In the Console, call the function in the format like "myHier(data1)", "myKmeans(data1)", "myDen(data1)"(DBSCAN),
"myGraph(data1)"(SNN). Then the function would output the Confusion Matrix and clustering results as well as the plots.

For dataset2.R:
Step1: Open this R script in RStudio, copy the row 1-8 and paste in the Console then press enter, this will
install the 8 needed packages for this project: class, lattice, caret, cluster, ggplot2, fpc, dbscan and hydroGOF.
Step2: Copy the row 9-26 and paste in the Console then press enter, a prompt window would appear, choose dataset2.csv as the input file. 
row 11-18 will load the needed libraries into working space. 
Row 20-26 is the k-means clustering function.
Row 27 is the usage of k-means.
Step3: In the Console, call the function in the format like "myKmeans(data)"Then the function would output the clustering results 
and sum of internal root-mean-square-deviation (RMSD).
