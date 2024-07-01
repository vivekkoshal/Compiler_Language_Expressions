#include "entry.h"
#include <iostream>
#include <string>
using namespace std;

SymEntry::SymEntry() {
    this->id = "";
    this->left = nullptr;
    this->right = nullptr;
    this->value = nullptr;
}

SymEntry::SymEntry(string id, UnlimitedRational* value) {
    this->id = id;
    this->left = nullptr;
    this->right = nullptr;
    this->value = value;
}

SymEntry::~SymEntry() {
    delete value;
    this->id = "";
}
