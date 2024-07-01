
#include "heapnode.h"

HeapNode::HeapNode() {
    value = -1;
    parent = NULL;
    leftChild = NULL;
    rightChild = NULL;
}

HeapNode::HeapNode(int initialValue) {
    parent = NULL;
    leftChild = NULL;
    rightChild = NULL;
    this->value = initialValue;
}

HeapNode::~HeapNode() {
    ;
}