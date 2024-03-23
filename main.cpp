#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "CSVReader.hpp"
#include "DecisionTree.hpp"
#include <iostream>

int main() {
    // Specify the path to your CSV dataset
    std::string dataPath = "Diagnosis_7features.csv";
    CSVReader reader(dataPath);

    // Load the dataset
    std::vector<std::vector<double>> dataset = reader.getData();

    // Initialize and build the decision tree
    DecisionTree tree;
    tree.buildTree(dataset);

    // Print the tree structure
    std::cout << "Decision Tree Structure: " << std::endl;
    tree.printTree();

    // Calculate and print the accuracy of the decision tree
    double accuracy = tree.calculateAccuracy();
    std::cout << "Accuracy of the Decision Tree: " << accuracy * 100 << "%" << std::endl;

    return 0;
}
