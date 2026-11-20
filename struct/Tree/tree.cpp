#include "tree.h"
#include <stdexcept>
using namespace std;

Tree::Tree() : root(nullptr) {}

Tree::~Tree() { TClear(); }

int Tree::THeight(const TNode *node) const {
  if (node == nullptr) {
    return 0;
  }
  return node->height;
}

int Tree::THeight() const { return THeight(root); }

int Tree::TBalanceFactor(const TNode *node) const {
  if (node == nullptr) {
    return 0;
  }
  return THeight(node->left) - THeight(node->right);
}

void Tree::TUpdateHeight(TNode *node) {
  int hl = THeight(node->left);
  int hr = THeight(node->right);
  if (hl > hr) {
    node->height = hl + 1;
  } else {
    node->height = hr + 1;
  }
}

void Tree::TRotateLeft(TNode *&node) {
  TNode *newRoot = node->right;
  node->right = newRoot->left;
  newRoot->left = node;
  TUpdateHeight(node);
  TUpdateHeight(newRoot);
  node = newRoot;
}

void Tree::TRotateRight(TNode *&node) {
  TNode *newRoot = node->left;
  node->left = newRoot->right;
  newRoot->right = node;
  TUpdateHeight(node);
  TUpdateHeight(newRoot);
  node = newRoot;
}

void Tree::TBalance(TNode *&node) {
  if (node == nullptr) {
    return;
  }
  TUpdateHeight(node);
  int bf = TBalanceFactor(node);
  if (bf > 1) {                           // левое поддерево выше
    if (TBalanceFactor(node->left) < 0) { // LR: сначала левый поворот у сына
      TRotateLeft(node->left);
    }
    TRotateRight(node);                    // LL
  } else if (bf < -1) {                    // правое поддерево выше
    if (TBalanceFactor(node->right) > 0) { // RL: сначала правый поворот у сына
      TRotateRight(node->right);
    }
    TRotateLeft(node); // RR
  }
}

void Tree::TAdd(TNode *&node, int chislo) {
  if (node == nullptr) {
    node = new TNode(chislo, 1, nullptr, nullptr);
    return;
  }
  if (chislo < node->data) {
    TAdd(node->left, chislo);
  } else if (chislo > node->data) {
    TAdd(node->right, chislo);
  } else {
    throw runtime_error(to_string(chislo) +
                        " уже есть в дереве"); // повторы не храним
  }
  TBalance(node);
}

void Tree::TAdd(int chislo) { TAdd(root, chislo); }

void Tree::TDeleteMin(TNode *&node, int &chislo) {
  if (node->left == nullptr) {
    TNode *old = node;
    chislo = old->data;
    node = node->right;
    delete old;
    return;
  }
  TDeleteMin(node->left, chislo);
  TBalance(node);
}

void Tree::TDelete(TNode *&node, int chislo) {
  if (node == nullptr) {
    throw runtime_error(to_string(chislo) + " не найден");
  }
  if (chislo < node->data) {
    TDelete(node->left, chislo);
  } else if (chislo > node->data) {
    TDelete(node->right, chislo);
  } else {
    if (node->left != nullptr && node->right != nullptr) {
      int min_chislo;
      TDeleteMin(node->right, min_chislo);
      node->data = min_chislo;
    } else {
      TNode *old = node;
      if (node->left != nullptr) {
        node = node->left;
      } else {
        node = node->right;
      }
      delete old;
      return;
    }
  }
  TBalance(node);
}

void Tree::TDelete(int chislo) { TDelete(root, chislo); }

bool Tree::TFind(int chislo) const {
  const TNode *cur_node = root;
  while (cur_node != nullptr) {
    if (chislo == cur_node->data) {
      return true;
    }
    if (chislo < cur_node->data) {
      cur_node = cur_node->left;
    } else {
      cur_node = cur_node->right;
    }
  }
  return false;
}

void Tree::TPrintPryam(TNode *node) const {
  if (node == nullptr)
    return;
  cout << node->data << " ";
  TPrintPryam(node->left);
  TPrintPryam(node->right);
}
void Tree::TPrintPryam() const { TPrintPryam(root); }

void Tree::TPrintSim(TNode *node) const {
  if (node == nullptr)
    return;
  TPrintSim(node->left);
  cout << node->data << " ";
  TPrintSim(node->right);
}
void Tree::TPrintSim() const { TPrintSim(root); }

void Tree::TPrintObr(TNode *node) const {
  if (node == nullptr)
    return;
  TPrintObr(node->left);
  TPrintObr(node->right);
  cout << node->data << " ";
}
void Tree::TPrintObr() const { TPrintObr(root); }

void Tree::TPrintLevel(TNode *node, int level, string &out) const {
  if (node == nullptr)
    return;
  if (level == 0) {
    out += to_string(node->data) + " ";
    return;
  }
  TPrintLevel(node->left, level - 1, out);
  TPrintLevel(node->right, level - 1, out);
}

void Tree::TPrintBfs() const {
  string out;
  for (int level = 0; level < THeight(root); ++level) {
    TPrintLevel(root, level, out);
  }
  cout << out;
}

void Tree::TPrintTree(TNode *node, int otstup) const {
  if (node == nullptr)
    return;
  TPrintTree(node->right, otstup + 1);
  for (int i = 0; i < otstup; ++i) {
    cout << " ";
  }
  cout << node->data << "(h=" << node->height << ")\n";
  TPrintTree(node->left, otstup + 1);
}
void Tree::TPrintTree() const { TPrintTree(root, 0); }

void Tree::TPrintAddresses(TNode *node) const {
  if (node == nullptr)
    return;
  cout << "Значение: " << node->data << " | h: " << node->height
       << " | адрес: " << node << " | left: " << node->left
       << " | right: " << node->right << '\n';
  TPrintAddresses(node->left);
  TPrintAddresses(node->right);
}
void Tree::TPrintAddresses() const { TPrintAddresses(root); }

void Tree::TClear(TNode *&node) {
  if (node == nullptr)
    return;
  TClear(node->left);
  TClear(node->right);
  delete node;
  node = nullptr;
}
void Tree::TClear() { TClear(root); }

int Tree::TCountNodes(const TNode *node) const {
  if (node == nullptr) {
    return 0;
  }
  return 1 + TCountNodes(node->left) + TCountNodes(node->right);
}

void Tree::TSavePreorder(TNode *node, StateWriter &out) const {
  if (node == nullptr)
    return;
  out.number(node->data);
  TSavePreorder(node->left, out);
  TSavePreorder(node->right, out);
}

void Tree::Save(const string &filename) const {
  StateWriter out(filename, 'T');
  out.number(TCountNodes(root));
  TSavePreorder(root, out);
  out.finish();
}

void Tree::Load(const string &filename) {
  StateReader in(filename, 'T');
  int count;
  if (!in.count(count))
    return;
  std::vector<int> values;
  for (int i = 0; i < count; ++i)
    values.push_back(in.number());
  TClear();
  for (const auto &value : values)
    TAdd(value);
}
