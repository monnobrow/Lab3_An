package hash

import (
	"errors"
	"strconv"

	"lab3_go/internal/state"
)

type CellState int

const (
	Empty CellState = iota
	Occupied
	Deleted
)

type OpenTable struct {
	keys       []string
	values     []string
	states     []CellState
	size       int
	mesto      int
	doubleHash bool
	folding    bool
}

func NewOpenTable(mesto int, doubleHash, folding bool) *OpenTable {
	if mesto < 1 {
		mesto = 1
	}
	return &OpenTable{
		keys:       make([]string, mesto),
		values:     make([]string, mesto),
		states:     make([]CellState, mesto),
		size:       0,
		mesto:      mesto,
		doubleHash: doubleHash,
		folding:    folding,
	}
}

func (o *OpenTable) OHash1(key string) int {
	return int(polynomialHash(key, 31) % uint(o.mesto))
}

func (o *OpenTable) OHash2(key string) int {
	if o.mesto == 1 {
		return 1
	}
	step := 1 + int(polynomialHash(key, 37)%uint(o.mesto-1))
	for gcd(step, o.mesto) != 1 {
		step++
	}
	return step
}

func gcd(a, b int) int {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func (o *OpenTable) OFolding(key string) int {
	n, err := strconv.Atoi(key)
	if err != nil {
		return o.OHash1(key)
	}
	sum := 0
	for n > 0 {
		sum += n % 1000
		n /= 1000
	}
	return sum % o.mesto
}

func (o *OpenTable) OProbe(key string, i int) int {
	if o.folding {
		return (o.OFolding(key) + i) % o.mesto
	}
	if o.doubleHash {
		return (o.OHash1(key) + i*o.OHash2(key)) % o.mesto
	}
	return (o.OHash1(key) + i) % o.mesto
}

func (o *OpenTable) OResize() {
	newMesto := o.mesto * 2
	newKeys := make([]string, newMesto)
	newValues := make([]string, newMesto)
	newStates := make([]CellState, newMesto)

	oldKeys := o.keys
	oldValues := o.values
	oldStates := o.states
	oldMesto := o.mesto

	o.keys = newKeys
	o.values = newValues
	o.states = newStates
	o.mesto = newMesto
	o.size = 0

	for i := 0; i < oldMesto; i++ {
		if oldStates[i] == Occupied {
			o.OInsert(oldKeys[i], oldValues[i])
		}
	}
}

func (o *OpenTable) OInsert(key, value string) bool {
	free := -1
	for i := 0; i < o.mesto; i++ {
		idx := o.OProbe(key, i)
		if o.states[idx] == Occupied {
			if o.keys[idx] == key {
				o.values[idx] = value
				return false
			}
			continue
		}
		if free == -1 {
			free = idx
		}
		if o.states[idx] == Empty {
			break
		}
	}
	if o.size >= o.mesto-o.mesto/4 {
		o.OResize()
		return o.OInsert(key, value)
	}
	o.keys[free] = key
	o.values[free] = value
	o.states[free] = Occupied
	o.size++
	return true
}

func (o *OpenTable) OGet(key string) (string, error) {
	for i := 0; i < o.mesto; i++ {
		idx := o.OProbe(key, i)
		if o.states[idx] == Empty {
			break
		}
		if o.states[idx] == Occupied && o.keys[idx] == key {
			return o.values[idx], nil
		}
	}
	return "", errors.New("ключ не найден")
}

func (o *OpenTable) OContains(key string) bool {
	_, err := o.OGet(key)
	return err == nil
}

func (o *OpenTable) ODelete(key string) error {
	for i := 0; i < o.mesto; i++ {
		idx := o.OProbe(key, i)
		if o.states[idx] == Empty {
			break
		}
		if o.states[idx] == Occupied && o.keys[idx] == key {
			o.states[idx] = Deleted
			o.size--
			return nil
		}
	}
	return errors.New("ключ не найден")
}

func (o *OpenTable) OLength() int {
	return o.size
}

func (o *OpenTable) OPrint() string {
	result := ""
	for i := 0; i < o.mesto; i++ {
		if o.states[i] == Occupied {
			result += o.keys[i] + " -> " + o.values[i] + "\n"
		}
	}
	return result
}

func (o *OpenTable) OClear() {
	o.states = make([]CellState, o.mesto)
	o.keys = make([]string, o.mesto)
	o.values = make([]string, o.mesto)
	o.size = 0
}

type openState struct {
	tableState
	DoubleHash bool `json:"doubleHash"`
	Folding    bool `json:"folding"`
}

func (o *OpenTable) Save(filename string) error {
	var pairs []entry
	for i := 0; i < o.mesto; i++ {
		if o.states[i] == Occupied {
			pairs = append(pairs, entry{
				Key:   o.keys[i],
				Value: o.values[i],
			})
		}
	}

	return state.Write(filename, openState{tableState: tableState{Size: o.size, Capacity: o.mesto, Pairs: pairs}, DoubleHash: o.doubleHash, Folding: o.folding})
}

func (o *OpenTable) Load(filename string) error {
	var saved openState
	if err := state.Read(filename, &saved); err != nil {
		return err
	}
	if !saved.valid() {
		return errors.New("некорректные данные хеш-таблицы")
	}
	restored := NewOpenTable(saved.Capacity, saved.DoubleHash, saved.Folding)
	for _, p := range saved.Pairs {
		if !restored.OInsert(p.Key, p.Value) {
			return errors.New("повторяющийся ключ")
		}
	}
	*o = *restored
	return nil
}
