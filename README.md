# Decision-Tree

## Overview
The C++ version used for this project is C++20, ensuring modern features and optimizations are leveraged for efficient processing and analysis.

## Features
- **Gini Index Calculation**: Utilizes the Gini index as the cost function for decision tree construction.
- **Linked List Tree Structure**: Employs a linked list to construct the binary decision tree, with each node having two child nodes.
- **Overfitting Prevention**: Ensures each leaf node contains at least 5 subjects to prevent overfitting.
- **Node Categorization**: Leaf nodes are categorized as positive or negative based on the count of positive and negative cases.
- **Accuracy Calculation**: Calculates the accuracy of the decision tree based on the categorization of leaf nodes.

## Output
The program outputs the following:
- The entire structure of the decision tree.
- The feature and threshold value used in each node.
- The Gini index at each node.
- The categorization (positive or negative), along with
