#pragma once
#include <string>
using namespace std;

class TreeNode {
public:
    string id;
    int depth;
    int location = -1;
    
    TreeNode* parent = nullptr;
    TreeNode* leftChild = nullptr;
    TreeNode* rightChild = nullptr;

    TreeNode();
    TreeNode(string key);

    TreeNode* rotateLeftLeft(); // case 1
    TreeNode* rotateRightRight(); // case 2
    TreeNode* rotateLeftRight(); // case 3
    TreeNode* rotateRightLeft(); // case 4

    ~TreeNode();
};
