package linkedlist

import (
	"errors"

	"lab3_go/internal/state"
)

type LNode struct {
	chislo string
	next   *LNode
	prev   *LNode
}

type DoubleList struct {
	head *LNode
	tail *LNode
}

func NewDoubleList() *DoubleList {
	return &DoubleList{head: nil, tail: nil}
}

func (l *DoubleList) LSearch(chislo string) *LNode {
	cur := l.head
	for cur != nil && cur.chislo != chislo {
		cur = cur.next
	}
	return cur
}

func (l *DoubleList) LLinkBefore(node *LNode, chislo string) {
	newNode := &LNode{chislo: chislo, next: node, prev: node.prev}
	if node.prev != nil {
		node.prev.next = newNode
	} else {
		l.head = newNode
	}
	node.prev = newNode
}

func (l *DoubleList) LLinkAfter(node *LNode, chislo string) {
	newNode := &LNode{chislo: chislo, next: node.next, prev: node}
	if node.next != nil {
		node.next.prev = newNode
	} else {
		l.tail = newNode
	}
	node.next = newNode
}

func (l *DoubleList) LDeleteNode(node *LNode) string {
	chislo := node.chislo
	if node.prev != nil {
		node.prev.next = node.next
	} else {
		l.head = node.next
	}
	if node.next != nil {
		node.next.prev = node.prev
	} else {
		l.tail = node.prev
	}
	return chislo
}

func (l *DoubleList) LAddFirst(chislo string) {
	newNode := &LNode{chislo: chislo, next: l.head, prev: nil}
	if l.head != nil {
		l.head.prev = newNode
	} else {
		l.tail = newNode
	}
	l.head = newNode
}

func (l *DoubleList) LAddEnd(chislo string) {
	newNode := &LNode{chislo: chislo, next: nil, prev: l.tail}
	if l.tail != nil {
		l.tail.next = newNode
	} else {
		l.head = newNode
	}
	l.tail = newNode
}

func (l *DoubleList) LAddAfter(target, chislo string) error {
	cur := l.LSearch(target)
	if cur == nil {
		return errors.New(target + " не найден")
	}
	l.LLinkAfter(cur, chislo)
	return nil
}

func (l *DoubleList) LAddBefore(target, chislo string) error {
	cur := l.LSearch(target)
	if cur == nil {
		return errors.New(target + " не найден")
	}
	l.LLinkBefore(cur, chislo)
	return nil
}

func (l *DoubleList) LAddIndex(chislo string, index int) error {
	cur := l.head
	i := 0
	for i < index && cur != nil {
		cur = cur.next
		i++
	}
	if index < 0 || i < index {
		return errors.New("нет такого индекса")
	}
	if cur == nil {
		l.LAddEnd(chislo)
	} else {
		l.LLinkBefore(cur, chislo)
	}
	return nil
}

func (l *DoubleList) LDeleteFirst() (string, error) {
	if l.head == nil {
		return "", errors.New("список пуст")
	}
	return l.LDeleteNode(l.head), nil
}

func (l *DoubleList) LDeleteEnd() (string, error) {
	if l.tail == nil {
		return "", errors.New("список пуст")
	}
	return l.LDeleteNode(l.tail), nil
}

func (l *DoubleList) LDeleteAfter(target string) (string, error) {
	cur := l.LSearch(target)
	if cur == nil || cur.next == nil {
		return "", errors.New("после " + target + " нечего удалять")
	}
	return l.LDeleteNode(cur.next), nil
}

func (l *DoubleList) LDeleteBefore(target string) (string, error) {
	cur := l.LSearch(target)
	if cur == nil || cur.prev == nil {
		return "", errors.New("перед " + target + " нечего удалять")
	}
	return l.LDeleteNode(cur.prev), nil
}

func (l *DoubleList) LDeleteIndex(index int) (string, error) {
	cur := l.head
	for i := 0; i < index && cur != nil; i++ {
		cur = cur.next
	}
	if index < 0 || cur == nil {
		return "", errors.New("нет такого индекса")
	}
	return l.LDeleteNode(cur), nil
}

func (l *DoubleList) LDeleteValue(chislo string) (string, error) {
	cur := l.LSearch(chislo)
	if cur == nil {
		return "", errors.New(chislo + " не найден")
	}
	return l.LDeleteNode(cur), nil
}

func (l *DoubleList) LGet(index int) (string, error) {
	cur := l.head
	for i := 0; i < index && cur != nil; i++ {
		cur = cur.next
	}
	if index < 0 || cur == nil {
		return "", errors.New("нет такого индекса")
	}
	return cur.chislo, nil
}

func (l *DoubleList) LFind(chislo string) bool {
	return l.LSearch(chislo) != nil
}

func (l *DoubleList) LPrint() string {
	result := ""
	for cur := l.head; cur != nil; cur = cur.next {
		result += cur.chislo + "\n"
	}
	return result
}

func (l *DoubleList) LPrintBack() string {
	result := ""
	for cur := l.tail; cur != nil; cur = cur.prev {
		result += cur.chislo + "\n"
	}
	return result
}

func (l *DoubleList) LClear() {
	l.head = nil
	l.tail = nil
}

func (l *DoubleList) Save(filename string) error {
	var values []string
	for cur := l.head; cur != nil; cur = cur.next {
		values = append(values, cur.chislo)
	}

	return state.Write(filename, values)
}

func (l *DoubleList) Load(filename string) error {
	var values []string
	if err := state.Read(filename, &values); err != nil {
		return err
	}

	l.LClear()
	for _, v := range values {
		l.LAddEnd(v)
	}
	return nil
}
