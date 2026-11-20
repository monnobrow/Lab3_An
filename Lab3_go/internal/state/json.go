package state

import (
	"encoding/json"
	"os"
	"path/filepath"
)

func Read(filename string, target any) error {
	data, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, target)
}

func Write(filename string, value any) error {
	temp, err := os.CreateTemp(filepath.Dir(filename), ".lab3-*.json")
	if err != nil {
		return err
	}
	defer os.Remove(temp.Name())
	writeErr := json.NewEncoder(temp).Encode(value)
	closeErr := temp.Close()
	if writeErr != nil {
		return writeErr
	}
	if closeErr != nil {
		return closeErr
	}
	return os.Rename(temp.Name(), filename)
}
