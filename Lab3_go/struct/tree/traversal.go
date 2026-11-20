package tree

import (
	"fmt"
	"strings"
)

func (t *Tree) TPrintPryam() string {
	var out strings.Builder
	var visit func(*TNode)
	visit = func(n *TNode) {
		if n != nil {
			fmt.Fprintf(&out, "%d ", n.data)
			visit(n.left)
			visit(n.right)
		}
	}
	visit(t.root)
	return out.String()
}

func (t *Tree) TPrintObr() string {
	var out strings.Builder
	var visit func(*TNode)
	visit = func(n *TNode) {
		if n != nil {
			visit(n.left)
			visit(n.right)
			fmt.Fprintf(&out, "%d ", n.data)
		}
	}
	visit(t.root)
	return out.String()
}

func (t *Tree) TPrintBfs() string {
	var out strings.Builder
	nodes := []*TNode{t.root}
	for len(nodes) > 0 {
		n := nodes[0]
		nodes = nodes[1:]
		if n != nil {
			fmt.Fprintf(&out, "%d ", n.data)
			nodes = append(nodes, n.left, n.right)
		}
	}
	return out.String()
}

func (t *Tree) TPrintTree() string {
	var out strings.Builder
	var visit func(*TNode, int)
	visit = func(n *TNode, depth int) {
		if n != nil {
			visit(n.right, depth+1)
			fmt.Fprintf(&out, "%s%d\n", strings.Repeat("    ", depth), n.data)
			visit(n.left, depth+1)
		}
	}
	visit(t.root, 0)
	return out.String()
}
