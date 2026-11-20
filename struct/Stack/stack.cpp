#include "stack.h"
#include "utils.h"
#include <stdexcept>
#include <vector>
using namespace std;

Stack::Stack() : head(nullptr) {}

bool Stack::SIsEmptyStack() const { return head == nullptr; }

void Stack::SPush(const string &chislo) {
  Node *new_node = new Node(chislo, head);
  head = new_node;
}

string Stack::SPop() {
  if (SIsEmptyStack()) {
    throw runtime_error("стек пуст");
  }
  Node *delete_node = head;
  string chislo = delete_node->chislo;
  head = head->next;
  delete delete_node;
  return chislo;
}

string Stack::STop() const {
  if (SIsEmptyStack()) {
    throw runtime_error("стек пуст");
  }
  return head->chislo;
}

void Stack::SPrint() const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    cout << cur_node->chislo << "\n";
    cur_node = cur_node->next;
  }
}

void Stack::SPrintAddresses() const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    cout << cur_node->chislo << " | адрес узла: " << cur_node
         << " | next: " << cur_node->next << '\n';
    cur_node = cur_node->next;
  }
  cout << '\n';
}

void Stack::SClear() {
  while (head != nullptr) {
    Node *delete_node = head;
    head = head->next;
    delete delete_node;
  }
}

Stack::~Stack() { SClear(); }

void Stack::Save(const string &filename) const {
  StateWriter out(filename, 'S');
  int count = 0;
  for (Node *cur = head; cur != nullptr; cur = cur->next)
    count++;
  out.number(count);
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    out.text(cur->chislo);
  }
  out.finish();
}

void Stack::Load(const string &filename) {
  StateReader in(filename, 'S');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::string> values;
  for (int i = 0; i < count; ++i)
    values.push_back(in.text());
  SClear();
  for (auto value = values.rbegin(); value != values.rend(); ++value)
    SPush(*value);
}
