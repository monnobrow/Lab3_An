package stack

import (
	"errors"

	"lab3_go/internal/state"
)

type SNode struct {
	chislo string
	next   *SNode
}

type Stack struct {
	head *SNode
}

func NewStack() *Stack {
	return &Stack{head: nil}
}

func (s *Stack) SIsEmptyStack() bool {
	return s.head == nil
}

func (s *Stack) SPush(chislo string) {
	s.head = &SNode{chislo: chislo, next: s.head}
}

func (s *Stack) SPop() (string, error) {
	if s.head == nil {
		return "", errors.New("стек пуст")
	}
	chislo := s.head.chislo
	s.head = s.head.next
	return chislo, nil
}

func (s *Stack) STop() (string, error) {
	if s.head == nil {
		return "", errors.New("стек пуст")
	}
	return s.head.chislo, nil
}

func (s *Stack) SPrint() string {
	result := ""
	for cur := s.head; cur != nil; cur = cur.next {
		result += cur.chislo + "\n"
	}
	return result
}

func (s *Stack) SClear() {
	s.head = nil
}

func (s *Stack) Save(filename string) error {
	var values []string
	for cur := s.head; cur != nil; cur = cur.next {
		values = append(values, cur.chislo)
	}

	return state.Write(filename, values)
}

func (s *Stack) Load(filename string) error {
	var values []string
	if err := state.Read(filename, &values); err != nil {
		return err
	}

	s.SClear()
	for i := len(values) - 1; i >= 0; i-- {
		s.SPush(values[i])
	}
	return nil
}
