#include "array.h"
#include "utils.h"
#include <stdexcept>
using namespace std;

Array::Array(int mesto) {
  if (mesto < 1) {
    mesto = 1;
  }
  this->data = new string[mesto];
  this->size = 0;
  this->mesto = mesto;
}

Array::~Array() { delete[] data; }

void Array::MResize() {
  string *new_data = new string[mesto * 2];
  for (int i = 0; i < size; ++i) {
    new_data[i] = data[i];
  }
  delete[] data;
  data = new_data;
  mesto = mesto * 2;
}

void Array::MCheckIndex(int index) const {
  if (index < 0 || index >= size) {
    throw runtime_error("нет такого индекса");
  }
}

void Array::MAddEnd(const string &chislo) {
  if (size == mesto) {
    MResize();
  }
  data[size] = chislo;
  size++; // O(1)
}

void Array::MAddIndex(const string &chislo, int index) {
  if (index < 0 || index > size) { // index == size - вставка в конец
    throw runtime_error("нет такого индекса");
  }
  if (size == mesto) {
    MResize();
  }
  for (int i = size; i > index; --i) {
    data[i] = data[i - 1]; // чтобы не затереть элементы
  }
  data[index] = chislo;
  size++; // O(n)
}

string Array::MGet(int index) const {
  MCheckIndex(index);
  return data[index]; // O(1)
}

string Array::MDeleteIndex(int index) {
  MCheckIndex(index);
  string chislo = data[index];
  for (int i = index; i < size - 1; ++i) {
    data[i] = data[i + 1];
  }
  size--; // O(n)
  return chislo;
}

void Array::MReplace(const string &chislo, int index) {
  MCheckIndex(index);
  data[index] = chislo; // O(1)
}

int Array::MLength() const {
  return size; // O(1)
}

void Array::MPrint() const {
  for (int i = 0; i < size; ++i) {
    cout << data[i] << " ";
  }
  cout << "\n";
}

void Array::MPrintAddresses() const {
  for (int i = 0; i < size; ++i) {
    cout << "[" << i << "] " << data[i] << " | адрес: " << &data[i] << '\n';
  }
  cout << "size = " << size << " mesto = " << mesto << "\n\n";
}

void Array::MClear() {
  size = 0; // память остаётся, её освободит деструктор
}
void Array::Save(const string &filename) const {
  StateWriter out(filename, 'A');
  out.number(size);
  for (int i = 0; i < size; ++i) {
    out.text(data[i]);
  }
  out.finish();
}

void Array::Load(const string &filename) {
  StateReader in(filename, 'A');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::string> values;
  for (int i = 0; i < count; ++i)
    values.push_back(in.text());
  MClear();
  for (const auto &value : values)
    MAddEnd(value);
}
