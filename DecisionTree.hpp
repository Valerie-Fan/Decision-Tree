#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include "Node.hpp"
#include "LinkedList.hpp"
#include <vector>
#include <limits>
#include <map>
#include <set>

// Helper struct for best split result
struct SplitResult {
    int featureIndex;
    double threshold;
    double giniIndex;
    std::vector<std::vector<double>> leftSplit, rightSplit;

    SplitResult() : featureIndex(-1), threshold(0), giniIndex(std::numeric_limits<double>::max()) {}
};

class DecisionTree {
private:
    Node* root;
    std::vector<std::vector<double>> data;
    int minSize;
    
    // Calculate Gini index for a split
    double calculateGiniIndex(const std::vector<std::vector<std::vector<double>>>& subsets) const {
        double totalSize = 0;
        for (const auto& subset : subsets) {
            totalSize += subset.size(); // Total size of all subsets
        }
        
        double giniSplit = 0.0;
        for (const auto& subset : subsets) {
            if (subset.empty()) continue;
            
            double size = subset.size();
            std::map<int, int> countMap;
            for (const auto& item : subset) {
                int label = static_cast<int>(item.back()); // Assuming the label is the last element
                countMap[label]++;
            }
            
            double gini = 1.0;
            for (auto& count : countMap) {
                double probability = count.second / size;
                gini -= probability * probability;
            }
            
            // Weighted Gini index for this subset
            giniSplit += gini * (size / totalSize);
        }
        
        return giniSplit;
    }
    
    
    // Find the best split for a dataset
    SplitResult findBestSplit(const std::vector<std::vector<double>>& subset, std::set<int> usedFeatures) {
        SplitResult bestSplit;
        bestSplit.giniIndex = std::numeric_limits<double>::max(); // Initialize with max double value
        
        unsigned long numFeatures = subset[0].size() - 1; // Assuming the last column is the label
        
        // Iterate over all features
        for (int featureIndex = 0; featureIndex < numFeatures; ++featureIndex) {
            if (usedFeatures.find(featureIndex) != usedFeatures.end()) continue;
            std::set<double> uniqueValues;
            
            // Collect unique values for the feature
            for (const auto& row : subset) {
                uniqueValues.insert(row[featureIndex]);
            }
            
            // Iterate over all unique values to try splitting
            for (double value : uniqueValues) {
                // Split dataset by the current feature and value
                std::vector<std::vector<double>> leftSplit, rightSplit;
                for (const auto& row : subset) {
                    if (row[featureIndex] <= value) {
                        leftSplit.push_back(row);
                    } else {
                        rightSplit.push_back(row);
                    }
                }
                
                // Calculate the weighted Gini index for this split
                std::vector<std::vector<std::vector<double>>> splits = {leftSplit, rightSplit};
                double gini = calculateGiniIndex(splits);
                
                // Update bestSplit if this is the best split found so far
                if (gini < bestSplit.giniIndex) {
                    bestSplit.featureIndex = featureIndex;
                    bestSplit.threshold = value;
                    bestSplit.giniIndex = gini;
                    bestSplit.leftSplit = leftSplit;
                    bestSplit.rightSplit = rightSplit;
                }
            }
        }
        
        return bestSplit;
    }
    
    
    // Recursively split nodes
    void splitNode(Node* node, const std::vector<std::vector<double>>& subset, int depth, std::set<int> usedFeatures) {
        // Criteria for creating a leaf node
        if (subset.size() <= 5) {
            createLeafNode(node, subset);
            return;
        }
        
        // Find the best split for the current subset
        SplitResult bestSplit = findBestSplit(subset, usedFeatures);
        usedFeatures.insert(bestSplit.featureIndex);
        
        // Check if no split could be found or the split does not actually divide the data
        if (bestSplit.leftSplit.empty() || bestSplit.rightSplit.empty()) {
            createLeafNode(node, subset);
            return;
        }
        
        if (bestSplit.leftSplit.size() < 5 || bestSplit.rightSplit.size() < 5){
            createLeafNode(node, subset);
            return;
        }
        
        // Apply the best split to the current node
        node->setFeatureIndex(bestSplit.featureIndex);
        node->setThreshold(bestSplit.threshold);
        node->setGiniIndex(bestSplit.giniIndex);
        
        // Create child nodes
        node->leftChild = new Node();
        node->rightChild = new Node();
        
        // Recursively split child nodes
        splitNode(node->getLeftChild(), bestSplit.leftSplit, depth + 1, usedFeatures);
        splitNode(node->getRightChild(), bestSplit.rightSplit, depth + 1, usedFeatures);
    }
    
    
    
    // Create a leaf node
    void createLeafNode(Node* node, const std::vector<std::vector<double>>& subset) {
        int positives = 0;
        int negatives = 0;
        for (const auto& row : subset) {
            if (row.back() == 1) positives++; // Assuming the label is the last element
            else negatives++;
        }
        node->setN1(positives);
        node->setN2(negatives);
        node->setIsLeaf(true);
        node->setGiniIndex(0); // Leaf nodes have a Gini index of 0 since they are pure
    }
    
    
public:
    DecisionTree(int minLeafSize = 5) : root(nullptr), minSize(minLeafSize) {}
    
    ~DecisionTree() {
        delete root;
    }
    
    // Build the decision tree
    void buildTree(const std::vector<std::vector<double>>& dataset) {
        data = dataset;
        root = new Node();
        std::set<int> usedFeatures;
        splitNode(root, data, 0, usedFeatures);
    }
    
    // Predict the class label for a given sample
    int predict(const std::vector<double>& sample) const {
        const Node* node = root; // Start with the root node
        
        // Traverse the tree until reaching a leaf node
        while (!node->isLeaf) {
            if (sample[node->featureIndex] <= node->threshold) {
                // If the feature value of the sample is less than or equal to the node's threshold,
                // move to the left child.
                node = node->leftChild;
            } else {
                // Otherwise, move to the right child.
                node = node->rightChild;
            }
        }
        
        // Once a leaf node is reached, the classification of the node is returned.
        // The classification decision could be based on the majority of class labels
        // (n1 >= n2 for positive, otherwise negative) within that node.
        // This assumes the leaf node has properties to indicate whether it's classified
        // as positive or negative based on the criteria given in your homework.
        if (node->n1 >= node->n2) {
            return 1; // Positive classification
        } else {
            return 0; // Negative classification
        }
    }
    
    
    // Calculate the accuracy of the tree
    double calculateAccuracyForSpecificCriteria(const Node* node) const {
        // Base case: if the node is null, return 0
        if (node == nullptr) return 0.0;
        
        // If the node is a leaf, calculate its contribution to the accuracy
        if (node->isLeaf) {
            if (node->n1 >= node->n2) {
                // For a positive node, add n1
                return node->n1;
            } else {
                // For a negative node, add n2
                return node->n2;
            }
        }
        
        // Recursively calculate the accuracy contribution from both subtrees
        double leftAccuracy = calculateAccuracyForSpecificCriteria(node->leftChild);
        double rightAccuracy = calculateAccuracyForSpecificCriteria(node->rightChild);
        
        // Combine the accuracy contributions from the left and right subtrees
        return leftAccuracy + rightAccuracy;
    }
    
    double calculateAccuracy() const {
        // Call the recursive function starting from the root
        double totalCorrect = calculateAccuracyForSpecificCriteria(root);
        
        // Assuming the total number of subjects is 640
        const double totalSubjects = 640.0;
        
        // Calculate and return the accuracy
        return totalCorrect / totalSubjects;
    }
    
    
    
    void printTree(const Node* node, std::string indent = "", bool isRoot = true) const {
        // Check if the node is null
        if (!node) return;
        
        std::cout << indent;
        if (isRoot) {
            std::cout << "Root";
        } else {
            std::cout << "|----";
        }
        
        // Print the node details
        if (node->isLeaf) {
            // If it's a leaf node, print its classification
            std::cout << "Leaf: Class = " << ((node->n1 >= node->n2) ? "Positive" : "Negative")
            << " (n1=" << node->n1 << ", n2=" << node->n2 << "), Gini: " << node->giniIndex << std::endl;
        } else {
            // If it's not a leaf node, print the feature and threshold used for splitting
            std::cout << " Node: Feature " << node->featureIndex << " < " << node->threshold
            << ", Gini: " << node->giniIndex << std::endl;
        }
        
        // Recursive calls for the left and right children
        if (node->leftChild != nullptr) {
            printTree(node->leftChild, indent + (isRoot ? "" : "     "), false);
        }
        if (node->rightChild != nullptr) {
            printTree(node->rightChild, indent + (isRoot ? "" : "     "), false);
        }
    }
    
    // Wrapper method to call printTree with the root node
    void printTree() const {
        printTree(root);
    }
    
};
#endif
