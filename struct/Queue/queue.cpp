#include "queue.h"
#include "utils.h"
#include <stdexcept>
using namespace std;

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue() { QClear(); }

void Queue::QPush(const string &chislo) {
  Node *new_node = new Node(chislo, nullptr);
  if (head == nullptr) {
    head = new_node;
    tail = new_node;
    return;
  }
  tail->next = new_node; // ссылку на новый
  tail = new_node;
}

string Queue::QPop() {
  if (head == nullptr) {
    throw runtime_error("очередь пуста");
  }
  Node *delete_node = head;
  string chislo = delete_node->chislo;
  head = head->next;
  delete delete_node;
  if (head == nullptr) {
    tail = nullptr;
  }
  return chislo;
}

string Queue::QFront() const {
  if (head == nullptr) {
    throw runtime_error("очередь пуста");
  }
  return head->chislo;
}

string Queue::QBack() const {
  if (tail == nullptr) {
    throw runtime_error("очередь пуста");
  }
  return tail->chislo;
}

void Queue::QPrintQueue() const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    cout << cur_node->chislo << "\n";
    cur_node = cur_node->next;
  }
}

void Queue::QPrintAddresses() const {
  const Node *cur_node = head;
  while (cur_node != nullptr) {
    cout << cur_node->chislo << " | адрес узла: " << cur_node
         << " | next: " << cur_node->next << '\n';
    cur_node = cur_node->next;
  }
  cout << '\n';
}

void Queue::QClear() {
  while (head != nullptr) {
    Node *delete_node = head;
    head = head->next;
    delete delete_node;
  }
  tail = nullptr;
}

void Queue::Save(const string &filename) const {
  StateWriter out(filename, 'Q');
  int count = 0;
  for (Node *cur = head; cur != nullptr; cur = cur->next)
    count++;
  out.number(count);
  for (Node *cur = head; cur != nullptr; cur = cur->next) {
    out.text(cur->chislo);
  }
  out.finish();
}

void Queue::Load(const string &filename) {
  StateReader in(filename, 'Q');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::string> values;
  for (int i = 0; i < count; ++i)
    values.push_back(in.text());
  QClear();
  for (const auto &value : values)
    QPush(value);
}
