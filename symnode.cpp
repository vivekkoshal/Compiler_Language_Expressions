#include "symtable.h"
#include <iostream>
using namespace std;

int calculateHeight(SymNode* node);
int getBalance(SymNode* node) {
    if(node == nullptr)
        return 0;
    return calculateHeight(node->left) - calculateHeight(node->right);
}
SymNode* createNode(string key){
    SymNode* newNode = new SymNode(key);
    newNode->height = 1;
    return newNode;
}

SymNode* rotateRight(SymNode* node) {
    SymNode* leftChild = node->left;
    SymNode* rightSubTree = leftChild->right;

    leftChild->right = node;
    node->left = rightSubTree;

    node->height = max(calculateHeight(node->left), calculateHeight(node->right)) + 1;
    leftChild->height = max(calculateHeight(leftChild->left), calculateHeight(leftChild->right)) + 1;

    return leftChild;
}

SymNode* rotateLeft(SymNode* node) {
    SymNode* rightChild = node->right;
    SymNode* leftSubTree = rightChild->left;

    rightChild->left = node;
    node->right = leftSubTree;

    node->height = max(calculateHeight(node->left), calculateHeight(node->right)) + 1;
    rightChild->height = max(calculateHeight(rightChild->left), calculateHeight(rightChild->right)) + 1;

    return rightChild;
}

SymNode* insertNode(SymNode* node, string key, int& treeSize) {
    if(node == nullptr){
        treeSize++;
        return createNode(key);
    }

    if(key < node->key) {
        SymNode* temp = insertNode(node->left, key, treeSize);
        temp->par = node;
        node->left = temp;
    } else if(key > node->key) {
        SymNode* temp = insertNode(node->right, key, treeSize);
        temp->par = node;
        node->right = temp;
    } else {
        return node;
    }

    node->height = 1 + max(calculateHeight(node->left), calculateHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

SymNode* findMinValue(SymNode* node) {
    SymNode* current = node;

    while (current->left != nullptr)
        current = current->left;

    return current;
}

SymNode* deleteNode(SymNode* node, string key, int& treeSize, int& tempSize) {
    if (node == nullptr)
        return node;

    if (key < node->key)
        node->left = deleteNode(node->left, key, treeSize, tempSize);
    else if (key > node->key)
        node->right = deleteNode(node->right, key, treeSize, tempSize);
    else {
        treeSize--;

        if ((node->left == nullptr) || (node->right == nullptr)) {
            SymNode* temp = node->left ? node->left : node->right;

            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            SymNode* temp = findMinValue(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key, tempSize, tempSize);
        }
    }

    if (node == nullptr)
        return node;

    node->height = 1 + max(calculateHeight(node->left), calculateHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

SymNode* searchNode(SymNode* node, string key){
    if(node == nullptr)
        return nullptr;

    if(key < node->key)
        return searchNode(node->left, key);
    if(key > node->key)
        return searchNode(node->right, key);

    return node;
}

void deleteAllNodes(SymNode* node){
    if(node == nullptr)
        return;

    if(node->left != nullptr)
        deleteAllNodes(node->left);
    if(node->right != nullptr)
        deleteAllNodes(node->right);

    delete node;
}

void preorderTraversal(SymNode* node) {
    if (node != nullptr) {
        cout << node->key << " ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
}

SymbolTable::SymbolTable(){
    this->size = 0;
    this->root = nullptr;
}

void SymbolTable::insert(string key){
    this->root = insertNode(root, key, size);
}

void SymbolTable::remove(string key){
    int tempSize = 0;
    this->root = deleteNode(this->root, key, size, tempSize);
}

int SymbolTable::search(string key){
    SymNode* result = searchNode(this->get_root(), key);
    if(result == nullptr)
        return -2;

    return result->address;
}

void SymbolTable::assign_address(string key, int address){
    SymNode* node = searchNode(this->get_root(), key);
    if(node == nullptr)
        return;

    node->address = address;
}

int SymbolTable::get_size(){
    return size;
}

SymNode* SymbolTable::get_root(){
    return root;
}

SymbolTable::~SymbolTable(){
    deleteAllNodes(root);
}
