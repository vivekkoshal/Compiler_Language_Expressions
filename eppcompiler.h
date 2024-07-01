#pragma once
#include "parser.h"
#include "minheap.h"
#include <vector>
#include <fstream>
using namespace std;

class CodeCompiler {
  private:
  Parser parser;
  int mem_size;
  string output_filename;
  vector<int> mem_locations;
  MinHeap free_mem_locations;

  public:

  CodeCompiler();
  CodeCompiler(string out_filename, int mem_limit);

  void compile(vector<vector<string>> code);
  vector<string> generate_target_commands();
  void write_to_file(vector<string> commands);
  
  ~CodeCompiler();
};