#pragma once
#include <string>
using namespace std;

class Node {
  public:
  int key;
  Node* parent = nullptr;
  Node* leftChild = nullptr;
  Node* rightChild = nullptr;
  
  Node();
  Node(int initialKey);
  
  ~Node();
};

