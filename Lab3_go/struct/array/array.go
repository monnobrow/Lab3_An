package array

import (
	"errors"

	"lab3_go/internal/state"
)

type Array struct {
	data  []string
	size  int
	mesto int
}

func NewArray(mesto int) *Array {
	if mesto < 1 {
		mesto = 1
	}
	return &Array{
		data:  make([]string, mesto),
		size:  0,
		mesto: mesto,
	}
}

func (a *Array) MResize() {
	newData := make([]string, a.mesto*2)
	copy(newData, a.data)
	a.data = newData
	a.mesto *= 2
}

func (a *Array) MCheckIndex(index int) error {
	if index < 0 || index >= a.size {
		return errors.New("нет такого индекса")
	}
	return nil
}

func (a *Array) MAddEnd(chislo string) {
	if a.size == a.mesto {
		a.MResize()
	}
	a.data[a.size] = chislo
	a.size++
}

func (a *Array) MAddIndex(chislo string, index int) error {
	if index < 0 || index > a.size {
		return errors.New("нет такого индекса")
	}
	if a.size == a.mesto {
		a.MResize()
	}
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = chislo
	a.size++
	return nil
}

func (a *Array) MGet(index int) (string, error) {
	if err := a.MCheckIndex(index); err != nil {
		return "", err
	}
	return a.data[index], nil
}

func (a *Array) MDeleteIndex(index int) (string, error) {
	if err := a.MCheckIndex(index); err != nil {
		return "", err
	}
	chislo := a.data[index]
	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
	return chislo, nil
}

func (a *Array) MReplace(chislo string, index int) error {
	if err := a.MCheckIndex(index); err != nil {
		return err
	}
	a.data[index] = chislo
	return nil
}

func (a *Array) MLength() int {
	return a.size
}

func (a *Array) MPrint() string {
	result := ""
	for i := 0; i < a.size; i++ {
		result += a.data[i] + " "
	}
	return result + "\n"
}

func (a *Array) MClear() {
	a.size = 0
}

type snapshot struct {
	Size     int      `json:"size"`
	Capacity int      `json:"mesto"`
	Data     []string `json:"data"`
}

func (a *Array) Save(filename string) error {
	return state.Write(filename, snapshot{Size: a.size, Capacity: a.mesto, Data: a.data[:a.size]})
}

func (a *Array) Load(filename string) error {
	var saved snapshot
	if err := state.Read(filename, &saved); err != nil {
		return err
	}
	if saved.Capacity < 1 || saved.Size != len(saved.Data) || saved.Size > saved.Capacity {
		return errors.New("некорректные размер или ёмкость массива")
	}
	restored := NewArray(saved.Capacity)
	copy(restored.data, saved.Data)
	restored.size = saved.Size
	*a = *restored
	return nil
}
