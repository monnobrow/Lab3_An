#include <exception>
#include <iostream>
#include <string>

#include "ProcessCommand.h"
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1 || (argc == 2 && string(argv[1]) == "--help")) {
    PrintHelp();
    return 0;
  }
  string filename;
  string query;
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "--file" && i + 1 < argc && filename.empty()) {
      filename = argv[++i];
    } else if (arg == "--query" && i + 1 < argc && query.empty()) {
      query = argv[++i];
    } else {
      cerr << "Ошибка: неизвестный, повторный или неполный параметр " << arg
           << '\n';
      return 1;
    }
  }
  if (filename.empty() || query.empty()) {
    cerr << "Ошибка: нужны оба параметра --file и --query\n";
    return 1;
  }
  try {
    ProcessCommand(filename, query);
  } catch (const exception &error) {
    cerr << "Ошибка: " << error.what() << '\n';
    return 1;
  }
  return 0;
}