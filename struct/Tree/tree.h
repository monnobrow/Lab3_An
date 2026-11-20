#pragma once
#include "utils.h"
#include <iostream>
#include <string>
using namespace std;

class TNode {
  friend class Tree;
  int data;
  int height;
  TNode *left;
  TNode *right;

  TNode(int data, int height, TNode *left, TNode *right) : data(data), height(height), left(left), right(right) {}
};

class Tree {
private:
  TNode *root;

  int THeight(const TNode *node) const;
  int TBalanceFactor(const TNode *node) const;
  void TUpdateHeight(TNode *node);
  void TRotateLeft(TNode *&node);
  void TRotateRight(TNode *&node);
  void TBalance(TNode *&node);
  int TCountNodes(const TNode *node) const;
  void TSavePreorder(TNode *node, StateWriter &out) const;

  void TAdd(TNode *&node, int chislo);
  void TDeleteMin(TNode *&node, int &chislo);
  void TDelete(TNode *&node, int chislo);

  void TPrintPryam(TNode *node) const;
  void TPrintSim(TNode *node) const;
  void TPrintObr(TNode *node) const;
  void TPrintLevel(TNode *node, int level, string &out) const;
  void TPrintTree(TNode *node, int otstup) const;
  void TPrintAddresses(TNode *node) const;
  void TClear(TNode *&node);

public:
  Tree();
  Tree(const Tree &) = delete;
  Tree &operator=(const Tree &) = delete;

  void TAdd(int chislo);
  void TDelete(int chislo);
  bool TFind(int chislo) const;
  int THeight() const;

  void TPrintPryam() const; // preorder
  void TPrintSim() const;   // inorder
  void TPrintObr() const;   // postorder
  void TPrintBfs() const;   // в ширину
  void TPrintTree() const;  // боком
  void TPrintAddresses() const;
  void TClear();
  ~Tree();

  void Save(const string &filename) const;
  void Load(const string &filename);
};
