package process

import (
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
	"unicode"

	"lab3_go/struct/array"
	"lab3_go/struct/hash"
	"lab3_go/struct/linkedlist"
	"lab3_go/struct/queue"
	"lab3_go/struct/singlelist"
	"lab3_go/struct/stack"
	"lab3_go/struct/tree"
)

func PrintHelp(out io.Writer) {
	fmt.Fprintln(out, "Запуск:")
	fmt.Fprintln(out, "./lab3_go --file файл --query 'КОМАНДА аргументы'")
	fmt.Fprintln(out, "Один файл хранит одну структуру. Индексы с 0.")
	fmt.Fprintln(out, "Значение с пробелами пишется в двойных кавычках: SPush \"hello world\"")
	fmt.Fprintln(out, "Односвязный список (F) и двусвязный (L), для L те же команды с буквой L:")
	fmt.Fprintln(out, "FAddFirst x | FAddEnd x | FAddAfter t x | FAddBefore t x | FAddIndex x i")
	fmt.Fprintln(out, "FDeleteFirst | FDeleteEnd | FDeleteAfter t | FDeleteBefore t")
	fmt.Fprintln(out, "FDeleteIndex i | FDeleteValue x | FGet i | FFind x")
	fmt.Fprintln(out, "FPrint | FPrintBack | FClear")
	fmt.Fprintln(out, "Стек:")
	fmt.Fprintln(out, "SPush x | SPop | STop | SIsEmptyStack | SPrint | SClear")
	fmt.Fprintln(out, "Очередь:")
	fmt.Fprintln(out, "QPush x | QPop | QFront | QBack | QPrintQueue | QClear")
	fmt.Fprintln(out, "Массив:")
	fmt.Fprintln(out, "MAddEnd x | MAddIndex x i | MGet i | MDeleteIndex i | MReplace x i")
	fmt.Fprintln(out, "MLength | MPrint | MClear")
	fmt.Fprintln(out, "АВЛ-дерево (целые числа):")
	fmt.Fprintln(out, "TAdd n | TDelete n | TFind n | THeight")
	fmt.Fprintln(out, "TPrintPryam | TPrintSim | TPrintObr | TPrintBfs | TPrintTree | TClear")
	fmt.Fprintln(out, "Хеш-таблица с цепочками:")
	fmt.Fprintln(out, "HInsert k v | HGet k | HContains k | HDelete k")
	fmt.Fprintln(out, "HLength | HPrint | HClear")
	fmt.Fprintln(out, "Открытая адресация:")
	fmt.Fprintln(out, "OInsert k v | OGet k | OContains k | ODelete k | OLength | OPrint | OClear")
}

func ProcessCommand(out io.Writer, filename, query string) error {
	parts, err := splitQuery(query)
	if err != nil {
		return err
	}
	command, args := parts[0], parts[1:]
	switch command[0] {
	case 'F':
		return runSinglList(out, filename, command, args)
	case 'L':
		return runDoubleList(out, filename, command, args)
	case 'S':
		return runStack(out, filename, command, args)
	case 'Q':
		return runQueue(out, filename, command, args)
	case 'M':
		return runArray(out, filename, command, args)
	case 'T':
		return runTree(out, filename, command, args)
	case 'H':
		return runHashTable(out, filename, command, args)
	case 'O':
		return runOpenTable(out, filename, command, args)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
}

func splitQuery(query string) ([]string, error) {
	var parts []string
	var token strings.Builder
	inQuotes, active := false, false
	for _, r := range query {
		switch {
		case r == '"':
			inQuotes = !inQuotes
			active = true
		case unicode.IsSpace(r) && !inQuotes:
			if active {
				parts = append(parts, token.String())
				token.Reset()
				active = false
			}
		default:
			token.WriteRune(r)
			active = true
		}
	}
	if inQuotes {
		return nil, errors.New("незакрытая кавычка")
	}
	if active {
		parts = append(parts, token.String())
	}
	if len(parts) == 0 || parts[0] == "" {
		return nil, errors.New("пустая команда")
	}
	return parts, nil
}

type storage interface {
	Load(string) error
	Save(string) error
}

func load(value storage, filename string) error {
	if err := value.Load(filename); err != nil && !errors.Is(err, os.ErrNotExist) {
		return fmt.Errorf("загрузка состояния: %w", err)
	}
	return nil
}

func save(out io.Writer, value storage, filename string, result any) error {
	if err := value.Save(filename); err != nil {
		return fmt.Errorf("запись состояния: %w", err)
	}
	if result != nil {
		fmt.Fprintln(out, "->", result)
	}
	return nil
}

func printBool(out io.Writer, value bool) {
	fmt.Fprintln(out, "->", strings.ToUpper(strconv.FormatBool(value)))
}

func runArray(out io.Writer, filename, command string, args []string) error {
	arr := array.NewArray(1)
	if err := load(arr, filename); err != nil {
		return err
	}

	switch command {
	case "MAddEnd":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		arr.MAddEnd(args[0])
		return save(out, arr, filename, args[0])
	case "MAddIndex":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		index, err := strconv.Atoi(args[1])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[1])
		}
		if err := arr.MAddIndex(args[0], index); err != nil {
			return err
		}
		return save(out, arr, filename, args[0])
	case "MGet":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		index, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		val, err := arr.MGet(index)
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "MDeleteIndex":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		index, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		val, err := arr.MDeleteIndex(index)
		if err != nil {
			return err
		}
		return save(out, arr, filename, val)
	case "MReplace":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		index, err := strconv.Atoi(args[1])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[1])
		}
		err = arr.MReplace(args[0], index)
		if err != nil {
			return err
		}
		return save(out, arr, filename, args[0])
	case "MLength":
		fmt.Fprintln(out, "->", arr.MLength())
	case "MPrint":
		fmt.Fprint(out, arr.MPrint())
	case "MClear":
		arr.MClear()
		return save(out, arr, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runSinglList(out io.Writer, filename, command string, args []string) error {
	list := singlelist.NewSinglList()
	if err := load(list, filename); err != nil {
		return err
	}

	switch command {
	case "FAddFirst":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		list.FAddFirst(args[0])
		return save(out, list, filename, args[0])
	case "FAddEnd":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		list.FAddEnd(args[0])
		return save(out, list, filename, args[0])
	case "FAddAfter":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		err := list.FAddAfter(args[0], args[1])
		if err != nil {
			return err
		}
		return save(out, list, filename, args[1])
	case "FAddBefore":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		err := list.FAddBefore(args[0], args[1])
		if err != nil {
			return err
		}
		return save(out, list, filename, args[1])
	case "FAddIndex":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		index, err := strconv.Atoi(args[1])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[1])
		}
		err = list.FAddIndex(args[0], index)
		if err != nil {
			return err
		}
		return save(out, list, filename, args[0])
	case "FDeleteFirst":
		val, err := list.FDeleteFirst()
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "FDeleteEnd":
		val, err := list.FDeleteEnd()
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "FDeleteAfter":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := list.FDeleteAfter(args[0])
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "FDeleteBefore":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := list.FDeleteBefore(args[0])
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "FDeleteIndex":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		index, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		val, err := list.FDeleteIndex(index)
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "FDeleteValue":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := list.FDeleteValue(args[0])
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "FGet":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		index, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		val, err := list.FGet(index)
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "FFind":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		found := list.FFind(args[0])
		printBool(out, found)
	case "FPrint":
		fmt.Fprint(out, list.FPrint())
	case "FPrintBack":
		fmt.Fprint(out, list.FPrintBack())
	case "FClear":
		list.FClear()
		return save(out, list, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runDoubleList(out io.Writer, filename, command string, args []string) error {
	list := linkedlist.NewDoubleList()
	if err := load(list, filename); err != nil {
		return err
	}

	switch command {
	case "LAddFirst":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		list.LAddFirst(args[0])
		return save(out, list, filename, args[0])
	case "LAddEnd":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		list.LAddEnd(args[0])
		return save(out, list, filename, args[0])
	case "LAddAfter":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		err := list.LAddAfter(args[0], args[1])
		if err != nil {
			return err
		}
		return save(out, list, filename, args[1])
	case "LAddBefore":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		err := list.LAddBefore(args[0], args[1])
		if err != nil {
			return err
		}
		return save(out, list, filename, args[1])
	case "LAddIndex":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		index, err := strconv.Atoi(args[1])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[1])
		}
		err = list.LAddIndex(args[0], index)
		if err != nil {
			return err
		}
		return save(out, list, filename, args[0])
	case "LDeleteFirst":
		val, err := list.LDeleteFirst()
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "LDeleteEnd":
		val, err := list.LDeleteEnd()
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "LDeleteAfter":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := list.LDeleteAfter(args[0])
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "LDeleteBefore":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := list.LDeleteBefore(args[0])
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "LDeleteIndex":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		index, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		val, err := list.LDeleteIndex(index)
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "LDeleteValue":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := list.LDeleteValue(args[0])
		if err != nil {
			return err
		}
		return save(out, list, filename, val)
	case "LGet":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		index, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		val, err := list.LGet(index)
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "LFind":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		found := list.LFind(args[0])
		printBool(out, found)
	case "LPrint":
		fmt.Fprint(out, list.LPrint())
	case "LPrintBack":
		fmt.Fprint(out, list.LPrintBack())
	case "LClear":
		list.LClear()
		return save(out, list, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runStack(out io.Writer, filename, command string, args []string) error {
	stack := stack.NewStack()
	if err := load(stack, filename); err != nil {
		return err
	}

	switch command {
	case "SPush":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		stack.SPush(args[0])
		return save(out, stack, filename, args[0])
	case "SPop":
		val, err := stack.SPop()
		if err != nil {
			return err
		}
		return save(out, stack, filename, val)
	case "STop":
		val, err := stack.STop()
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "SIsEmptyStack":
		printBool(out, stack.SIsEmptyStack())
	case "SPrint":
		fmt.Fprint(out, stack.SPrint())
	case "SClear":
		stack.SClear()
		return save(out, stack, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runQueue(out io.Writer, filename, command string, args []string) error {
	queue := queue.NewQueue()
	if err := load(queue, filename); err != nil {
		return err
	}

	switch command {
	case "QPush":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		queue.QPush(args[0])
		return save(out, queue, filename, args[0])
	case "QPop":
		val, err := queue.QPop()
		if err != nil {
			return err
		}
		return save(out, queue, filename, val)
	case "QFront":
		val, err := queue.QFront()
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "QBack":
		val, err := queue.QBack()
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "QPrintQueue":
		fmt.Fprint(out, queue.QPrintQueue())
	case "QClear":
		queue.QClear()
		return save(out, queue, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runTree(out io.Writer, filename, command string, args []string) error {
	tree := tree.NewTree()
	if err := load(tree, filename); err != nil {
		return err
	}

	switch command {
	case "TAdd":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		value, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		err = tree.TAddRoot(value)
		if err != nil {
			return err
		}
		return save(out, tree, filename, value)
	case "TDelete":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		value, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		err = tree.TDeleteRoot(value)
		if err != nil {
			return err
		}
		return save(out, tree, filename, value)
	case "TFind":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		value, err := strconv.Atoi(args[0])
		if err != nil {
			return fmt.Errorf("ожидалось целое число: %q", args[0])
		}
		found := tree.TFind(value)
		printBool(out, found)
	case "THeight":
		fmt.Fprintln(out, "->", tree.THeightRoot())
	case "TPrintPryam":
		fmt.Fprintln(out, strings.TrimSpace(tree.TPrintPryam()))
	case "TPrintSim":
		fmt.Fprintln(out, strings.TrimSpace(tree.TPrintSimRoot()))
	case "TPrintObr":
		fmt.Fprintln(out, strings.TrimSpace(tree.TPrintObr()))
	case "TPrintBfs":
		fmt.Fprintln(out, strings.TrimSpace(tree.TPrintBfs()))
	case "TPrintTree":
		fmt.Fprint(out, tree.TPrintTree())
	case "TClear":
		tree.TClearRoot()
		return save(out, tree, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runHashTable(out io.Writer, filename, command string, args []string) error {
	table := hash.NewHashTable(8)
	if err := load(table, filename); err != nil {
		return err
	}

	switch command {
	case "HInsert":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		table.HInsert(args[0], args[1])
		return save(out, table, filename, args[0]+": "+args[1])
	case "HGet":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := table.HGet(args[0])
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "HContains":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		found := table.HContains(args[0])
		printBool(out, found)
	case "HDelete":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		err := table.HDelete(args[0])
		if err != nil {
			return err
		}
		return save(out, table, filename, args[0])
	case "HLength":
		fmt.Fprintln(out, "->", table.HLength())
	case "HPrint":
		fmt.Fprint(out, table.HPrint())
	case "HClear":
		table.HClear()
		return save(out, table, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}

func runOpenTable(out io.Writer, filename, command string, args []string) error {
	table := hash.NewOpenTable(11, false, false)
	if err := load(table, filename); err != nil {
		return err
	}

	switch command {
	case "OInsert":
		if len(args) != 2 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 2)
		}
		table.OInsert(args[0], args[1])
		return save(out, table, filename, args[0]+": "+args[1])
	case "OGet":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		val, err := table.OGet(args[0])
		if err != nil {
			return err
		}
		fmt.Fprintln(out, "->", val)
	case "OContains":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		found := table.OContains(args[0])
		printBool(out, found)
	case "ODelete":
		if len(args) != 1 {
			return fmt.Errorf("%s: ожидается аргументов: %d", command, 1)
		}
		err := table.ODelete(args[0])
		if err != nil {
			return err
		}
		return save(out, table, filename, args[0])
	case "OLength":
		fmt.Fprintln(out, "->", table.OLength())
	case "OPrint":
		fmt.Fprint(out, table.OPrint())
	case "OClear":
		table.OClear()
		return save(out, table, filename, nil)
	default:
		return fmt.Errorf("неизвестная команда %q", command)
	}
	return nil
}
