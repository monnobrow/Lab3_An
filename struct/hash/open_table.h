#pragma once
#include "../Array/array.h"
#include <iostream>
#include <string>
using namespace std;

class OpenTable {
private:
  Array *keys;
  Array *values;
  Array *states;
  int size;
  int mesto;
  int deleted;
  bool double_hash;
  bool folding;

  bool IsPrime(int value) const;
  int NextPrime(int value) const;
  int HHash(const string &key) const;
  int FoldHash(const string &key) const;
  int OpenStart(const string &key) const;
  int OpenStep(const string &key) const;
  int OpenFind(const string &key) const;
  void OCreate(int new_mesto);
  void OPlace(const string &key, const string &value);
  void OpenRehash(int new_mesto);

public:
  OpenTable(int mesto = 11, bool double_hash = false, bool folding = false);
  ~OpenTable();
  OpenTable(const OpenTable &) = delete;
  OpenTable &operator=(const OpenTable &) = delete;

  bool OInsert(const string &key, const string &value);
  string OGet(const string &key) const;
  bool OContains(const string &key) const;
  void ODelete(const string &key);
  int OLength() const;
  void OPrint() const;

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
