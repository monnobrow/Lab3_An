#pragma once
#include <iostream>
#include <string>
using namespace std;

class LNode {
  friend class DoubleList;
  string chislo;
  LNode *next;
  LNode *prev;
  LNode(string chislo, LNode *next, LNode *prev)
      : chislo(chislo), next(next), prev(prev) {}
};

class DoubleList {
private:
  LNode *head;
  LNode *tail;

  LNode *LSearch(const string &chislo) const;
  void LLinkBefore(LNode *node, const string &chislo);
  void LLinkAfter(LNode *node, const string &chislo);
  string LDeleteNode(LNode *node);

public:
  DoubleList();

  DoubleList(const DoubleList &) = delete;
  DoubleList &operator=(const DoubleList &) = delete;

  void LAddFirst(const string &chislo);
  void LAddEnd(const string &chislo);
  void LAddAfter(const string &target, const string &chislo);
  void LAddBefore(const string &target, const string &chislo);
  void LAddIndex(const string &chislo, int index);

  string LDeleteFirst();
  string LDeleteEnd();
  string LDeleteAfter(string target);
  string LDeleteBefore(string target);
  string LDeleteIndex(int index);
  string LDeleteValue(string chislo);

  string LGet(int index) const;
  bool LFind(const string &chislo) const;

  void LPrint() const;
  void LPrintBack() const;
  void LPrintAddresses() const;
  void LClear();
  ~DoubleList();

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
