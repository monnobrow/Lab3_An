package test

import (
	"os"
	"testing"

	"lab3_go/struct/queue"
)

func TestQueue_PushPopFifo(t *testing.T) {
	q := queue.NewQueue()
	q.QPush("1")
	q.QPush("2")
	q.QPush("3")
	if v, _ := q.QFront(); v != "1" {
		t.Errorf("expected '1', got '%s'", v)
	}
	if v, _ := q.QBack(); v != "3" {
		t.Errorf("expected '3', got '%s'", v)
	}
	if v, _ := q.QPop(); v != "1" {
		t.Errorf("expected '1', got '%s'", v)
	}
	if v, _ := q.QPop(); v != "2" {
		t.Errorf("expected '2', got '%s'", v)
	}
	if v, _ := q.QFront(); v != "3" {
		t.Errorf("expected '3', got '%s'", v)
	}
}

func TestQueue_TailResetAfterLastPop(t *testing.T) {
	q := queue.NewQueue()
	q.QPush("a")
	q.QPop()
	if _, err := q.QBack(); err == nil {
		t.Error("expected error on empty queue")
	}
	q.QPush("b")
	if v, _ := q.QFront(); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	if v, _ := q.QBack(); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
}

func TestQueue_EmptyErrors(t *testing.T) {
	q := queue.NewQueue()
	if _, err := q.QPop(); err == nil {
		t.Error("expected error on pop empty")
	}
	if _, err := q.QFront(); err == nil {
		t.Error("expected error on front empty")
	}
	if _, err := q.QBack(); err == nil {
		t.Error("expected error on back empty")
	}
}

func TestQueue_Clear(t *testing.T) {
	q := queue.NewQueue()
	q.QPush("x")
	q.QPush("y")
	q.QClear()
	if _, err := q.QPop(); err == nil {
		t.Error("expected error after clear")
	}
}

func TestQueue_Serialization(t *testing.T) {
	q1 := queue.NewQueue()
	q1.QPush("first")
	q1.QPush("second")
	q1.QPush("third")
	q1.Save("test_queue.json")

	q2 := queue.NewQueue()
	q2.Load("test_queue.json")
	if v, _ := q2.QFront(); v != "first" {
		t.Errorf("expected 'first', got '%s'", v)
	}
	if v, _ := q2.QBack(); v != "third" {
		t.Errorf("expected 'third', got '%s'", v)
	}
	if v, _ := q2.QPop(); v != "first" {
		t.Errorf("expected 'first', got '%s'", v)
	}
	if v, _ := q2.QPop(); v != "second" {
		t.Errorf("expected 'second', got '%s'", v)
	}
	if v, _ := q2.QPop(); v != "third" {
		t.Errorf("expected 'third', got '%s'", v)
	}
	os.Remove("test_queue.json")
}
