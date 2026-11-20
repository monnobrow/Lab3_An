#pragma once
#include <string>
using namespace std;

class SinglList;
class Stack;
class Queue;

class Node {
    friend class SinglList;
    friend class Stack;
    friend class Queue;
    string chislo;
    Node* next;
    Node(string chislo, Node* next) : chislo(chislo), next(next) {}
};
