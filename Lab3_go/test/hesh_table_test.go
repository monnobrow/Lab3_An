package test

import (
	"os"
	"testing"

	"lab3_go/struct/hash"
)

func TestHashTable_InsertGetUpdate(t *testing.T) {
	table := hash.NewHashTable(8)
	if !table.HInsert("name", "Ann") {
		t.Error("expected true for new key")
	}
	if !table.HInsert("city", "Msk") {
		t.Error("expected true for new key")
	}
	if table.HInsert("name", "Anna") {
		t.Error("expected false for existing key")
	}
	if v, _ := table.HGet("name"); v != "Anna" {
		t.Errorf("expected 'Anna', got '%s'", v)
	}
	if table.HLength() != 2 {
		t.Errorf("expected length 2, got %d", table.HLength())
	}
}

func TestHashTable_Delete(t *testing.T) {
	table := hash.NewHashTable(8)
	table.HInsert("a", "1")
	table.HInsert("b", "2")
	table.HDelete("a")
	if table.HContains("a") {
		t.Error("expected not to contain 'a'")
	}
	if !table.HContains("b") {
		t.Error("expected to contain 'b'")
	}
	if err := table.HDelete("a"); err == nil {
		t.Error("expected error for missing key")
	}
	if _, err := table.HGet("a"); err == nil {
		t.Error("expected error for missing key")
	}
}

func TestHashTable_CollisionsAndResize(t *testing.T) {
	table := hash.NewHashTable(1)
	for i := 0; i < 100; i++ {
		table.HInsert("key"+string(rune('0'+i%10)), "v")
	}
	if table.HLength() != 10 {
		t.Errorf("expected length 10, got %d", table.HLength())
	}
}

func TestHashTable_Clear(t *testing.T) {
	table := hash.NewHashTable(8)
	table.HInsert("x", "1")
	table.HClear()
	if table.HLength() != 0 {
		t.Errorf("expected length 0, got %d", table.HLength())
	}
	if table.HContains("x") {
		t.Error("expected not to contain 'x'")
	}
	table.HInsert("y", "2")
	if v, _ := table.HGet("y"); v != "2" {
		t.Errorf("expected '2', got '%s'", v)
	}
}

func TestHashTable_Print(t *testing.T) {
	table := hash.NewHashTable(8)
	table.HInsert("k", "v")
	result := table.HPrint()
	if result != "k -> v\n" {
		t.Errorf("expected 'k -> v\\n', got '%q'", result)
	}
}

func TestHashTable_Serialization(t *testing.T) {
	table1 := hash.NewHashTable(8)
	table1.HInsert("name", "Alice")
	table1.HInsert("age", "25")
	table1.HInsert("city", "Moscow")
	table1.Save("test_ht.json")

	table2 := hash.NewHashTable(8)
	table2.Load("test_ht.json")
	if table2.HLength() != 3 {
		t.Errorf("expected length 3, got %d", table2.HLength())
	}
	if v, _ := table2.HGet("name"); v != "Alice" {
		t.Errorf("expected 'Alice', got '%s'", v)
	}
	if v, _ := table2.HGet("age"); v != "25" {
		t.Errorf("expected '25', got '%s'", v)
	}
	if v, _ := table2.HGet("city"); v != "Moscow" {
		t.Errorf("expected 'Moscow', got '%s'", v)
	}
	os.Remove("test_ht.json")
}
