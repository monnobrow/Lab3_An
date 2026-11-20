package hash

import (
	"errors"

	"lab3_go/internal/state"
)

type HNode struct {
	key   string
	value string
	next  *HNode
}

type HashTable struct {
	data  []*HNode
	size  int
	mesto int
}

func NewHashTable(mesto int) *HashTable {
	if mesto < 1 {
		mesto = 1
	}
	return &HashTable{
		data:  make([]*HNode, mesto),
		size:  0,
		mesto: mesto,
	}
}

func (h *HashTable) HHash(key string) int {
	return int(polynomialHash(key, 31) % uint(h.mesto))
}

func (h *HashTable) HResize() {
	newMesto := h.mesto * 2
	newData := make([]*HNode, newMesto)
	for i := 0; i < h.mesto; i++ {
		node := h.data[i]
		for node != nil {
			next := node.next
			index := h.HHashWithMesto(node.key, newMesto)
			node.next = newData[index]
			newData[index] = node
			node = next
		}
	}
	h.data = newData
	h.mesto = newMesto
}

func (h *HashTable) HHashWithMesto(key string, mesto int) int {
	return int(polynomialHash(key, 31) % uint(mesto))
}

func (h *HashTable) HInsert(key, value string) bool {
	index := h.HHash(key)
	node := h.data[index]
	for node != nil {
		if node.key == key {
			node.value = value
			return false
		}
		node = node.next
	}
	h.data[index] = &HNode{key: key, value: value, next: h.data[index]}
	h.size++
	if h.size >= h.mesto-h.mesto/4 {
		h.HResize()
	}
	return true
}

func (h *HashTable) HGet(key string) (string, error) {
	node := h.data[h.HHash(key)]
	for node != nil {
		if node.key == key {
			return node.value, nil
		}
		node = node.next
	}
	return "", errors.New("ключ не найден")
}

func (h *HashTable) HContains(key string) bool {
	node := h.data[h.HHash(key)]
	for node != nil {
		if node.key == key {
			return true
		}
		node = node.next
	}
	return false
}

func (h *HashTable) HDelete(key string) error {
	index := h.HHash(key)
	node := h.data[index]
	var prev *HNode
	for node != nil && node.key != key {
		prev = node
		node = node.next
	}
	if node == nil {
		return errors.New("ключ не найден")
	}
	if prev == nil {
		h.data[index] = node.next
	} else {
		prev.next = node.next
	}
	h.size--
	return nil
}

func (h *HashTable) HLength() int {
	return h.size
}

func (h *HashTable) HPrint() string {
	result := ""
	for i := 0; i < h.mesto; i++ {
		node := h.data[i]
		for node != nil {
			result += node.key + " -> " + node.value + "\n"
			node = node.next
		}
	}
	return result
}

func (h *HashTable) HClear() {
	h.data = make([]*HNode, h.mesto)
	h.size = 0
}

type entry struct {
	Key   string `json:"key"`
	Value string `json:"value"`
}
type tableState struct {
	Size     int     `json:"size"`
	Capacity int     `json:"mesto"`
	Pairs    []entry `json:"pairs"`
}

func (s tableState) valid() bool {
	return s.Capacity > 0 && s.Size == len(s.Pairs) && s.Size <= s.Capacity
}

func polynomialHash(key string, base uint) uint {
	var value uint
	for _, r := range key {
		value = value*base + uint(r)
	}
	return value
}

func (h *HashTable) Save(filename string) error {
	var pairs []entry
	for i := 0; i < h.mesto; i++ {
		node := h.data[i]
		for node != nil {
			pairs = append(pairs, entry{
				Key:   node.key,
				Value: node.value,
			})
			node = node.next
		}
	}

	return state.Write(filename, tableState{Size: h.size, Capacity: h.mesto, Pairs: pairs})
}

func (h *HashTable) Load(filename string) error {
	var saved tableState
	if err := state.Read(filename, &saved); err != nil {
		return err
	}
	if !saved.valid() {
		return errors.New("некорректные данные хеш-таблицы")
	}
	restored := NewHashTable(saved.Capacity)
	for _, p := range saved.Pairs {
		if !restored.HInsert(p.Key, p.Value) {
			return errors.New("повторяющийся ключ")
		}
	}
	*h = *restored
	return nil
}
