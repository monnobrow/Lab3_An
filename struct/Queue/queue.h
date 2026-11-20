#pragma once
#include "node.h"
#include <iostream>
#include <string>
using namespace std;

class Queue {
private:
  Node *head;
  Node *tail;

public:
  Queue();
  Queue(const Queue &) = delete;
  Queue &operator=(const Queue &) = delete;
  void QPush(const string &chislo);
  string QPop();
  string QFront() const;
  string QBack() const;
  void QPrintQueue() const;
  void QPrintAddresses() const;
  void QClear();
  ~Queue();

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
