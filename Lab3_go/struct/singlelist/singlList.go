package singlelist

import (
	"errors"

	"lab3_go/internal/state"
)

type Node struct {
	chislo string
	next   *Node
}

type SinglList struct {
	head *Node
}

func NewSinglList() *SinglList {
	return &SinglList{head: nil}
}

func (l *SinglList) FAddFirst(chislo string) {
	l.head = &Node{chislo: chislo, next: l.head}
}

func (l *SinglList) FAddEnd(chislo string) {
	newNode := &Node{chislo: chislo, next: nil}
	if l.head == nil {
		l.head = newNode
		return
	}
	cur := l.head
	for cur.next != nil {
		cur = cur.next
	}
	cur.next = newNode
}

func (l *SinglList) FAddAfter(target, chislo string) error {
	cur := l.head
	for cur != nil && cur.chislo != target {
		cur = cur.next
	}
	if cur == nil {
		return errors.New(target + " не найден")
	}
	cur.next = &Node{chislo: chislo, next: cur.next}
	return nil
}

func (l *SinglList) FAddBefore(target, chislo string) error {
	if l.head != nil && l.head.chislo == target {
		l.FAddFirst(chislo)
		return nil
	}
	cur := l.head
	for cur != nil && cur.next != nil && cur.next.chislo != target {
		cur = cur.next
	}
	if cur == nil || cur.next == nil {
		return errors.New(target + " не найден")
	}
	cur.next = &Node{chislo: chislo, next: cur.next}
	return nil
}

func (l *SinglList) FAddIndex(chislo string, index int) error {
	if index == 0 {
		l.FAddFirst(chislo)
		return nil
	}
	cur := l.head
	for i := 0; i < index-1 && cur != nil; i++ {
		cur = cur.next
	}
	if index < 0 || cur == nil {
		return errors.New("нет такого индекса")
	}
	cur.next = &Node{chislo: chislo, next: cur.next}
	return nil
}

func (l *SinglList) FDeleteFirst() (string, error) {
	if l.head == nil {
		return "", errors.New("список пуст")
	}
	chislo := l.head.chislo
	l.head = l.head.next
	return chislo, nil
}

func (l *SinglList) FDeleteEnd() (string, error) {
	if l.head == nil || l.head.next == nil {
		return l.FDeleteFirst()
	}
	cur := l.head
	for cur.next.next != nil {
		cur = cur.next
	}
	chislo := cur.next.chislo
	cur.next = nil
	return chislo, nil
}

func (l *SinglList) FDeleteAfter(target string) (string, error) {
	cur := l.head
	for cur != nil && cur.chislo != target {
		cur = cur.next
	}
	if cur == nil || cur.next == nil {
		return "", errors.New("после " + target + " нечего удалять")
	}
	chislo := cur.next.chislo
	cur.next = cur.next.next
	return chislo, nil
}

func (l *SinglList) FDeleteBefore(target string) (string, error) {
	if l.head == nil || l.head.chislo == target {
		return "", errors.New("перед " + target + " нечего удалять")
	}
	if l.head.next != nil && l.head.next.chislo == target {
		return l.FDeleteFirst()
	}
	cur := l.head
	for cur.next != nil && cur.next.next != nil && cur.next.next.chislo != target {
		cur = cur.next
	}
	if cur.next == nil || cur.next.next == nil {
		return "", errors.New("перед " + target + " нечего удалять")
	}
	chislo := cur.next.chislo
	cur.next = cur.next.next
	return chislo, nil
}

func (l *SinglList) FDeleteIndex(index int) (string, error) {
	if index == 0 {
		return l.FDeleteFirst()
	}
	cur := l.head
	for i := 0; i < index-1 && cur != nil; i++ {
		cur = cur.next
	}
	if index < 0 || cur == nil || cur.next == nil {
		return "", errors.New("нет такого индекса")
	}
	chislo := cur.next.chislo
	cur.next = cur.next.next
	return chislo, nil
}

func (l *SinglList) FDeleteValue(chislo string) (string, error) {
	if l.head != nil && l.head.chislo == chislo {
		return l.FDeleteFirst()
	}
	cur := l.head
	for cur != nil && cur.next != nil && cur.next.chislo != chislo {
		cur = cur.next
	}
	if cur == nil || cur.next == nil {
		return "", errors.New(chislo + " не найден")
	}
	result := cur.next.chislo
	cur.next = cur.next.next
	return result, nil
}

func (l *SinglList) FGet(index int) (string, error) {
	cur := l.head
	for i := 0; i < index && cur != nil; i++ {
		cur = cur.next
	}
	if index < 0 || cur == nil {
		return "", errors.New("нет такого индекса")
	}
	return cur.chislo, nil
}

func (l *SinglList) FFind(chislo string) bool {
	cur := l.head
	for cur != nil {
		if cur.chislo == chislo {
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *SinglList) FPrint() string {
	result := ""
	cur := l.head
	for cur != nil {
		result += cur.chislo + "\n"
		cur = cur.next
	}
	return result
}

func (l *SinglList) FPrintBack() string {
	var values []string
	cur := l.head
	for cur != nil {
		values = append(values, cur.chislo)
		cur = cur.next
	}
	result := ""
	for i := len(values) - 1; i >= 0; i-- {
		result += values[i] + "\n"
	}
	return result
}

func (l *SinglList) FClear() {
	l.head = nil
}

func (l *SinglList) Save(filename string) error {
	var values []string
	cur := l.head
	for cur != nil {
		values = append(values, cur.chislo)
		cur = cur.next
	}

	return state.Write(filename, values)
}

func (l *SinglList) Load(filename string) error {
	var values []string
	if err := state.Read(filename, &values); err != nil {
		return err
	}

	l.FClear()
	for i := len(values) - 1; i >= 0; i-- {
		l.FAddFirst(values[i])
	}
	return nil
}
