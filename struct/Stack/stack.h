#pragma once
#include "node.h"
#include <iostream>
#include <string>
using namespace std;

class Stack {
private:
  Node *head;

public:
  Stack();
  Stack(const Stack &) = delete;
  Stack &operator=(const Stack &) = delete;
  bool SIsEmptyStack() const;
  void SPush(const string &chislo);
  string SPop(); // снимает вершину и сразу отдаёт её значение
  string STop() const;
  void SPrint() const;
  void SPrintAddresses() const;
  void SClear();
  ~Stack();

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
