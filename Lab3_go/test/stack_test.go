package test

import (
	"os"
	"testing"

	"lab3_go/struct/stack"
)

func TestStack_PushPopLifo(t *testing.T) {
	s := stack.NewStack()
	if !s.SIsEmptyStack() {
		t.Error("expected empty stack")
	}
	s.SPush("a")
	s.SPush("hello world")
	if s.SIsEmptyStack() {
		t.Error("expected non-empty stack")
	}
	if v, _ := s.STop(); v != "hello world" {
		t.Errorf("expected 'hello world', got '%s'", v)
	}
	if v, _ := s.SPop(); v != "hello world" {
		t.Errorf("expected 'hello world', got '%s'", v)
	}
	if v, _ := s.SPop(); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if !s.SIsEmptyStack() {
		t.Error("expected empty stack")
	}
}

func TestStack_EmptyErrors(t *testing.T) {
	s := stack.NewStack()
	if _, err := s.SPop(); err == nil {
		t.Error("expected error on pop empty")
	}
	if _, err := s.STop(); err == nil {
		t.Error("expected error on top empty")
	}
}

func TestStack_Clear(t *testing.T) {
	s := stack.NewStack()
	s.SPush("1")
	s.SPush("2")
	s.SClear()
	if !s.SIsEmptyStack() {
		t.Error("expected empty stack after clear")
	}
}

func TestStack_Serialization(t *testing.T) {
	s1 := stack.NewStack()
	s1.SPush("bottom")
	s1.SPush("middle")
	s1.SPush("top")
	s1.Save("test_stack.json")

	s2 := stack.NewStack()
	s2.Load("test_stack.json")
	if v, _ := s2.STop(); v != "top" {
		t.Errorf("expected 'top', got '%s'", v)
	}
	if v, _ := s2.SPop(); v != "top" {
		t.Errorf("expected 'top', got '%s'", v)
	}
	if v, _ := s2.SPop(); v != "middle" {
		t.Errorf("expected 'middle', got '%s'", v)
	}
	if v, _ := s2.SPop(); v != "bottom" {
		t.Errorf("expected 'bottom', got '%s'", v)
	}
	if !s2.SIsEmptyStack() {
		t.Error("expected empty stack")
	}
	os.Remove("test_stack.json")
}
