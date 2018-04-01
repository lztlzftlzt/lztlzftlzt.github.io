Note: The proj1_data.csv should be in the same folder as the R script.
Step1: Open this R script in RStudio, copy the row 1-6 and paste in the Console then press enter, this will
install the 6 needed packages for this project: class, caret, RWeka, e1071, lattice and ggplot2.
Step2: Copy the row 7-73 and paste in the Console then press enter, a prompt window would appear, choose proj1_data.csv as the input file. 
Row 7-24 will create 5 different pairs of training set and test set£¬row 25-30 will load the needed libraries into working space. 
Row 32-73 are 4 functions, each corresponds to an algorithm.
Step3: In the Console, call the function in the format like "myKnn(dataset1)", "myRipper(dataset1)", "myC45(dataset1)",
"mySVM(dataset1)". Chose one of the five pairs of datasets as the parameter. Then the function would output
the Confusion Matrix and statistics data.