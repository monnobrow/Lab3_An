package main

import (
	"bytes"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/require"
)

type step struct {
	query  string
	result string
}

func TestCommandRoundTrips(t *testing.T) {
	cases := map[string][]step{
		"array": {
			{`MAddEnd "два слова"`, "-> два слова"}, {"MAddIndex a 0", "-> a"},
			{"MReplace b 0", "-> b"}, {"MGet 1", "-> два слова"}, {"MLength", "-> 2"},
			{"MPrint", "b два слова"}, {"MDeleteIndex 0", "-> b"}, {"MClear", ""}, {"MLength", "-> 0"},
		},
		"stack": {
			{"SIsEmptyStack", "-> TRUE"}, {"SPush first", "-> first"}, {`SPush ""`, "->"},
			{"STop", "->"}, {"SPop", "->"}, {"SIsEmptyStack", "-> FALSE"},
			{"SPrint", "first"}, {"SClear", ""}, {"SIsEmptyStack", "-> TRUE"},
		},
		"queue": {
			{"QPush first", "-> first"}, {"QPush last", "-> last"}, {"QFront", "-> first"},
			{"QBack", "-> last"}, {"QPrintQueue", "first\nlast"}, {"QPop", "-> first"}, {"QClear", ""},
		},
		"tree": {
			{"TAdd 30", "-> 30"}, {"TAdd 20", "-> 20"}, {"TAdd 10", "-> 10"},
			{"TPrintPryam", "20 10 30"}, {"TPrintSim", "10 20 30"}, {"TPrintObr", "10 30 20"},
			{"TPrintBfs", "20 10 30"}, {"TPrintTree", "30\n20\n    10"}, {"THeight", "-> 2"},
			{"TFind 25", "-> FALSE"}, {"TDelete 20", "-> 20"}, {"TFind 10", "-> TRUE"},
			{"TClear", ""}, {"THeight", "-> 0"},
		},
	}
	for _, prefix := range []string{"F", "L"} {
		for _, s := range []step{
			{"AddFirst b", "-> b"}, {"AddEnd d", "-> d"}, {"AddAfter b c", "-> c"},
			{"AddBefore b a", "-> a"}, {"AddIndex e 4", "-> e"}, {"Get 2", "-> c"},
			{"Find d", "-> TRUE"}, {"Print", "a\nb\nc\nd\ne"},
			{"PrintBack", "e\nd\nc\nb\na"}, {"DeleteAfter c", "-> d"},
			{"DeleteBefore c", "-> b"}, {"DeleteIndex 1", "-> c"}, {"DeleteValue e", "-> e"},
			{"DeleteFirst", "-> a"}, {"AddFirst a", "-> a"}, {"DeleteEnd", "-> a"}, {"Clear", ""},
		} {
			cases[prefix] = append(cases[prefix], step{prefix + s.query, s.result})
		}
	}
	for _, prefix := range []string{"H", "O"} {
		cases[prefix] = []step{
			{prefix + "Insert key value", "-> key: value"}, {prefix + "Get key", "-> value"},
			{prefix + "Contains other", "-> FALSE"}, {prefix + "Length", "-> 1"},
			{prefix + "Delete key", "-> key"}, {prefix + "Clear", ""}, {prefix + "Length", "-> 0"},
		}
	}
	for name, scenario := range cases {
		t.Run(name, func(t *testing.T) {
			filename := filepath.Join(t.TempDir(), "state.json")
			for _, s := range scenario {
				var out bytes.Buffer
				require.NoError(t, run([]string{"--file", filename, "--query", s.query}, &out), s.query)
				require.Equal(t, s.result, strings.TrimSpace(out.String()), s.query)
			}
		})
	}
}

func TestCommandErrors(t *testing.T) {
	var out bytes.Buffer
	require.NoError(t, run(nil, &out))
	require.Contains(t, out.String(), "--file")
	require.Error(t, run([]string{"--wrong"}, io.Discard))
	require.Error(t, run([]string{"--file", "state.json"}, io.Discard))
	filename := filepath.Join(t.TempDir(), "state.json")
	for _, query := range []string{" ", `""`, `SPush "незакрыто`, "XUnknown",
		"MAddEnd", "MAddIndex a", "MAddIndex a n", "MAddIndex a 1", "MGet x", "MGet 0",
		"MDeleteIndex 0", "MDeleteIndex x", "MReplace a x", "MReplace a 0", "MUnknown",
		"SPush", "SPop", "STop", "SUnknown", "QPush", "QPop", "QFront", "QBack", "QUnknown",
		"TAdd", "TAdd a", "TDelete a", "TFind a", "TDelete 1", "TUnknown",
		"HInsert", "HGet a", "HDelete a", "HUnknown", "OInsert", "OGet a", "ODelete a", "OUnknown",
	} {
		require.Error(t, run([]string{"--file", filename, "--query", query}, io.Discard), query)
	}
	for _, prefix := range []string{"F", "L"} {
		for _, query := range []string{"AddFirst", "AddEnd", "AddAfter a b", "AddBefore a b",
			"AddIndex a x", "AddIndex a 3", "DeleteFirst", "DeleteEnd", "DeleteAfter a",
			"DeleteBefore a", "DeleteIndex 0", "DeleteIndex x", "DeleteValue a", "Get 0", "Get x", "Unknown",
		} {
			require.Error(t, run([]string{"--file", filename, "--query", prefix + query}, io.Discard), prefix+query)
		}
	}
	require.ErrorContains(t, run([]string{"--file", filepath.Join(filename, "x"), "--query", "SPush a"}, io.Discard), "запись")
	invalid := []byte("{broken")
	require.NoError(t, os.WriteFile(filename, invalid, 0600))
	for _, query := range []string{"MAddEnd a", "FAddEnd a", "LAddEnd a", "SPush a", "QPush a", "TAdd 2", "HInsert a b", "OInsert a b"} {
		require.ErrorContains(t, run([]string{"--file", filename, "--query", query}, io.Discard), "загрузка")
	}
	preserved, err := os.ReadFile(filename)
	require.NoError(t, err)
	require.Equal(t, invalid, preserved)
}
