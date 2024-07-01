#include <iostream>
#include <stdexcept>
#include "minheap.h"

using namespace std;

void displayHeap(HeapNode* node) {
    if (node) {
        displayHeap(node->left);
        cout << node->val << " ";
        displayHeap(node->right);
    }
}

int main(int argc, char* argv[]) {
    string input, command;
    MinHeap heap;

    do {
        cout << "Enter the operation: ";
        cin >> command >> input;

        if (input == "q") break;

        if (command == "push") {
            cout << "Pushing: " << stoi(input) << endl;
            heap.push_heap(stoi(input));
            cout << "Push successful: " << input << endl;
        }
        else if (command == "get_min") {
            cout << "Minimum element is: " << heap.get_min() << endl;
        }
        else if (command == "pop") {
            heap.pop();
            cout << "Pop successful" << endl;
        }
        else {
            cout << "Unknown command: " << command << endl;
        }

    } while (input != "q");

    return 0;
}
