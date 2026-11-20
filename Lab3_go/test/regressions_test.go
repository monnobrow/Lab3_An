package test

import (
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/require"
	"lab3_go/struct/hash"
)

func TestOpenTableTombstoneAndLongKeys(t *testing.T) {
	table := hash.NewOpenTable(11, false, false)
	require.True(t, table.OInsert("a", "first"))
	require.True(t, table.OInsert("l", "old"))
	require.NoError(t, table.ODelete("a"))
	require.False(t, table.OInsert("l", "new"))
	require.Equal(t, 1, table.OLength())
	got, err := table.OGet("l")
	require.NoError(t, err)
	require.Equal(t, "new", got)

	double := hash.NewOpenTable(1, true, false)
	for i := 0; i < 80; i++ {
		key := strings.Repeat("длинный ключ ", i+1)
		require.True(t, double.OInsert(key, "v"))
		require.True(t, double.OContains(key))
	}
	require.Equal(t, 80, double.OLength())

	file := filepath.Join(t.TempDir(), "empty.json")
	double.OClear()
	require.NoError(t, double.Save(file))
	require.NoError(t, table.Load(file))
	require.Equal(t, 0, table.OLength())
}
