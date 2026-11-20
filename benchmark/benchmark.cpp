#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#include "../struct/Array/array.h"
#include "../struct/LinkedList/doublelist.h"
#include "../struct/Queue/queue.h"
#include "../struct/SingleList/singlList.h"
#include "../struct/Stack/stack.h"
#include "../struct/Tree/tree.h"
#include "../struct/hash/hesh_table.h"
#include "../struct/hash/open_table.h"
using namespace std;
using namespace std::chrono;

const int N = 10000;
long long checksum = 0;

steady_clock::time_point Start() { return steady_clock::now(); }

string Pad(const string &text, int width) {
  int symbols = 0;
  for (unsigned char c : text) {
    if ((c & 0xC0) != 0x80)
      ++symbols;
  }
  string result = text;
  while (symbols < width) {
    result += ' ';
    ++symbols;
  }
  return result;
}

// печатает строку таблицы: структура, операция, всего мс, нс на операцию
void Report(const string &structure, const string &operation, int count, steady_clock::time_point start) {
  duration<double, milli> total = steady_clock::now() - start;
  double per_op = total.count() * 1000000.0 / count;
  cout << Pad(structure, 14) << Pad(operation, 26) << right << fixed << setprecision(3) << setw(12) << total.count() << " мс" << setprecision(1) << setw(12) << per_op << " нс/оп\n";
}

void BenchArray() {
  Array arr;
  auto t = Start();
  for (int i = 0; i < N; ++i)
    arr.MAddEnd(to_string(i));
  Report("Array", "MAddEnd O(1)*", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    checksum += arr.MGet(i).size();
  Report("Array", "MGet O(1)", N, t);

  Array front;
  t = Start();
  for (int i = 0; i < N; ++i)
    front.MAddIndex(to_string(i), 0);
  Report("Array", "MAddIndex(0) O(n)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    front.MDeleteIndex(0);
  Report("Array", "MDeleteIndex(0) O(n)", N, t);
}

void BenchSinglList() {
  SinglList list;
  auto t = Start();
  for (int i = 0; i < N; ++i)
    list.FAddFirst(to_string(i));
  Report("SinglList", "FAddFirst O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    list.FDeleteFirst();
  Report("SinglList", "FDeleteFirst O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    list.FAddEnd(to_string(i));
  Report("SinglList", "FAddEnd O(n)", N, t);

  const int finds = 1000;
  t = Start();
  for (int i = 0; i < finds; ++i)
    checksum += list.FFind(to_string(N - 1));
  Report("SinglList", "FFind (последний) O(n)", finds, t);
}

void BenchDoubleList() {
  DoubleList list;
  auto t = Start();
  for (int i = 0; i < N; ++i)
    list.LAddEnd(to_string(i));
  Report("DoubleList", "LAddEnd O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    list.LDeleteEnd();
  Report("DoubleList", "LDeleteEnd O(1)", N, t);
}

void BenchStack() {
  Stack stack;
  auto t = Start();
  for (int i = 0; i < N; ++i)
    stack.SPush(to_string(i));
  Report("Stack", "SPush O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    checksum += stack.SPop().size();
  Report("Stack", "SPop O(1)", N, t);
}

void BenchQueue() {
  Queue queue;
  auto t = Start();
  for (int i = 0; i < N; ++i)
    queue.QPush(to_string(i));
  Report("Queue", "QPush O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    checksum += queue.QPop().size();
  Report("Queue", "QPop O(1)", N, t);
}

void BenchTree() {
  Tree tree;
  // по возрастанию: обычное BST выродилось бы в список, АВЛ остаётся log n
  auto t = Start();
  for (int i = 0; i < N; ++i)
    tree.TAdd(i);
  Report("Tree (AVL)", "TAdd O(log n)", N, t);
  cout << "              высота после " << N << " вставок: " << tree.THeight()
       << '\n';

  t = Start();
  for (int i = 0; i < N; ++i)
    checksum += tree.TFind(i);
  Report("Tree (AVL)", "TFind O(log n)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    tree.TDelete(i);
  Report("Tree (AVL)", "TDelete O(log n)", N, t);
}

void BenchHashTable() {
  HashTable table;
  auto t = Start();
  for (int i = 0; i < N; ++i)
    table.HInsert("key" + to_string(i), to_string(i));
  Report("HashTable", "HInsert O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    checksum += table.HGet("key" + to_string(i)).size();
  Report("HashTable", "HGet O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    table.HDelete("key" + to_string(i));
  Report("HashTable", "HDelete O(1)", N, t);
}

void BenchOpenTable(const string &name, bool double_hash, bool folding) {
  OpenTable table(11, double_hash, folding);
  auto t = Start();
  for (int i = 0; i < N; ++i)
    table.OInsert(to_string(i * 7), to_string(i));
  Report(name, "OInsert O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    checksum += table.OGet(to_string(i * 7)).size();
  Report(name, "OGet O(1)", N, t);

  t = Start();
  for (int i = 0; i < N; ++i)
    table.ODelete(to_string(i * 7));
  Report(name, "ODelete O(1)", N, t);
}

int main() {
  cout << "Бенчмарк, N = " << N << " операций в каждом замере\n";
  cout << Pad("Структура", 14) << Pad("Операция", 26)
       << Pad("         Всего", 18) << "На операцию\n";
  cout << string(73, '-') << '\n';

  BenchArray();
  BenchSinglList();
  BenchDoubleList();
  BenchStack();
  BenchQueue();
  BenchTree();
  BenchHashTable();
  BenchOpenTable("Open linear", false, false);
  BenchOpenTable("Open double", true, false);
  BenchOpenTable("Open folding", false, true);

  cout << string(73, '-') << '\n';
  cout << "* MAddEnd O(1) в среднем: при заполнении массив удваивается\n";
  cout << "Свёртка: сумма кусков по 3 цифры у ключей до 70000 не больше "
          "~1100,\n";
  cout << "поэтому все ключи стартуют в первых ~1100 ячейках и образуют "
          "длинные кластеры\n";
  cout << "checksum = " << checksum << '\n';
  return 0;
}
