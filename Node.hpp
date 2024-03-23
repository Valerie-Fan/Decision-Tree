#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

class DecisionTree;

class Node {
    friend class DecisionTree;
    friend class LinkedList;

private:
    Node* leftChild; // Pointer to the left child
    Node* rightChild; // Pointer to the right child
    int featureIndex; // Index of the feature used for splitting at this node
    double threshold; // Threshold value for the split
    int n1; // Number of positive cases in the leaf node
    int n2; // Number of negative cases in the leaf node
    double giniIndex; // Gini index at this node
    bool isLeaf; // Flag indicating if this node is a leaf

public:
    // Constructor
    Node() : leftChild(nullptr), rightChild(nullptr), featureIndex(-1), threshold(0.0),
             n1(0), n2(0), giniIndex(0.0), isLeaf(false) {}

    // Destructor
    ~Node() {
        delete leftChild;
        delete rightChild;
    }

    // Setters and getters
    void setFeatureIndex(int index) { featureIndex = index; }
    void setThreshold(double thres) { threshold = thres; }
    void setN1(int count) { n1 = count; }
    void setN2(int count) { n2 = count; }
    void setGiniIndex(double gini) { giniIndex = gini; }
    void setIsLeaf(bool leaf) { isLeaf = leaf; }
    void setLeftChild(Node* child) {
        leftChild = child;
    }
    void setRightChild(Node* child) {
        rightChild = child;
    }

    int getFeatureIndex() const { return featureIndex; }
    double getThreshold() const { return threshold; }
    int getN1() const { return n1; }
    int getN2() const { return n2; }
    double getGiniIndex() const { return giniIndex; }
    bool getIsLeaf() const { return isLeaf; }
    Node* getLeftChild() const {
        return leftChild;
    }
    Node* getRightChild() const {
        return rightChild;
    }
};

#endif
