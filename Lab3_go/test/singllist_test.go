package test

import (
	"lab3_go/struct/singlelist"
	"os"
	"testing"
)

func TestSinglList_AddAndGet(t *testing.T) {
	list := singlelist.NewSinglList()
	list.FAddEnd("1")
	list.FAddFirst("0")
	list.FAddAfter("1", "2")
	list.FAddBefore("0", "-1")
	if v, _ := list.FGet(0); v != "-1" {
		t.Errorf("expected '-1', got '%s'", v)
	}
	if v, _ := list.FGet(3); v != "2" {
		t.Errorf("expected '2', got '%s'", v)
	}
	if !list.FFind("1") {
		t.Error("expected to find '1'")
	}
	if list.FFind("9") {
		t.Error("expected not to find '9'")
	}
}

func TestSinglList_Delete(t *testing.T) {
	list := singlelist.NewSinglList()
	list.FAddEnd("a")
	list.FAddEnd("b")
	list.FAddEnd("c")
	if v, _ := list.FDeleteBefore("b"); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if v, _ := list.FDeleteEnd(); v != "c" {
		t.Errorf("expected 'c', got '%s'", v)
	}
	if v, _ := list.FDeleteFirst(); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	if _, err := list.FDeleteFirst(); err == nil {
		t.Error("expected error on empty list")
	}
}

func TestSinglList_Errors(t *testing.T) {
	list := singlelist.NewSinglList()
	list.FAddEnd("x")
	if err := list.FAddAfter("nope", "y"); err == nil {
		t.Error("expected error for missing target")
	}
	if _, err := list.FDeleteAfter("x"); err == nil {
		t.Error("expected error for delete after last")
	}
	if _, err := list.FDeleteBefore("x"); err == nil {
		t.Error("expected error for delete before first")
	}
	if _, err := list.FGet(5); err == nil {
		t.Error("expected error for index 5")
	}
}

func TestSinglList_IndexAndValue(t *testing.T) {
	list := singlelist.NewSinglList()
	list.FAddIndex("b", 0)
	list.FAddIndex("d", 1)
	list.FAddIndex("c", 1)
	list.FAddIndex("a", 0)
	if err := list.FAddIndex("x", 9); err == nil {
		t.Error("expected error for index 9")
	}
	if err := list.FAddIndex("x", -1); err == nil {
		t.Error("expected error for index -1")
	}
	if v, _ := list.FDeleteIndex(2); v != "c" {
		t.Errorf("expected 'c', got '%s'", v)
	}
	if v, _ := list.FDeleteIndex(0); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
}

func TestSinglList_PrintAndClear(t *testing.T) {
	list := singlelist.NewSinglList()
	list.FAddEnd("a")
	list.FAddEnd("b")
	result := list.FPrint()
	if result != "a\nb\n" {
		t.Errorf("expected 'a\\nb\\n', got '%q'", result)
	}
	list.FClear()
	if _, err := list.FGet(0); err == nil {
		t.Error("expected error after clear")
	}
}

func TestSinglList_Serialization(t *testing.T) {
	list1 := singlelist.NewSinglList()
	list1.FAddEnd("a")
	list1.FAddEnd("b")
	list1.Save("test_list.json")

	list2 := singlelist.NewSinglList()
	list2.Load("test_list.json")
	if v, _ := list2.FGet(0); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if v, _ := list2.FGet(1); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	os.Remove("test_list.json")
}
