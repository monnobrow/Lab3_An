package test

import (
	"lab3_go/struct/array"
	"os"
	"testing"
)

func TestArray_AddGetResize(t *testing.T) {
	arr := array.NewArray(1)
	arr.MAddEnd("a")
	arr.MAddEnd("b")
	arr.MAddEnd("c")
	if arr.MLength() != 3 {
		t.Errorf("expected length 3, got %d", arr.MLength())
	}
	if v, _ := arr.MGet(0); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if v, _ := arr.MGet(2); v != "c" {
		t.Errorf("expected 'c', got '%s'", v)
	}
}

func TestArray_InsertDeleteReplace(t *testing.T) {
	arr := array.NewArray(1)
	arr.MAddEnd("a")
	arr.MAddEnd("c")
	arr.MAddIndex("b", 1)
	arr.MAddIndex("d", 3)
	if v, _ := arr.MGet(1); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	if v, _ := arr.MGet(3); v != "d" {
		t.Errorf("expected 'd', got '%s'", v)
	}
	if v, _ := arr.MDeleteIndex(0); v != "a" {
		t.Errorf("expected 'a', got '%s'", v)
	}
	if v, _ := arr.MGet(0); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	arr.MReplace("x", 0)
	if v, _ := arr.MGet(0); v != "x" {
		t.Errorf("expected 'x', got '%s'", v)
	}
	if arr.MLength() != 3 {
		t.Errorf("expected length 3, got %d", arr.MLength())
	}
}

func TestArray_Errors(t *testing.T) {
	arr := array.NewArray(1)
	arr.MAddEnd("a")
	if _, err := arr.MGet(1); err == nil {
		t.Error("expected error for index 1")
	}
	if _, err := arr.MGet(-1); err == nil {
		t.Error("expected error for index -1")
	}
	if err := arr.MAddIndex("x", 5); err == nil {
		t.Error("expected error for index 5")
	}
	if _, err := arr.MDeleteIndex(3); err == nil {
		t.Error("expected error for delete index 3")
	}
	if err := arr.MReplace("x", 1); err == nil {
		t.Error("expected error for replace index 1")
	}
}

func TestArray_ClearAndReuse(t *testing.T) {
	arr := array.NewArray(1)
	arr.MAddEnd("a")
	arr.MClear()
	if arr.MLength() != 0 {
		t.Errorf("expected length 0 after clear, got %d", arr.MLength())
	}
	arr.MAddEnd("b")
	if v, _ := arr.MGet(0); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
}

func TestArray_Print(t *testing.T) {
	arr := array.NewArray(1)
	arr.MAddEnd("a")
	arr.MAddEnd("b")
	result := arr.MPrint()
	if result != "a b \n" {
		t.Errorf("expected 'a b \\n', got '%q'", result)
	}
}

func TestArray_Serialization(t *testing.T) {
	arr1 := array.NewArray(1)
	arr1.MAddEnd("hello")
	arr1.MAddEnd("world")
	arr1.Save("test_arr.json")

	arr2 := array.NewArray(1)
	arr2.Load("test_arr.json")
	if arr2.MLength() != 2 {
		t.Errorf("expected length 2, got %d", arr2.MLength())
	}
	if v, _ := arr2.MGet(0); v != "hello" {
		t.Errorf("expected 'hello', got '%s'", v)
	}
	if v, _ := arr2.MGet(1); v != "world" {
		t.Errorf("expected 'world', got '%s'", v)
	}
	os.Remove("test_arr.json")
}
