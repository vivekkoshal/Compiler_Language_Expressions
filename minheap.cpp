#include "minheap.h"
#include <iostream>

class Queue {
public:
    int capacity;
    HeapNode** elements;
    int frontIndex;
    int backIndex;

    Queue(int size) {
        capacity = size;
        elements = new HeapNode*[size];
        frontIndex = -1;
        backIndex = -1;
    }

    ~Queue() {
        delete[] elements;
    }

    void enqueue(HeapNode* node) {
        if (isFull())
            return;
        if (frontIndex == -1 && backIndex == -1)
            frontIndex = 0;
        if (backIndex == capacity - 1)
            backIndex = -1;
        backIndex++;
        elements[backIndex] = node;
    }

    void dequeue() {
        if (isEmpty())
            return;
        if (frontIndex == backIndex) {
            frontIndex = -1;
            backIndex = -1;
        } else if (frontIndex == capacity - 1) {
            frontIndex = 0;
        } else {
            frontIndex++;
        }
    }

    HeapNode* getFront() {
        if (isEmpty()) {
            return nullptr;
        }
        return elements[frontIndex];
    }

    HeapNode* getBack() {
        if (isEmpty())
            return nullptr;
        return elements[backIndex];
    }

    bool isEmpty() {
        return (frontIndex == -1);
    }

    bool isFull() {
        return ((frontIndex == 0 && backIndex == capacity - 1) || (backIndex == frontIndex - 1));
    }
};

MinHeap::MinHeap() {
    heapSize = 0;
    rootNode = nullptr;
}

void MinHeap::insert(int value) {
    if (!rootNode) {
        rootNode = new HeapNode(value);
        heapSize = 1;
        return;
    }

    HeapNode* parentNode = nullptr;
    Queue nodeQueue(500000);
    nodeQueue.enqueue(rootNode);

    while (!nodeQueue.isEmpty()) {
        parentNode = nodeQueue.getFront();
        nodeQueue.dequeue();
        if (!parentNode->left || !parentNode->right) {
            break;
        }

        if (parentNode->left)
            nodeQueue.enqueue(parentNode->left);
        if (parentNode->right)
            nodeQueue.enqueue(parentNode->right);
    }

    HeapNode* newNode = new HeapNode(value);
    newNode->parent = parentNode;
    if (!parentNode->left)
        parentNode->left = newNode;
    else
        parentNode->right = newNode;

    while (parentNode && parentNode->value > newNode->value) {
        int temp = parentNode->value;
        parentNode->value = newNode->value;
        newNode->value = temp;
        newNode = parentNode;
        parentNode = parentNode->parent;
    }

    heapSize++;
}

int MinHeap::getMin() {
    if (rootNode)
        return rootNode->value;
    else
        return -1;
}

void MinHeap::removeMin() {
    if (!rootNode) {
        return;
    }
    if (!rootNode->left && !rootNode->right) {
        delete rootNode;
        rootNode = nullptr;
        heapSize = 0;
        return;
    }
    if (rootNode->left && !rootNode->right) {
        rootNode->value = rootNode->left->value;
        delete rootNode->left;
        rootNode->left = nullptr;
        heapSize = 1;
        return;
    }

    HeapNode* lastNode = nullptr;
    HeapNode* minNode = rootNode->left;
    Queue nodeQueue(500000);
    nodeQueue.enqueue(rootNode->left);
    nodeQueue.enqueue(rootNode->right);

    while (!nodeQueue.isEmpty()) {
        HeapNode* node = nodeQueue.getFront();
        nodeQueue.dequeue();

        if (!node->left || !node->right)
            lastNode = node;
        if (node->value < minNode->value)
            minNode = node;

        if (node->left)
            nodeQueue.enqueue(node->left);
        if (node->right)
            nodeQueue.enqueue(node->right);
    }

    rootNode->value = minNode->value;
    minNode->value = lastNode->value;
    if (lastNode->parent->left == lastNode)
        lastNode->parent->left = nullptr;
    else if (lastNode->parent->right == lastNode)
        lastNode->parent->right = nullptr;
    delete lastNode;
}

MinHeap::~MinHeap() {

}
