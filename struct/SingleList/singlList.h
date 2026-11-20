#pragma once
#include "node.h"
#include <iostream>
#include <string>
using namespace std;

class SinglList {
private:
  Node *head;

  string FDeleteNext(Node *prev);
  void FPrintBack(Node *node) const;

public:
  SinglList();
  SinglList(const SinglList &) = delete;
  SinglList &operator=(const SinglList &) = delete;
  void FAddFirst(const string &chislo);
  void FAddEnd(const string &chislo);
  void FAddAfter(const string &target, const string &chislo);
  void FAddBefore(const string &target, const string &chislo);
  void FAddIndex(const string &chislo, int index);
  string FDeleteFirst();
  string FDeleteEnd();
  string FDeleteAfter(const string &target);
  string FDeleteBefore(const string &target);
  string FDeleteIndex(int index);
  string FDeleteValue(const string &chislo);
  string FGet(int index) const;
  bool FFind(const string &chislo) const;
  void FPrint() const;
  void FPrintBack() const;
  void FPrintAddresses() const;
  void FClear();
  ~SinglList();

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
