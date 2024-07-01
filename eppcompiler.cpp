#include "eppcompiler.h"
#include <iostream>
using namespace std;

int search_free_slot(MinHeap &least_mem_loc) {
    int idx = least_mem_loc.get_min();
    least_mem_loc.pop();
    return idx;
}

vector<string> gen_targ(ExprTreeNode* root, Parser& targ) {
    vector<string> commands;
    if (root == NULL)
        return commands;

    vector<string> left_commands = gen_targ(root->left, targ);
    vector<string> right_commands = gen_targ(root->right, targ);

    commands.insert(commands.end(), right_commands.begin(), right_commands.end());
    commands.insert(commands.end(), left_commands.begin(), left_commands.end());

    if (root->type == "VAR") {
        int idx = targ.symtable->search(root->id);
        commands.push_back("PUSH mem[" + to_string(idx) + "]");
    }
    if (root->type == "VAL") {
        commands.push_back("PUSH " + to_string(root->num));
    }
    if (root->type == "ADD") {
        commands.push_back("ADD");
    }
    if (root->type == "MUL") {
        commands.push_back("MUL");
    }
    if (root->type == "SUB") {
        commands.push_back("SUB");
    }
    if (root->type == "DIV") {
        commands.push_back("DIV");
    }
    return commands;
}

EPPCompiler::EPPCompiler() {
    memory_size = 0;
    output_file = "";
}

EPPCompiler::EPPCompiler(string out_file, int mem_limit) {
    memory_size = mem_limit;
    output_file = out_file;
    for (int i = 0; i < mem_limit; i++) {
        least_mem_loc.push_heap(i);
    }
}

void EPPCompiler::compile(vector<vector<string>> code) {
    ofstream out(output_file, ios::out);
    out.close();
    vector<vector<string>> commands;
    int count = 0;
    for (auto i : code) {
        targ.parse(i);
        count++;
        ExprTreeNode* curr = targ.expr_trees.back();
        if (curr->left->type == "VAR") {
            int idx = search_free_slot(least_mem_loc);
            targ.symtable->assign_address(curr->left->id, idx);
        }
        if (curr->left->type == "DEL") {
            ;
        }
        commands.push_back(generate_targ_commands());
    }
    for (auto i : commands) {
        write_to_file(i);
        ofstream out(output_file, ios::app);
        out << "\n";
        out.close();
    }
}

vector<string> EPPCompiler::generate_targ_commands() {
    ExprTreeNode* curr = targ.expr_trees.back();
    vector<string> commands;
    if (curr->left->type == "DEL") {
        int idx = targ.symtable->search(curr->right->id);
        least_mem_loc.push_heap(idx);
        targ.symtable->remove(curr->right->id);
        commands.push_back("DEL = mem[" + to_string(idx) + "]");
        return commands;
    }

    commands = gen_targ(curr->right, targ);
    if (curr->left->type == "VAR") {
        int idx = targ.symtable->search(curr->left->id);
        commands.push_back("mem[" + to_string(idx) + "] = POP");
        return commands;
    }
    if (curr->left->type == "RET") {
        commands.push_back("RET = POP");
        return commands;
    }
    return commands;
}

void EPPCompiler::write_to_file(vector<string> commands) {
    ofstream out(output_file, ios::app);
    for (auto i : commands) {
        out << i << endl;
    }
    out.close();
}

EPPCompiler::~EPPCompiler() {
}
