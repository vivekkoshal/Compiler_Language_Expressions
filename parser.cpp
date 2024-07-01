#pragma once
#include "exprtreenode.h"
#include "symtable.h"
#include <vector>
using namespace std;

class ExpressionParser {
public:
    vector<ExprTreeNode*> trees;
    SymbolTable* symbolTable;
    int recentlyRemoved;
    
    ExpressionParser();
    ~ExpressionParser();
    
    void parseExpression(vector<string> tokens);
};

static ExprTreeNode* findParent(ExprTreeNode* node, ExprTreeNode* root) {
    if (root->leftChild == node || root->rightChild == node)
        return root;
    if (root->leftChild) {
        if (ExprTreeNode* parent = findParent(node, root->leftChild))
            return parent;
    }
    if (root->rightChild) {
        if (ExprTreeNode* parent = findParent(node, root->rightChild))
            return parent;
    }
    return nullptr;
}

static bool isNumeric(string token) {
    if (token.empty())
        return false;
    int index = 0;
    if (token[index] == '-') {
        if (token.size() == 1)
            return false;
        index++;
    }
    while (index < token.size()) {
        if (token[index] < '0' || token[index] > '9')
            return false;
        index++;
    }
    return true;
}

ExpressionParser::ExpressionParser() {
    symbolTable = new SymbolTable();
}

void ExpressionParser::parseExpression(vector<string> tokens) {
    ExprTreeNode* rootNode = new ExprTreeNode();
    rootNode->leftChild = new ExprTreeNode();
    ExprTreeNode* currentNode = rootNode->leftChild;

    for (const string& token : tokens) {
        if (token == "(") {
            currentNode->leftChild = new ExprTreeNode();
            currentNode = currentNode->leftChild;
            continue;
        }

        if (token == ")") {
            currentNode = findParent(currentNode, rootNode);
            continue;
        }

        if (token == ":=" || token == "+" || token == "-" || token == "*" || token == "/" || token == "mod") {
            if (token == ":=") {
                currentNode->type = "ASSIGN";
                currentNode->rightChild = new ExprTreeNode();
                currentNode = currentNode->rightChild;
                continue;
            } else if (token == "+")
                currentNode->type = "ADD";
            else if (token == "-")
                currentNode->type = "SUBTRACT";
            else if (token == "*")
                currentNode->type = "MULTIPLY";
            else if (token == "/")
                currentNode->type = "DIVIDE";
            else if (token == "mod")
                currentNode->type = "MODULO";
            currentNode->rightChild = new ExprTreeNode();
            currentNode = currentNode->rightChild;
            continue;
        }

        if (isNumeric(token)) {
            currentNode->type = "VALUE";
            currentNode->number = stoi(token);
            currentNode = findParent(currentNode, rootNode);
            continue;
        }

        if (token == "ret") {
            currentNode->type = "RETURN";
            currentNode = findParent(currentNode, rootNode);
        } else if (token == "del") {
            currentNode->type = "DELETE";
            currentNode = findParent(currentNode, rootNode);
        } else {
            currentNode->type = "VARIABLE";
            currentNode->identifier = token;
            if (tokens[0] != "del")
                symbolTable->insert(token);
            currentNode = findParent(currentNode, rootNode);
        }
    }

    trees.push_back(rootNode);
}

ExpressionParser::~ExpressionParser() {
    delete symbolTable;
    for (auto node : trees)
        delete node;
}
