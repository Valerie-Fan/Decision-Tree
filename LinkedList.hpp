#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Node.hpp"

// Represents a linked list used to construct the decision tree
class LinkedList {
private:
    Node* head; // Pointer to the first node of the linked list
    Node* tail; // Pointer to the last node of the linked list
    int size; // Number of nodes in the linked list

public:
    // Constructor
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor
    ~LinkedList() {
        clear();
    }

    // Add a new node to the end of the linked list
    void append(Node* newNode) {
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->setRightChild(newNode);
            tail = newNode;
        }
        size++;
    }

    // Remove and return the first node from the linked list
    Node* popFront() {
        if (head == nullptr) return nullptr;

        Node* nodeToRemove = head;
        head = head->getRightChild();
        if (head == nullptr) tail = nullptr; // If the list is now empty, adjust tail

        nodeToRemove->setRightChild(nullptr); // Disconnect the removed node
        size--;
        return nodeToRemove;
    }

    // Clear the linked list
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->getRightChild();
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // Check if the list is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Getters
    Node* getHead() const { return head; }
    Node* getTail() const { return tail; }
    int getSize() const { return size; }
};

#endif 
