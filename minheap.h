#pragma once
#include "treenode.h"
using namespace std;

class PriorityQueue {
  private:
  int count = 0;
  TreeNode* top = nullptr;
  
  public:
  PriorityQueue();
  ~PriorityQueue();

  void insert(int value);
  int retrieveMin();
  void removeMin();
};
