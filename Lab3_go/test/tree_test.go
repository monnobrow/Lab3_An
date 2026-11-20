package test

import (
	"os"
	"testing"

	"lab3_go/struct/tree"
)

func fillExample(tr *tree.Tree) {
	values := []int{10, 20, 30, 25, 27, 5, 4}
	for _, v := range values {
		tr.TAddRoot(v)
	}
}

func TestTree_Traversals(t *testing.T) {
	tr := tree.NewTree()
	fillExample(tr)
	result := tr.TPrintSimRoot()
	if result != "4 5 10 20 25 27 30 " {
		t.Errorf("expected inorder '4 5 10 20 25 27 30 ', got '%s'", result)
	}
	if tr.THeightRoot() != 3 {
		t.Errorf("expected height 3, got %d", tr.THeightRoot())
	}
}

func TestTree_RotationRR(t *testing.T) {
	tr := tree.NewTree()
	tr.TAddRoot(10)
	tr.TAddRoot(20)
	tr.TAddRoot(30)
	result := tr.TPrintSimRoot()
	if result != "10 20 30 " {
		t.Errorf("expected '10 20 30 ', got '%s'", result)
	}
	if tr.THeightRoot() != 2 {
		t.Errorf("expected height 2, got %d", tr.THeightRoot())
	}
}

func TestTree_FindAndErrors(t *testing.T) {
	tr := tree.NewTree()
	fillExample(tr)
	if !tr.TFind(27) {
		t.Error("expected to find 27")
	}
	if tr.TFind(99) {
		t.Error("expected not to find 99")
	}
	if err := tr.TAddRoot(27); err == nil {
		t.Error("expected error for duplicate")
	}
	if err := tr.TDeleteRoot(99); err == nil {
		t.Error("expected error for missing value")
	}
}

func TestTree_Clear(t *testing.T) {
	tr := tree.NewTree()
	fillExample(tr)
	tr.TClearRoot()
	if tr.THeightRoot() != 0 {
		t.Errorf("expected height 0, got %d", tr.THeightRoot())
	}
	if tr.TFind(10) {
		t.Error("expected not to find 10 after clear")
	}
}

func TestTree_Serialization(t *testing.T) {
	tr1 := tree.NewTree()
	tr1.TAddRoot(10)
	tr1.TAddRoot(5)
	tr1.TAddRoot(20)
	tr1.Save("test_tree.json")

	tr2 := tree.NewTree()
	tr2.Load("test_tree.json")
	if !tr2.TFind(10) {
		t.Error("expected to find 10")
	}
	if !tr2.TFind(5) {
		t.Error("expected to find 5")
	}
	if !tr2.TFind(20) {
		t.Error("expected to find 20")
	}
	os.Remove("test_tree.json")
}
