#include "hesh_table.h"
#include "utils.h"
#include <stdexcept>
using namespace std;

HashTable::HashTable(int mesto) {
  if (mesto < 1) {
    mesto = 1;
  }
  this->data = new HNode *[mesto] {}; // {} - все головы nullptr
  this->size = 0;
  this->mesto = mesto;
}

HashTable::~HashTable() {
  HClear();
  delete[] data;
}

int HashTable::HHash(const string &key, int count) const {
  unsigned int hash = 0;
  for (unsigned char c : key) {
    hash = hash * 31 + c;
  }
  return hash % count;
}

void HashTable::HResize() {
  int new_mesto = mesto * 2;
  HNode **new_data = new HNode *[new_mesto] {};
  for (int i = 0; i < mesto; ++i) {
    HNode *node = data[i];
    while (node != nullptr) {
      HNode *next = node->next;
      int index = HHash(node->key, new_mesto); // при новом mesto ячейка другая
      node->next = new_data[index];
      new_data[index] = node;
      node = next;
    }
  }
  delete[] data;
  data = new_data;
  mesto = new_mesto;
}

// true — добавили новый ключ; false — обновили существующий.
bool HashTable::HInsert(const string &key, const string &value) {
  int index = HHash(key, mesto);
  HNode *node = data[index];
  while (node != nullptr) {
    if (node->key == key) {
      node->value = value;
      return false;
    }
    node = node->next;
  }
  data[index] = new HNode(key, value, data[index]);
  ++size;
  if (size >= mesto - mesto / 4) {
    HResize();
  }
  return true;
}

string HashTable::HGet(const string &key) const {
  const HNode *node = data[HHash(key, mesto)];
  while (node != nullptr) {
    if (node->key == key) {
      return node->value;
    }
    node = node->next;
  }
  throw runtime_error("ключ " + key + " не найден");
}

bool HashTable::HContains(const string &key) const {
  const HNode *node = data[HHash(key, mesto)];
  while (node != nullptr) {
    if (node->key == key) {
      return true;
    }
    node = node->next;
  }
  return false;
}

void HashTable::HDelete(const string &key) {
  int index = HHash(key, mesto);
  HNode *node = data[index];
  HNode *previous = nullptr;
  while (node != nullptr && node->key != key) {
    previous = node;
    node = node->next;
  }
  if (node == nullptr) {
    throw runtime_error("ключ " + key + " не найден");
  }
  if (previous == nullptr) {
    data[index] = node->next;
  } else {
    previous->next = node->next;
  }
  delete node;
  --size;
}

int HashTable::HLength() const { return size; }

void HashTable::HPrint() const {
  for (int i = 0; i < mesto; ++i) {
    const HNode *node = data[i];
    while (node != nullptr) {
      cout << node->key << " -> " << node->value << '\n';
      node = node->next;
    }
  }
}

void HashTable::HPrintAddresses() const {
  for (int i = 0; i < mesto; ++i) {
    cout << '[' << i << ']';
    const HNode *node = data[i];
    while (node != nullptr) {
      cout << " -> " << node->key << ':' << node->value << " (" << node << ')';
      node = node->next;
    }
    cout << '\n';
  }
  cout << "size = " << size << " mesto = " << mesto << '\n';
}

void HashTable::HClear() {
  for (int i = 0; i < mesto; ++i) {
    HNode *node = data[i];
    while (node != nullptr) {
      HNode *next = node->next;
      delete node;
      node = next;
    }
    data[i] = nullptr;
  }
  size = 0;
}

void HashTable::Save(const string &filename) const {
  StateWriter out(filename, 'H');
  out.number(size);
  for (int i = 0; i < mesto; ++i) {
    HNode *node = data[i];
    while (node != nullptr) {
      out.text(node->key);
      out.text(node->value);
      node = node->next;
    }
  }
  out.finish();
}
void HashTable::Load(const string &filename) {
  StateReader in(filename, 'H');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::pair<std::string, std::string>> pairs;
  for (int i = 0; i < count; ++i) {
    auto key = in.text();
    pairs.emplace_back(std::move(key), in.text());
  }
  HClear();
  for (const auto &entry : pairs)
    HInsert(entry.first, entry.second);
}
