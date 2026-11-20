#pragma once
#include <iostream>
#include <string>
using namespace std;

class HNode {
  friend class HashTable;
  string key;
  string value;
  HNode *next;

  HNode(string key, string value, HNode *next)
      : key(key), value(value), next(next) {}
};

class HashTable {
private:
  HNode **data; // массив голов цепочек
  int size;     // сколько пар лежит
  int mesto;    // сколько ячеек

  int HHash(const string &key, int count) const;
  void HResize();

public:
  HashTable(int mesto = 8);
  ~HashTable();
  HashTable(const HashTable &) = delete;
  HashTable &operator=(const HashTable &) = delete;

  bool HInsert(const string &key, const string &value);
  string HGet(const string &key) const;
  bool HContains(const string &key) const;
  void HDelete(const string &key);
  int HLength() const;

  void HPrint() const;
  void HPrintAddresses() const;
  void HClear();

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
