package test

import (
	"os"
	"testing"

	"lab3_go/struct/hash"
)

func TestOpenTable_LinearInsertGetUpdate(t *testing.T) {
	table := hash.NewOpenTable(11, false, false)
	if !table.OInsert("name", "Ann") {
		t.Error("expected true for new key")
	}
	if table.OInsert("name", "Anna") {
		t.Error("expected false for existing key")
	}
	if v, _ := table.OGet("name"); v != "Anna" {
		t.Errorf("expected 'Anna', got '%s'", v)
	}
	if table.OLength() != 1 {
		t.Errorf("expected length 1, got %d", table.OLength())
	}
	if _, err := table.OGet("nope"); err == nil {
		t.Error("expected error for missing key")
	}
}

func TestOpenTable_DeletedMarkKeepsSearch(t *testing.T) {
	table := hash.NewOpenTable(11, false, true)
	table.OInsert("1", "a")
	table.OInsert("12", "b")
	table.ODelete("1")
	if table.OContains("1") {
		t.Error("expected not to contain '1'")
	}
	if v, _ := table.OGet("12"); v != "b" {
		t.Errorf("expected 'b', got '%s'", v)
	}
	if err := table.ODelete("1"); err == nil {
		t.Error("expected error for already deleted")
	}
}

func TestOpenTable_FoldingKeys(t *testing.T) {
	table := hash.NewOpenTable(11, false, true)
	table.OInsert("523456795", "fold")
	table.OInsert("abc", "text")
	if v, _ := table.OGet("523456795"); v != "fold" {
		t.Errorf("expected 'fold', got '%s'", v)
	}
	if v, _ := table.OGet("abc"); v != "text" {
		t.Errorf("expected 'text', got '%s'", v)
	}
}

func TestOpenTable_RehashAllModes(t *testing.T) {
	modes := [][2]bool{{false, false}, {true, false}, {false, true}}
	for _, m := range modes {
		table := hash.NewOpenTable(3, m[0], m[1])
		for i := 0; i < 200; i++ {
			table.OInsert(string(rune('0'+i%10)), "v")
		}
		if table.OLength() < 1 {
			t.Errorf("expected non-empty table for mode %v", m)
		}
	}
}

func TestOpenTable_ManyDeletes(t *testing.T) {
	table := hash.NewOpenTable(11, false, false)
	for i := 0; i < 100; i++ {
		key := string(rune('a' + i%26))
		table.OInsert(key, "v")
		table.ODelete(key)
	}
	if table.OLength() != 0 {
		t.Errorf("expected length 0, got %d", table.OLength())
	}
	table.OInsert("last", "ok")
	if v, _ := table.OGet("last"); v != "ok" {
		t.Errorf("expected 'ok', got '%s'", v)
	}
}

func TestOpenTable_Print(t *testing.T) {
	table := hash.NewOpenTable(3, false, false)
	table.OInsert("a", "1")
	table.ODelete("a")
	table.OInsert("b", "2")
	result := table.OPrint()
	if result != "b -> 2\n" {
		t.Errorf("expected 'b -> 2\\n', got '%q'", result)
	}
}

func TestOpenTable_Serialization(t *testing.T) {
	table1 := hash.NewOpenTable(11, false, false)
	table1.OInsert("key1", "value1")
	table1.OInsert("key2", "value2")
	table1.OInsert("key3", "value3")
	table1.Save("test_ot.json")

	table2 := hash.NewOpenTable(11, false, false)
	table2.Load("test_ot.json")
	if table2.OLength() != 3 {
		t.Errorf("expected length 3, got %d", table2.OLength())
	}
	if v, _ := table2.OGet("key1"); v != "value1" {
		t.Errorf("expected 'value1', got '%s'", v)
	}
	if v, _ := table2.OGet("key2"); v != "value2" {
		t.Errorf("expected 'value2', got '%s'", v)
	}
	if v, _ := table2.OGet("key3"); v != "value3" {
		t.Errorf("expected 'value3', got '%s'", v)
	}
	os.Remove("test_ot.json")
}
