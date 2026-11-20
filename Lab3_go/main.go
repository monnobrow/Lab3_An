package main

import (
	"errors"
	"flag"
	"fmt"
	"io"
	"os"

	"lab3_go/process"
)

func run(args []string, out io.Writer) error {
	if len(args) == 0 || (len(args) == 1 && args[0] == "--help") {
		process.PrintHelp(out)
		return nil
	}
	flags := flag.NewFlagSet("lab3_go", flag.ContinueOnError)
	flags.SetOutput(io.Discard)
	filename := flags.String("file", "", "файл состояния")
	query := flags.String("query", "", "команда")
	if err := flags.Parse(args); err != nil {
		return err
	}
	if *filename == "" || *query == "" || flags.NArg() != 0 {
		return errors.New("укажите --file и --query")
	}
	return process.ProcessCommand(out, *filename, *query)
}

func main() {
	if err := run(os.Args[1:], os.Stdout); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка:", err)
		os.Exit(1)
	}
}
