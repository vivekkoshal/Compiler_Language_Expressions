#pragma once
#include "exprtreenode.h"
#include "symtable.h"
#include <vector>
using namespace std;

class ExpressionParser {
public:
    vector<ExprTreeNode*> treeNodes;
    SymbolTable* table;
    int lastRemoved;
    ExpressionParser();
    ~ExpressionParser();
  
    void process(vector<string> expr);
};
