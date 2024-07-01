#include "evaluator.h"

#include <iostream>
#include <string>
using namespace std;

#define MAX_STACK_SIZE 100

UnlimitedInt* one = new UnlimitedInt(1);
UnlimitedInt* zeroInt = new UnlimitedInt(0);
UnlimitedRational* zero = new UnlimitedRational(zeroInt, one);

int precedence(string op) {
    if (op == "ADD" || op == "SUB")
        return 1;
    if (op == "MUL" || op == "DIV")
        return 2;
    return 0;
}

bool isOperator(string s) {
    char c = s[0];
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

string determineType(string t) {
    if (t == "*")
        return "MUL";
    if (t == "-")
        return "SUB";
    if (t == "+")
        return "ADD";
    if (t == "/")
        return "DIV";
    if (t == "(" || t == ")")
        return "NULL";

    bool isValue = true;
    for (char c : t) {
        if (!(isDigit(c) || (c == '-' && isValue))) {
            isValue = false;
            break;
        }
    }
    if (isValue) {
        return "VAL";
    }
    return "VAR";
}

Evaluator::Evaluator() {
    symtable = new SymbolTable();
}

Evaluator::~Evaluator() {
    for (ExprTreeNode* node : expr_trees) {
        delete node;
    }
}

void eulerTraversal(ExprTreeNode* root) {
    if (root != nullptr) {
        std::cout << root->type;

        if (root->type == "VAL") {
            std::cout << "(" << root->val->get_p_str() << ")";
        }

        if (root->left != nullptr || root->right != nullptr) {
            std::cout << "(";
            eulerTraversal(root->left);
            std::cout << ",";
            eulerTraversal(root->right);
            std::cout << ")";
        }
    }
}

void postorder(ExprTreeNode* root) {
    if (root != nullptr) {
        postorder(root->left);
        postorder(root->right);
        cout << root->type << " ";
    }
}

static ExprTreeNode* findParentNode(ExprTreeNode* node, ExprTreeNode* root) {
    if (root->left == node || root->right == node)
        return root;

    if (root->left) {
        if (ExprTreeNode* n = findParentNode(node, root->left))
            return n;
    }

    if (root->right) {
        if (ExprTreeNode* n = findParentNode(node, root->right))
            return n;
    }

    return nullptr;
}

static bool isNumber(string token) {
    if (token.size() == 0)
        return false;

    int curChar = 0;
    if (token[curChar] == '-') {
        if (token.size() == 1)
            return false;
        curChar++;
    }

    while (curChar < token.size()) {
        if (token[curChar] < '0' || token[curChar] > '9')
            return false;
        curChar++;
    }

    return true;
}

void Evaluator::parse(vector<string> code) {
    ExprTreeNode* root = new ExprTreeNode();
    root->left = new ExprTreeNode();
    ExprTreeNode* currentNode = root->left;

    for (string token : code) {
        if (token == "(") {
            currentNode->left = new ExprTreeNode();
            currentNode = currentNode->left;
            continue;
        }

        if (token == ")") {
            currentNode = findParentNode(currentNode, root);
            continue;
        }

        if (token == ":=" || token == "+" || token == "-" || token == "*" || token == "/" || token == "mod") {
            if (token == ":=")
                currentNode->type = "ASS";
            else if (token == "+")
                currentNode->type = "ADD";
            else if (token == "-")
                currentNode->type = "SUB";
            else if (token == "*")
                currentNode->type = "MUL";
            else if (token == "/")
                currentNode->type = "DIV";
            else if (token == "mod")
                currentNode->type = "MOD";

            currentNode->right = new ExprTreeNode();
            currentNode = currentNode->right;
            continue;
        }

        if (isNumber(token)) {
            currentNode->type = "VAL";
            UnlimitedInt* num = new UnlimitedInt(token);
            UnlimitedInt* den = new UnlimitedInt(1);
            currentNode->val = new UnlimitedRational(num, den);
            delete num;
            delete den;
            currentNode = findParentNode(currentNode, root);
            continue;
        }

        currentNode->type = "VAR";
        currentNode->id = token;
        currentNode = findParentNode(currentNode, root);
    }

    expr_trees.push_back(root);
    return;
}

UnlimitedRational* evaluateExprTreeNode(ExprTreeNode* node, SymbolTable* symtable) {
    if (!node) {
        return zero;
    }

    if (!node->left && !node->right) {
        if (node->type == "VAL")
            return node->val;
        else {
            return symtable->search(node->id);
        }
    }

    UnlimitedRational* leftVal = evaluateExprTreeNode(node->left, symtable);
    UnlimitedRational* rightVal = evaluateExprTreeNode(node->right, symtable);

    if (node->type == "MUL") {
        return UnlimitedRational::mul(leftVal, rightVal);
    } else if (node->type == "DIV") {
        return UnlimitedRational::div(leftVal, rightVal);
    } else if (node->type == "ADD") {
        return UnlimitedRational::add(leftVal, rightVal);
    }

    return UnlimitedRational::sub(leftVal, rightVal);
}

void Evaluator::eval() {
    if (expr_trees.empty())
        return;

    ExprTreeNode* current = expr_trees.back();
    ExprTreeNode* varNode = current->left;
    ExprTreeNode* exprNode = current->right;

    UnlimitedRational* exprVal = evaluateExprTreeNode(exprNode, symtable);
    symtable->insert(varNode->id, exprVal);

    current->right = new ExprTreeNode("VAL", exprVal);
    expr_trees.pop_back();
    expr_trees.push_back(current);
}
