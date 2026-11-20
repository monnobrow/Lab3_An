package test

import (
	"os"
	"testing"

	"lab3_go/struct/linkedlist"
)

func TestDoubleList_AddEverywhere(t *testing.T) {
	list := linkedlist.NewDoubleList()
	list.LAddEnd("2")
	list.LAddFirst("0")
	list.LAddBefore("2", "1")
	list.LAddAfter("2", "3")
	list.LAddIndex("4", 4)
	list.LAddIndex("-1", 0)
	if v, _ := list.LGet(0); v != "-1" {
		t.Errorf("expected '-1', got '%s'", v)
	}
	if v, _ := list.LGet(2); v != "1" {
		t.Errorf("expected '1', got '%s'", v)
	}
	if v, _ := list.LGet(5); v != "4" {
		t.Errorf("expected '4', got '%s'", v)
	}
	if !list.LFind("3") {
		t.Error("expected to find '3'")
	}
}

func TestDoubleList_DeleteEverywhere(t *testing.T) {
	list := linkedlist.NewDoubleList()
	for _, s := range []string{"a", "b", "c", "d", "e"} {
		list.LAddEnd(s)
	}
	if v, _ := list.LDeleteFirst(); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if v, _ := list.LDeleteEnd(); v != "e" {
		t.Errorf("expected 'e', got '%s'", v)
	}
	if v, _ := list.LDeleteAfter("b"); v != "c" {
		t.Errorf("expected 'c', got '%s'", v)
	}
	if v, _ := list.LDeleteBefore("d"); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	if v, _ := list.LDeleteValue("d"); v != "d" {
		t.Errorf("expected 'd', got '%s'", v)
	}
	if _, err := list.LDeleteFirst(); err == nil {
		t.Error("expected error on empty list")
	}
}

func TestDoubleList_HeadTailConsistency(t *testing.T) {
	list := linkedlist.NewDoubleList()
	list.LAddEnd("x")
	list.LDeleteIndex(0)
	list.LAddEnd("y")
	list.LAddFirst("w")
	result := list.LPrintBack()
	if result != "y\nw\n" {
		t.Errorf("expected 'y\\nw\\n', got '%q'", result)
	}
}

func TestDoubleList_Errors(t *testing.T) {
	list := linkedlist.NewDoubleList()
	list.LAddEnd("x")
	if err := list.LAddAfter("nope", "y"); err == nil {
		t.Error("expected error for missing target")
	}
	if err := list.LAddBefore("nope", "y"); err == nil {
		t.Error("expected error for missing target")
	}
	if err := list.LAddIndex("y", 5); err == nil {
		t.Error("expected error for index 5")
	}
	if err := list.LAddIndex("y", -1); err == nil {
		t.Error("expected error for index -1")
	}
	if _, err := list.LDeleteAfter("x"); err == nil {
		t.Error("expected error for delete after last")
	}
	if _, err := list.LDeleteBefore("x"); err == nil {
		t.Error("expected error for delete before first")
	}
	if _, err := list.LDeleteIndex(3); err == nil {
		t.Error("expected error for index 3")
	}
	if _, err := list.LDeleteValue("nope"); err == nil {
		t.Error("expected error for missing value")
	}
	if _, err := list.LGet(1); err == nil {
		t.Error("expected error for index 1")
	}
}

func TestDoubleList_PrintAndClear(t *testing.T) {
	list := linkedlist.NewDoubleList()
	list.LAddEnd("a")
	list.LAddEnd("b")
	result := list.LPrint()
	if result != "a\nb\n" {
		t.Errorf("expected 'a\\nb\\n', got '%q'", result)
	}
	list.LClear()
	if _, err := list.LGet(0); err == nil {
		t.Error("expected error after clear")
	}
}

func TestDoubleList_Serialization(t *testing.T) {
	list1 := linkedlist.NewDoubleList()
	list1.LAddEnd("a")
	list1.LAddEnd("b")
	list1.LAddEnd("c")
	list1.Save("test_dlist.json")

	list2 := linkedlist.NewDoubleList()
	list2.Load("test_dlist.json")
	if v, _ := list2.LGet(0); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if v, _ := list2.LGet(1); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	if v, _ := list2.LGet(2); v != "c" {
		t.Errorf("expected 'c', got '%s'", v)
	}
	os.Remove("test_dlist.json")
}
