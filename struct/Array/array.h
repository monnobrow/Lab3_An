#pragma once
#include <iostream>
#include <string>
using namespace std;

class Array {
private:
  string *data;
  int size;
  int mesto;
  void MResize();
  void MCheckIndex(int index) const;

public:
  Array(int mesto = 1);
  Array(const Array &) = delete;
  Array &operator=(const Array &) = delete;
  void MAddEnd(const string &chislo);
  void MAddIndex(const string &chislo, int index);
  string MGet(int index) const;
  string MDeleteIndex(int index);
  void MReplace(const string &chislo, int index);
  int MLength() const;
  void MPrint() const;
  void MPrintAddresses() const;
  void MClear();
  ~Array();

  // сериализация: пока заглушки
  void Save(const string &filename) const;
  void Load(const string &filename);
};
