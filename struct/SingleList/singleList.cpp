#include "singlList.h"
#include "utils.h"
#include <stdexcept>
using namespace std;

SinglList::SinglList() : head(nullptr) {}

SinglList::~SinglList() { FClear(); }

void SinglList::FAddFirst(const string &chislo) {
  Node *new_node = new Node(chislo, head);
  head = new_node;
}

void SinglList::FAddEnd(const string &chislo) {
  Node *new_node = new Node(chislo, nullptr);
  if (head == nullptr) {
    head = new_node;
    return;
  }
  Node *cur_node = head;
  while (cur_node->next != nullptr) {
    cur_node = cur_node->next;
  }
  cur_node->next = new_node;
}

// нашли target и сразу вставили, второго прохода нет
void SinglList::FAddAfter(const string &target, const string &chislo) {
  Node *cur_node = head;
  while (cur_node != nullptr && cur_node->chislo != target) {
    cur_node = cur_node->next;
  }
  if (cur_node == nullptr) {
    throw runtime_error(target + " не найден");
  }
  cur_node->next = new Node(chislo, cur_node->next);
}

// идём на шаг позади: cur_node->next - кандидат на target
void SinglList::FAddBefore(const string &target, const string &chislo) {
  if (head != nullptr && head->chislo == target) { // перед головой = в начало
    FAddFirst(chislo);
    return;
  }
  Node *cur_node = head;
  while (cur_node != nullptr && cur_node->next != nullptr &&
         cur_node->next->chislo != target) {
    cur_node = cur_node->next;
  }
  if (cur_node == nullptr || cur_node->next == nullptr) {
    throw runtime_error(target + " не найден");
  }
  cur_node->next = new Node(chislo, cur_node->next);
}

void SinglList::FAddIndex(const string &chislo, int index) {
  if (index == 0) {
    FAddFirst(chislo);
    return;
  }
  Node *cur_node = head;
  for (int i = 0; i < index - 1 && cur_node != nullptr; ++i) {
    cur_node = cur_node->next;
  }
  if (index < 0 || cur_node == nullptr) {
    throw runtime_error("нет такого индекса");
  }
  cur_node->next = new Node(chislo, cur_node->next);
}

// вырезает узел, стоящий после prev
string SinglList::FDeleteNext(Node *prev) {
  Node *delete_node = prev->next;
  string chislo = delete_node->chislo;
  prev->next = delete_node->next;
  delete delete_node;
  return chislo;
}

string SinglList::FDeleteFirst() {
  if (head == nullptr) {
    throw runtime_error("список пуст");
  }
  Node *delete_node = head;
  string chislo = delete_node->chislo;
  head = head->next;
  delete delete_node;
  return chislo;
}

string SinglList::FDeleteEnd() {
  if (head == nullptr || head->next == nullptr) {
    return FDeleteFirst();
  }
  Node *cur_node = head;
  while (cur_node->next->next != nullptr) {
    cur_node = cur_node->next;
  }
  return FDeleteNext(cur_node);
}

string SinglList::FDeleteAfter(const string &target) {
  Node *cur_node = head;
  while (cur_node != nullptr && cur_node->chislo != target) {
    cur_node = cur_node->next;
  }
  if (cur_node == nullptr || cur_node->next == nullptr) {
    throw runtime_error("после " + target + " нечего удалять");
  }
  return FDeleteNext(cur_node);
}

// удаляем узел перед target: идём на два шага позади
string SinglList::FDeleteBefore(const string &target) {
  if (head == nullptr || head->chislo == target) { // перед головой ничего нет
    throw runtime_error("перед " + target + " нечего удалять");
  }
  if (head->next != nullptr && head->next->chislo == target) {
    return FDeleteFirst();
  }
  Node *cur_node = head;
  while (cur_node->next != nullptr && cur_node->next->next != nullptr &&
         cur_node->next->next->chislo != target) {
    cur_node = cur_node->next;
  }
  if (cur_node->next == nullptr || cur_node->next->next == nullptr) {
    throw runtime_error("перед " + target + " нечего удалять");
  }
  return FDeleteNext(cur_node);
}

string SinglList::FDeleteIndex(int index) {
  if (index == 0) {
    return FDeleteFirst();
  }
  Node *cur_node = head;
  for (int i = 0; i < index - 1 && cur_node != nullptr; ++i) {
    cur_node = cur_node->next;
  }
  if (index < 0 || cur_node == nullptr || cur_node->next == nullptr) {
    throw runtime_error("нет такого индекса");
  }
  return FDeleteNext(cur_node);
}

string SinglList::FDeleteValue(const string &chislo) {
  if (head != nullptr && head->chislo == chislo) {
    return FDeleteFirst();
  }
  Node *cur_node = head;
  while (cur_node != nullptr && cur_node->next != nullptr &&
         cur_node->next->chislo != chislo) {
    cur_node = cur_node->next;
  }
  if (cur_node == nullptr || cur_node->next == nullptr) {
    throw runtime_error(chislo + " не найден");
  }
  return FDeleteNext(cur_node);
}

string SinglList::FGet(int index) const {
  const Node *cur_node = head;
  for (int i = 0; i < index && cur_node != nullptr; ++i) {
    cur_node = cur_node->next;
  }
  if (index < 0 || cur_node == nullptr) {
    throw runtime_error("нет такого индекса");
  }
  return cur_node->chislo;
}

bool SinglList::FFind(const string &chislo) const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    if (cur_node->chislo == chislo) {
      return true;
    }
    cur_node = cur_node->next;
  }
  return false;
}

void SinglList::FPrintBack(Node *node) const {
  if (node == nullptr)
    return;
  FPrintBack(node->next);
  cout << node->chislo << "\n";
}

void SinglList::FPrintBack() const { FPrintBack(head); }

void SinglList::FPrint() const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    cout << cur_node->chislo << "\n";
    cur_node = cur_node->next;
  }
}

void SinglList::FPrintAddresses() const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    cout << "Узел: " << cur_node << ", chislo: " << cur_node->chislo
         << ", next: " << cur_node->next << '\n';
    cur_node = cur_node->next;
  }
}

void SinglList::FClear() {
  while (head != nullptr) {
    Node *delete_node = head;
    head = head->next;
    delete delete_node;
  }
}

void SinglList::Save(const string &filename) const {
  StateWriter out(filename, 'F');
  int count = 0;
  for (Node *cur = head; cur != nullptr; cur = cur->next)
    count++;
  out.number(count);
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    out.text(cur->chislo);
  }
  out.finish();
}

void SinglList::Load(const string &filename) {
  StateReader in(filename, 'F');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::string> values;
  for (int i = 0; i < count; ++i)
    values.push_back(in.text());
  FClear();
  for (auto value = values.rbegin(); value != values.rend(); ++value)
    FAddFirst(*value);
}
