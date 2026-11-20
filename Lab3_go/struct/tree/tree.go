package tree

import (
	"errors"
	"strconv"

	"lab3_go/internal/state"
)

type TNode struct {
	data   int
	height int
	left   *TNode
	right  *TNode
}

type Tree struct {
	root *TNode
}

func NewTree() *Tree {
	return &Tree{root: nil}
}

func (t *Tree) THeight(node *TNode) int {
	if node == nil {
		return 0
	}
	return node.height
}

func (t *Tree) TBalanceFactor(node *TNode) int {
	if node == nil {
		return 0
	}
	return t.THeight(node.left) - t.THeight(node.right)
}

func (t *Tree) TUpdateHeight(node *TNode) {
	hl := t.THeight(node.left)
	hr := t.THeight(node.right)
	if hl > hr {
		node.height = hl + 1
	} else {
		node.height = hr + 1
	}
}

func (t *Tree) TRotateLeft(node **TNode) {
	newRoot := (*node).right
	(*node).right = newRoot.left
	newRoot.left = *node
	t.TUpdateHeight(*node)
	t.TUpdateHeight(newRoot)
	*node = newRoot
}

func (t *Tree) TRotateRight(node **TNode) {
	newRoot := (*node).left
	(*node).left = newRoot.right
	newRoot.right = *node
	t.TUpdateHeight(*node)
	t.TUpdateHeight(newRoot)
	*node = newRoot
}

func (t *Tree) TBalance(node **TNode) {
	if *node == nil {
		return
	}
	t.TUpdateHeight(*node)
	bf := t.TBalanceFactor(*node)
	if bf > 1 {
		if t.TBalanceFactor((*node).left) < 0 {
			t.TRotateLeft(&(*node).left)
		}
		t.TRotateRight(node)
	} else if bf < -1 {
		if t.TBalanceFactor((*node).right) > 0 {
			t.TRotateRight(&(*node).right)
		}
		t.TRotateLeft(node)
	}
}

func (t *Tree) TAdd(node **TNode, chislo int) error {
	if *node == nil {
		*node = &TNode{data: chislo, height: 1, left: nil, right: nil}
		return nil
	}
	if chislo < (*node).data {
		if err := t.TAdd(&(*node).left, chislo); err != nil {
			return err
		}
	} else if chislo > (*node).data {
		if err := t.TAdd(&(*node).right, chislo); err != nil {
			return err
		}
	} else {
		return errors.New("ключ уже существует")
	}
	t.TBalance(node)
	return nil
}

func (t *Tree) TAddRoot(chislo int) error {
	return t.TAdd(&t.root, chislo)
}

func (t *Tree) TDeleteMin(node **TNode, chislo *int) {
	if (*node).left == nil {
		*chislo = (*node).data
		*node = (*node).right
		return
	}
	t.TDeleteMin(&(*node).left, chislo)
	t.TBalance(node)
}

func (t *Tree) TDelete(node **TNode, chislo int) error {
	if *node == nil {
		return errors.New("не найден")
	}
	if chislo < (*node).data {
		if err := t.TDelete(&(*node).left, chislo); err != nil {
			return err
		}
	} else if chislo > (*node).data {
		if err := t.TDelete(&(*node).right, chislo); err != nil {
			return err
		}
	} else {
		if (*node).left != nil && (*node).right != nil {
			var minChislo int
			t.TDeleteMin(&(*node).right, &minChislo)
			(*node).data = minChislo
		} else {
			if (*node).left != nil {
				*node = (*node).left
			} else {
				*node = (*node).right
			}
			return nil
		}
	}
	t.TBalance(node)
	return nil
}

func (t *Tree) TDeleteRoot(chislo int) error {
	return t.TDelete(&t.root, chislo)
}

func (t *Tree) TFind(chislo int) bool {
	cur := t.root
	for cur != nil {
		if chislo == cur.data {
			return true
		}
		if chislo < cur.data {
			cur = cur.left
		} else {
			cur = cur.right
		}
	}
	return false
}

func (t *Tree) THeightRoot() int {
	return t.THeight(t.root)
}

func (t *Tree) TPrintSim(node *TNode) string {
	if node == nil {
		return ""
	}
	result := t.TPrintSim(node.left)
	result += strconv.Itoa(node.data) + " "
	result += t.TPrintSim(node.right)
	return result
}

func (t *Tree) TPrintSimRoot() string {
	return t.TPrintSim(t.root)
}

func (t *Tree) TClear(node **TNode) {
	if *node == nil {
		return
	}
	t.TClear(&(*node).left)
	t.TClear(&(*node).right)
	*node = nil
}

func (t *Tree) TClearRoot() {
	t.TClear(&t.root)
}

func (t *Tree) Save(filename string) error {
	var values []int
	var collect func(node *TNode)
	collect = func(node *TNode) {
		if node == nil {
			return
		}
		values = append(values, node.data)
		collect(node.left)
		collect(node.right)
	}
	collect(t.root)

	return state.Write(filename, values)
}

func (t *Tree) Load(filename string) error {
	var values []int
	if err := state.Read(filename, &values); err != nil {
		return err
	}

	restored := NewTree()
	for _, v := range values {
		if err := restored.TAddRoot(v); err != nil {
			return err
		}
	}
	t.root = restored.root
	return nil
}
