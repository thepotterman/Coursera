package main

import (
	"fmt"
	"sort"
	"io"
	"os"
    "strconv"
	//"path/filepath"
	//"strings"
)

func formatSize(size_raw int64) string {
    if size_raw == 0 {
        return "(empty)"
    } else {
        return "(" + strconv.FormatInt(size_raw, 10) + "b)"
    }
}

func dirTree(out io.Writer, path string, printFiles bool) error {
    err := dirTreeReal(out, path, printFiles, "")
    return err
}

func dirTreeReal(out io.Writer, path string, printFiles bool, prefix string) error {
    currentPrefix := prefix + "├───"
    file, err := os.Open(path)
    if err != nil {
        return err
    }
    dirEntry, err := file.ReadDir(0)
    if err != nil {
        return err
    }
    sort.Slice(dirEntry, func(lhs, rhs int) bool {
        return dirEntry[lhs].Name() < dirEntry[rhs].Name()
    })
    if(!printFiles) {
        var dirEntryNew []os.DirEntry
        for _, file := range dirEntry {
            if file.IsDir() {
                dirEntryNew = append(dirEntryNew, file)
            }
        }
        dirEntry = dirEntryNew
    }
    for idx, file := range dirEntry {
        if idx == len(dirEntry) - 1 {
            currentPrefix = prefix + "└───"
        }
        if printFiles && !file.IsDir() {
            info, _ := file.Info()
            fmt.Fprintln(out, currentPrefix + file.Name(), formatSize(info.Size()))
        } else if file.IsDir() {
            fmt.Fprintln(out, currentPrefix + file.Name())
            if idx == len(dirEntry) - 1 {
                dirTreeReal(out, path + "/" + file.Name(), printFiles, prefix + "\t")
            } else {
                dirTreeReal(out, path + "/" + file.Name(), printFiles, prefix + "│" + "\t")
            }
        }
    }
    return nil
}

func main() {
	out := os.Stdout
	if !(len(os.Args) == 2 || len(os.Args) == 3) {
		panic("usage go run main.go . [-f]")
	}
	path := os.Args[1]
	printFiles := len(os.Args) == 3 && os.Args[2] == "-f"
	err := dirTree(out, path, printFiles)
	if err != nil {
		panic(err.Error())
	}
}
