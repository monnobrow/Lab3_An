#include "doublelist.h"
#include "utils.h"
#include <stdexcept>
using namespace std;

DoubleList::DoubleList() : head(nullptr), tail(nullptr) {}

DoubleList::~DoubleList() { LClear(); }

LNode *DoubleList::LSearch(const string &chislo) const {
  LNode *cur_node = head;
  while (cur_node != nullptr && cur_node->chislo != chislo) {
    cur_node = cur_node->next;
  }
  return cur_node;
}

// вставка рядом с узлом и удаление узла - O(1), есть prev
void DoubleList::LLinkBefore(LNode *node, const string &chislo) {
  LNode *new_node = new LNode(chislo, node, node->prev);
  if (node->prev != nullptr) {
    node->prev->next = new_node;
  } else {
    head = new_node;
  }
  node->prev = new_node;
}

void DoubleList::LLinkAfter(LNode *node, const string &chislo) {
  LNode *new_node = new LNode(chislo, node->next, node);
  if (node->next != nullptr) {
    node->next->prev = new_node;
  } else {
    tail = new_node;
  }
  node->next = new_node;
}

string DoubleList::LDeleteNode(LNode *node) {
  string chislo = node->chislo;
  if (node->prev != nullptr) {
    node->prev->next = node->next;
  } else {
    head = node->next;
  }
  if (node->next != nullptr) {
    node->next->prev = node->prev;
  } else {
    tail = node->prev;
  }
  delete node;
  return chislo;
}

void DoubleList::LAddFirst(const string &chislo) {
  LNode *new_node = new LNode(chislo, head, nullptr);
  if (head != nullptr) {
    head->prev = new_node;
  } else {
    tail = new_node;
  }
  head = new_node;
}

void DoubleList::LAddEnd(const string &chislo) {
  LNode *new_node = new LNode(chislo, nullptr, tail);
  if (tail != nullptr) {
    tail->next = new_node;
  } else {
    head = new_node;
  }
  tail = new_node;
}

void DoubleList::LAddAfter(const string &target, const string &chislo) {
  LNode *cur_node = LSearch(target);
  if (cur_node == nullptr) {
    throw runtime_error(target + " не найден");
  }
  LLinkAfter(cur_node, chislo);
}

void DoubleList::LAddBefore(const string &target, const string &chislo) {
  LNode *cur_node = LSearch(target);
  if (cur_node == nullptr) {
    throw runtime_error(target + " не найден");
  }
  LLinkBefore(cur_node, chislo);
}

// index == длина - вставка в конец
void DoubleList::LAddIndex(const string &chislo, int index) {
  LNode *cur_node = head;
  int i = 0;
  while (i < index && cur_node != nullptr) {
    cur_node = cur_node->next;
    i++;
  }
  if (index < 0 || i < index) { // список короче индекса
    throw runtime_error("нет такого индекса");
  }
  if (cur_node == nullptr) {
    LAddEnd(chislo);
  } else {
    LLinkBefore(cur_node, chislo);
  }
}

string DoubleList::LDeleteFirst() {
  if (head == nullptr) {
    throw runtime_error("список пуст");
  }
  return LDeleteNode(head);
}

string DoubleList::LDeleteEnd() {
  if (tail == nullptr) {
    throw runtime_error("список пуст");
  }
  return LDeleteNode(tail);
}

string DoubleList::LDeleteAfter(string target) {
  LNode *cur_node = LSearch(target);
  if (cur_node == nullptr || cur_node->next == nullptr) {
    throw runtime_error("после " + target + " нечего удалять");
  }
  return LDeleteNode(cur_node->next);
}

string DoubleList::LDeleteBefore(string target) {
  LNode *cur_node = LSearch(target);
  if (cur_node == nullptr || cur_node->prev == nullptr) {
    throw runtime_error("перед " + target + " нечего удалять");
  }
  return LDeleteNode(cur_node->prev);
}

string DoubleList::LDeleteIndex(int index) {
  LNode *cur_node = head;
  for (int i = 0; i < index && cur_node != nullptr; ++i) {
    cur_node = cur_node->next;
  }
  if (index < 0 || cur_node == nullptr) {
    throw runtime_error("нет такого индекса");
  }
  return LDeleteNode(cur_node);
}

string DoubleList::LDeleteValue(string chislo) {
  LNode *cur_node = LSearch(chislo);
  if (cur_node == nullptr) {
    throw runtime_error(chislo + " не найден");
  }
  return LDeleteNode(cur_node);
}

string DoubleList::LGet(int index) const {
  const LNode *cur_node = head;
  for (int i = 0; i < index && cur_node != nullptr; ++i) {
    cur_node = cur_node->next;
  }
  if (index < 0 || cur_node == nullptr) {
    throw runtime_error("нет такого индекса");
  }
  return cur_node->chislo;
}

bool DoubleList::LFind(const string &chislo) const {
  return LSearch(chislo) != nullptr;
}

void DoubleList::LPrint() const {
  for (const LNode *cur_node = head; cur_node != nullptr;
       cur_node = cur_node->next) {
    cout << cur_node->chislo << "\n";
  }
}

void DoubleList::LPrintBack() const {
  for (const LNode *cur_node = tail; cur_node != nullptr;
       cur_node = cur_node->prev) {
    cout << cur_node->chislo << "\n";
  }
}

void DoubleList::LPrintAddresses() const {
  for (const LNode *cur_node = head; cur_node != nullptr;
       cur_node = cur_node->next) {
    cout << cur_node->chislo << " | адрес: " << cur_node
         << " | next: " << cur_node->next << " | prev: " << cur_node->prev
         << '\n';
  }
  cout << '\n';
}

void DoubleList::LClear() {
  while (head != nullptr) {
    LNode *delete_node = head;
    head = head->next;
    delete delete_node;
  }
  tail = nullptr;
}

void DoubleList::Save(const string &filename) const {
  StateWriter out(filename, 'L');
  int count = 0;
  for (LNode *cur = head; cur != nullptr; cur = cur->next)
    count++;
  out.number(count);
  for (LNode *cur = head; cur != nullptr; cur = cur->next) {
    out.text(cur->chislo);
  }
  out.finish();
}

void DoubleList::Load(const string &filename) {
  StateReader in(filename, 'L');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::string> values;
  for (int i = 0; i < count; ++i)
    values.push_back(in.text());
  LClear();
  for (const auto &value : values)
    LAddEnd(value);
}
