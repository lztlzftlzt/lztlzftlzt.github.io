1 install Intellij-Idea and JDK 1.8
2 open project, select //CART_SRC_DATA, this is the project directory.
3 Run the program, Main.java is for training and testing on the whole data, Sample_Main.java is for training and testing on sample data

The filePath of both programs is set to be local directory, so it would be better to put SampleData_Mod.csv and ProjectData.txt in //CART_SRC_DATA,
which is the root directory of the project.

For Sample_Main.java
To change filepath, goto class Sample_Main->main(), String filePath is the path of source data.
To change number of split values, goto class SCart, int NumPartition is the number of split values, set to 4 by default.
To measure the accuracy, all the original dataset is classified on the generated decision tree, to
classify part of the original data, goto class SCart->accuracy_test(), change the startIndex and endIndex
of the test data. Program would test the accuracy of this part of original data.

For Main.java
To change filepath, goto class Main->main(), String filePath is the path of source data.
To change number of split values, goto class Cart, int NumPartition is the number of split values, set to 4 by default.
To determine which part of the whole dataset to be loaded into memory for training and testing, goto class Cart,
the int startLoadIndex is the start index of selected data, int numvalidrows is the number of records to be loaded,
note that startLoadIndex + numvalidrows should be less than or equal to 9075358. 
To measure the accuracy, all the original dataset is classified on the generated decision tree, to
classify part of the original data, goto class Cart->accuracy_test(), change the startIndex and endIndex
of the test data. Program would test the accuracy of this part of original data.
