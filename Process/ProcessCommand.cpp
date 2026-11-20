#include "ProcessCommand.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "struct/Array/array.h"
#include "struct/LinkedList/doublelist.h"
#include "struct/Queue/queue.h"
#include "struct/SingleList/singlList.h"
#include "struct/Stack/stack.h"
#include "struct/Tree/tree.h"
#include "struct/hash/hesh_table.h"
#include "struct/hash/open_table.h"
using namespace std;

void PrintHelp() {
  cout << "Запуск:\n";
  cout << "./build/app --file файл --query 'КОМАНДА аргументы'\n\n";
  cout << "Один файл хранит одну структуру. Индексы с 0.\n";
  cout << "Значение с пробелами пишется в двойных кавычках: SPush \"hello "
          "world\"\n\n";
  cout << "Односвязный список (F) и двусвязный (L), для L те же команды с "
          "буквой L:\n";
  cout << "FAddFirst x | FAddEnd x | FAddAfter t x | FAddBefore t x | "
          "FAddIndex x i\n";
  cout << "FDeleteFirst | FDeleteEnd | FDeleteAfter t | FDeleteBefore t\n";
  cout << "FDeleteIndex i | FDeleteValue x | FGet i | FFind x\n";
  cout << "FPrint | FPrintBack | FPrintAddresses | FClear\n\n";
  cout << "Стек:\nSPush x | SPop | STop | SIsEmptyStack | SPrint | "
          "SPrintAddresses | SClear\n\n";
  cout << "Очередь:\nQPush x | QPop | QFront | QBack | QPrintQueue | "
          "QPrintAddresses | QClear\n\n";
  cout << "Массив:\nMAddEnd x | MAddIndex x i | MGet i | MDeleteIndex i | "
          "MReplace x i\n";
  cout << "MLength | MPrint | MPrintAddresses | MClear\n\n";
  cout << "АВЛ-дерево (целые числа):\nTAdd n | TDelete n | TFind n | THeight\n";
  cout << "TPrintPryam | TPrintSim | TPrintObr | TPrintBfs | TPrintTree | "
          "TPrintAddresses | TClear\n\n";
  cout << "Хеш-таблица с цепочками:\nHInsert k v | HGet k | HContains k | "
          "HDelete k\n";
  cout << "HLength | HPrint | HPrintAddresses | HClear\n\n";
  cout << "Открытая адресация:\nOInsert k v | OGet k | OContains k | ODelete k "
          "| OLength | OPrint\n";
}

// следующее слово запроса; quoted понимает "значение с пробелами"
string ReadValue(istringstream &input) {
  string value;
  if (!(input >> quoted(value))) {
    throw invalid_argument("не хватает аргументов команды");
  }
  return value;
}

// число через stoi; pos проверяет, что после числа нет мусора ("12abc")
int ReadNumber(istringstream &input) {
  string value = ReadValue(input);
  size_t pos = 0;
  int number = stoi(value, &pos);
  if (pos != value.size()) {
    throw invalid_argument("ожидалось целое число: " + value);
  }
  return number;
}

void CheckEnd(istringstream &input) {
  string extra;
  if (input >> extra) {
    throw invalid_argument("лишние аргументы команды");
  }
}

// значение считается до вызова Out, поэтому при throw "-> " не печатается
void Out(const string &value) { cout << "-> " << value << '\n'; }

string BoolText(bool value) {
  if (value)
    return "TRUE";
  return "FALSE";
}

void Unknown(const string &command) {
  throw invalid_argument("неизвестная команда: " + command);
}

// ---------- односвязный список ----------
void RunSinglList(const string &filename, const string &command,
                  istringstream &input) {
  SinglList list;
  list.Load(filename);
  bool changed = true; // команды просмотра ниже ставят false

  if (command == "FAddFirst" || command == "FAddEnd") {
    string value = ReadValue(input);
    CheckEnd(input);
    if (command == "FAddFirst")
      list.FAddFirst(value);
    else
      list.FAddEnd(value);
    Out(value);
  } else if (command == "FAddAfter" || command == "FAddBefore") {
    string target = ReadValue(input);
    string value = ReadValue(input);
    CheckEnd(input);
    if (command == "FAddAfter")
      list.FAddAfter(target, value);
    else
      list.FAddBefore(target, value);
    Out(value);
  } else if (command == "FAddIndex") {
    string value = ReadValue(input);
    int index = ReadNumber(input);
    CheckEnd(input);
    list.FAddIndex(value, index);
    Out(value);
  } else if (command == "FDeleteFirst" || command == "FDeleteEnd") {
    CheckEnd(input);
    if (command == "FDeleteFirst")
      Out(list.FDeleteFirst());
    else
      Out(list.FDeleteEnd());
  } else if (command == "FDeleteAfter" || command == "FDeleteBefore" ||
             command == "FDeleteValue") {
    string value = ReadValue(input);
    CheckEnd(input);
    if (command == "FDeleteAfter")
      Out(list.FDeleteAfter(value));
    else if (command == "FDeleteBefore")
      Out(list.FDeleteBefore(value));
    else
      Out(list.FDeleteValue(value));
  } else if (command == "FDeleteIndex") {
    int index = ReadNumber(input);
    CheckEnd(input);
    Out(list.FDeleteIndex(index));
  } else if (command == "FClear") {
    CheckEnd(input);
    list.FClear();
  } else {
    changed = false;
    if (command == "FGet") {
      int index = ReadNumber(input);
      CheckEnd(input);
      Out(list.FGet(index));
    } else if (command == "FFind") {
      string value = ReadValue(input);
      CheckEnd(input);
      Out(BoolText(list.FFind(value)));
    } else if (command == "FPrint") {
      CheckEnd(input);
      list.FPrint();
    } else if (command == "FPrintBack") {
      CheckEnd(input);
      list.FPrintBack();
    } else if (command == "FPrintAddresses") {
      CheckEnd(input);
      list.FPrintAddresses();
    } else {
      Unknown(command);
    }
  }
  if (changed)
    list.Save(filename);
}

// ---------- двусвязный список ----------
void RunDoubleList(const string &filename, const string &command,
                   istringstream &input) {
  DoubleList list;
  list.Load(filename);
  bool changed = true;

  if (command == "LAddFirst" || command == "LAddEnd") {
    string value = ReadValue(input);
    CheckEnd(input);
    if (command == "LAddFirst")
      list.LAddFirst(value);
    else
      list.LAddEnd(value);
    Out(value);
  } else if (command == "LAddAfter" || command == "LAddBefore") {
    string target = ReadValue(input);
    string value = ReadValue(input);
    CheckEnd(input);
    if (command == "LAddAfter")
      list.LAddAfter(target, value);
    else
      list.LAddBefore(target, value);
    Out(value);
  } else if (command == "LAddIndex") {
    string value = ReadValue(input);
    int index = ReadNumber(input);
    CheckEnd(input);
    list.LAddIndex(value, index);
    Out(value);
  } else if (command == "LDeleteFirst" || command == "LDeleteEnd") {
    CheckEnd(input);
    if (command == "LDeleteFirst")
      Out(list.LDeleteFirst());
    else
      Out(list.LDeleteEnd());
  } else if (command == "LDeleteAfter" || command == "LDeleteBefore" ||
             command == "LDeleteValue") {
    string value = ReadValue(input);
    CheckEnd(input);
    if (command == "LDeleteAfter")
      Out(list.LDeleteAfter(value));
    else if (command == "LDeleteBefore")
      Out(list.LDeleteBefore(value));
    else
      Out(list.LDeleteValue(value));
  } else if (command == "LDeleteIndex") {
    int index = ReadNumber(input);
    CheckEnd(input);
    Out(list.LDeleteIndex(index));
  } else if (command == "LClear") {
    CheckEnd(input);
    list.LClear();
  } else {
    changed = false;
    if (command == "LGet") {
      int index = ReadNumber(input);
      CheckEnd(input);
      Out(list.LGet(index));
    } else if (command == "LFind") {
      string value = ReadValue(input);
      CheckEnd(input);
      Out(BoolText(list.LFind(value)));
    } else if (command == "LPrint") {
      CheckEnd(input);
      list.LPrint();
    } else if (command == "LPrintBack") {
      CheckEnd(input);
      list.LPrintBack();
    } else if (command == "LPrintAddresses") {
      CheckEnd(input);
      list.LPrintAddresses();
    } else {
      Unknown(command);
    }
  }
  if (changed)
    list.Save(filename);
}

// ---------- стек ----------
void RunStack(const string &filename, const string &command,
              istringstream &input) {
  Stack stack;
  stack.Load(filename);
  bool changed = true;

  if (command == "SPush") {
    string value = ReadValue(input);
    CheckEnd(input);
    stack.SPush(value);
    Out(value);
  } else if (command == "SPop") {
    CheckEnd(input);
    Out(stack.SPop());
  } else if (command == "SClear") {
    CheckEnd(input);
    stack.SClear();
  } else {
    changed = false;
    CheckEnd(input);
    if (command == "STop")
      Out(stack.STop());
    else if (command == "SIsEmptyStack")
      Out(BoolText(stack.SIsEmptyStack()));
    else if (command == "SPrint")
      stack.SPrint();
    else if (command == "SPrintAddresses")
      stack.SPrintAddresses();
    else
      Unknown(command);
  }
  if (changed)
    stack.Save(filename);
}

// ---------- очередь ----------
void RunQueue(const string &filename, const string &command,
              istringstream &input) {
  Queue queue;
  queue.Load(filename);
  bool changed = true;

  if (command == "QPush") {
    string value = ReadValue(input);
    CheckEnd(input);
    queue.QPush(value);
    Out(value);
  } else if (command == "QPop") {
    CheckEnd(input);
    Out(queue.QPop());
  } else if (command == "QClear") {
    CheckEnd(input);
    queue.QClear();
  } else {
    changed = false;
    CheckEnd(input);
    if (command == "QFront")
      Out(queue.QFront());
    else if (command == "QBack")
      Out(queue.QBack());
    else if (command == "QPrintQueue")
      queue.QPrintQueue();
    else if (command == "QPrintAddresses")
      queue.QPrintAddresses();
    else
      Unknown(command);
  }
  if (changed)
    queue.Save(filename);
}

// ---------- массив ----------
void RunArray(const string &filename, const string &command,
              istringstream &input) {
  Array arr;
  arr.Load(filename);
  bool changed = true;

  if (command == "MAddEnd") {
    string value = ReadValue(input);
    CheckEnd(input);
    arr.MAddEnd(value);
    Out(value);
  } else if (command == "MAddIndex" || command == "MReplace") {
    string value = ReadValue(input);
    int index = ReadNumber(input);
    CheckEnd(input);
    if (command == "MAddIndex")
      arr.MAddIndex(value, index);
    else
      arr.MReplace(value, index);
    Out(value);
  } else if (command == "MDeleteIndex") {
    int index = ReadNumber(input);
    CheckEnd(input);
    Out(arr.MDeleteIndex(index));
  } else if (command == "MClear") {
    CheckEnd(input);
    arr.MClear();
  } else {
    changed = false;
    if (command == "MGet") {
      int index = ReadNumber(input);
      CheckEnd(input);
      Out(arr.MGet(index));
    } else {
      CheckEnd(input);
      if (command == "MLength")
        Out(to_string(arr.MLength()));
      else if (command == "MPrint")
        arr.MPrint();
      else if (command == "MPrintAddresses")
        arr.MPrintAddresses();
      else
        Unknown(command);
    }
  }
  if (changed)
    arr.Save(filename);
}

// ---------- АВЛ-дерево ----------
void RunTree(const string &filename, const string &command,
             istringstream &input) {
  Tree tree;
  tree.Load(filename);
  bool changed = true;

  if (command == "TAdd" || command == "TDelete") {
    int number = ReadNumber(input);
    CheckEnd(input);
    if (command == "TAdd")
      tree.TAdd(number);
    else
      tree.TDelete(number);
    Out(to_string(number));
  } else if (command == "TClear") {
    CheckEnd(input);
    tree.TClear();
  } else {
    changed = false;
    if (command == "TFind") {
      int number = ReadNumber(input);
      CheckEnd(input);
      Out(BoolText(tree.TFind(number)));
    } else {
      CheckEnd(input);
      if (command == "THeight") {
        Out(to_string(tree.THeight()));
      } else if (command == "TPrintPryam") {
        tree.TPrintPryam();
        cout << '\n';
      } else if (command == "TPrintSim") {
        tree.TPrintSim();
        cout << '\n';
      } else if (command == "TPrintObr") {
        tree.TPrintObr();
        cout << '\n';
      } else if (command == "TPrintBfs") {
        tree.TPrintBfs();
        cout << '\n';
      } else if (command == "TPrintTree") {
        tree.TPrintTree();
      } else if (command == "TPrintAddresses") {
        tree.TPrintAddresses();
      } else {
        Unknown(command);
      }
    }
  }
  if (changed)
    tree.Save(filename);
}

// ---------- хеш-таблица с цепочками ----------
void RunHashTable(const string &filename, const string &command,
                  istringstream &input) {
  HashTable table;
  table.Load(filename);
  bool changed = true;

  if (command == "HInsert") {
    string key = ReadValue(input);
    string value = ReadValue(input);
    CheckEnd(input);
    table.HInsert(key, value);
    Out(key + ": " + value);
  } else if (command == "HDelete") {
    string key = ReadValue(input);
    CheckEnd(input);
    table.HDelete(key);
    Out(key);
  } else if (command == "HClear") {
    CheckEnd(input);
    table.HClear();
  } else {
    changed = false;
    if (command == "HGet" || command == "HContains") {
      string key = ReadValue(input);
      CheckEnd(input);
      if (command == "HGet")
        Out(table.HGet(key));
      else
        Out(BoolText(table.HContains(key)));
    } else {
      CheckEnd(input);
      if (command == "HLength")
        Out(to_string(table.HLength()));
      else if (command == "HPrint")
        table.HPrint();
      else if (command == "HPrintAddresses")
        table.HPrintAddresses();
      else
        Unknown(command);
    }
  }
  if (changed)
    table.Save(filename);
}

// ---------- открытая адресация ----------
void RunOpenTable(const string &filename, const string &command,
                  istringstream &input) {
  OpenTable table;
  table.Load(filename);
  bool changed = true;

  if (command == "OInsert") {
    string key = ReadValue(input);
    string value = ReadValue(input);
    CheckEnd(input);
    table.OInsert(key, value);
    Out(key + ": " + value);
  } else if (command == "ODelete") {
    string key = ReadValue(input);
    CheckEnd(input);
    table.ODelete(key);
    Out(key);
  } else {
    changed = false;
    if (command == "OGet" || command == "OContains") {
      string key = ReadValue(input);
      CheckEnd(input);
      if (command == "OGet")
        Out(table.OGet(key));
      else
        Out(BoolText(table.OContains(key)));
    } else {
      CheckEnd(input);
      if (command == "OLength")
        Out(to_string(table.OLength()));
      else if (command == "OPrint")
        table.OPrint();
      else
        Unknown(command);
    }
  }
  if (changed)
    table.Save(filename);
}

// по первой букве команды выбираем структуру
void ProcessCommand(const string &filename, const string &query) {
  istringstream input(query);
  string command;
  if (!(input >> command)) {
    throw invalid_argument("пустая команда");
  }
  switch (command[0]) {
  case 'F':
    RunSinglList(filename, command, input);
    break;
  case 'L':
    RunDoubleList(filename, command, input);
    break;
  case 'S':
    RunStack(filename, command, input);
    break;
  case 'Q':
    RunQueue(filename, command, input);
    break;
  case 'M':
    RunArray(filename, command, input);
    break;
  case 'T':
    RunTree(filename, command, input);
    break;
  case 'H':
    RunHashTable(filename, command, input);
    break;
  case 'O':
    RunOpenTable(filename, command, input);
    break;
  default:
    Unknown(command);
  }
}