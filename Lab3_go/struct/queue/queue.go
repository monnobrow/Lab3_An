package queue

import (
	"errors"

	"lab3_go/internal/state"
)

type QNode struct {
	chislo string
	next   *QNode
}

type Queue struct {
	head *QNode
	tail *QNode
}

func NewQueue() *Queue {
	return &Queue{head: nil, tail: nil}
}

func (q *Queue) QPush(chislo string) {
	newNode := &QNode{chislo: chislo, next: nil}
	if q.head == nil {
		q.head = newNode
		q.tail = newNode
		return
	}
	q.tail.next = newNode
	q.tail = newNode
}

func (q *Queue) QPop() (string, error) {
	if q.head == nil {
		return "", errors.New("очередь пуста")
	}
	chislo := q.head.chislo
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return chislo, nil
}

func (q *Queue) QFront() (string, error) {
	if q.head == nil {
		return "", errors.New("очередь пуста")
	}
	return q.head.chislo, nil
}

func (q *Queue) QBack() (string, error) {
	if q.tail == nil {
		return "", errors.New("очередь пуста")
	}
	return q.tail.chislo, nil
}

func (q *Queue) QPrintQueue() string {
	result := ""
	for cur := q.head; cur != nil; cur = cur.next {
		result += cur.chislo + "\n"
	}
	return result
}

func (q *Queue) QClear() {
	q.head = nil
	q.tail = nil
}

func (q *Queue) Save(filename string) error {
	var values []string
	for cur := q.head; cur != nil; cur = cur.next {
		values = append(values, cur.chislo)
	}

	return state.Write(filename, values)
}

func (q *Queue) Load(filename string) error {
	var values []string
	if err := state.Read(filename, &values); err != nil {
		return err
	}

	q.QClear()
	for _, v := range values {
		q.QPush(v)
	}
	return nil
}
