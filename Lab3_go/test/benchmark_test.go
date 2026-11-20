package test

import (
	"strconv"
	"testing"

	"lab3_go/struct/array"
	"lab3_go/struct/hash"
	"lab3_go/struct/linkedlist"
	"lab3_go/struct/queue"
	"lab3_go/struct/singlelist"
	"lab3_go/struct/stack"
	"lab3_go/struct/tree"
)

func BenchmarkSequenceOperations(b *testing.B) {
	a := array.NewArray(1)
	f := singlelist.NewSinglList()
	l := linkedlist.NewDoubleList()
	q := queue.NewQueue()
	s := stack.NewStack()
	for _, item := range []struct {
		name   string
		add    func(string)
		remove func() (string, error)
	}{
		{"Array", a.MAddEnd, func() (string, error) { return a.MDeleteIndex(0) }},
		{"SingleList", f.FAddFirst, f.FDeleteFirst},
		{"DoubleList", l.LAddFirst, l.LDeleteFirst},
		{"Queue", q.QPush, q.QPop},
		{"Stack", s.SPush, s.SPop},
	} {
		b.Run(item.name, func(b *testing.B) {
			b.ReportAllocs()
			for i := 0; i < b.N; i++ {
				item.add("value")
				if _, err := item.remove(); err != nil {
					b.Fatal(err)
				}
			}
		})
	}
}

func BenchmarkLookup(b *testing.B) {
	const size = 10000
	chains := hash.NewHashTable(size * 2)
	open := hash.NewOpenTable(size*2, false, false)
	avl := tree.NewTree()
	keys := make([]string, size)
	for i := range keys {
		keys[i] = strconv.Itoa(i)
		chains.HInsert(keys[i], "value")
		open.OInsert(keys[i], "value")
		if err := avl.TAddRoot(i); err != nil {
			b.Fatal(err)
		}
	}
	for _, item := range []struct {
		name string
		find func(int) bool
	}{
		{"HashTable", func(i int) bool { return chains.HContains(keys[i]) }},
		{"OpenTable", func(i int) bool { return open.OContains(keys[i]) }},
		{"AVL", avl.TFind},
	} {
		b.Run(item.name, func(b *testing.B) {
			b.ReportAllocs()
			for i := 0; i < b.N; i++ {
				if !item.find(i % size) {
					b.Fatal("ключ не найден")
				}
			}
		})
	}
}
