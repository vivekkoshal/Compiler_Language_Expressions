#include "exprtreenode.h"

ExprTreeNode::ExprTreeNode() {
    this->identifier = "";
    this->left_child = NULL;
    this->right_child = NULL;
    this->node_type = "";
    this->value = -1;
}

ExprTreeNode::ExprTreeNode(string type, int value) {
    this->node_type = type;
    this->left_child = NULL;
    this->right_child = NULL;
    this->value = value;
    this->identifier = "";
}

ExprTreeNode::~ExprTreeNode() {
}
