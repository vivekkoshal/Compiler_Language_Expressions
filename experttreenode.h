#pragma once
#include <string>
using namespace std;

class ExpressionTreeNode {
public:
    string node_type; // Can be "ADD", "SUB", "MUL", "DIV", "VAL", "DEL", "RET" or "VAR" (for variable)

    // Stores the variable name if node_type is "VAR"
    string variable_name;
    // Store the value of the number if node_type is "VAL"
    int value;

    ExpressionTreeNode* left_child = NULL;
    ExpressionTreeNode* right_child = NULL;

    ExpressionTreeNode();
    ExpressionTreeNode(string type, int val);

    ~ExpressionTreeNode();
};
