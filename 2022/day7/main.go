package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type node interface {
	size(reqSize int, minSize *int) int
}

type file struct {
	fileSize int
}

func (f *file) size(reqSize int, minSize *int) int {
	return f.fileSize
}

type dir struct {
	dirStruct map[string]node
}

func newDir(parent *dir) *dir {
	ds := make(map[string]node)
	ds[".."] = parent
	return &dir{
		dirStruct: ds,
	}
}

func (d *dir) size(reqSize int, minSize *int) int {
	var total int
	for path, n := range d.dirStruct {
		if path != ".." {
			total += n.size(reqSize, minSize)
		}
	}

	if total >= reqSize && (*minSize) > total {
		(*minSize) = total
	}

	return total
}

func parseOutput() *dir {
	root := newDir(nil)
	var pwd *dir
	var isls bool
	for {
		//parse input
		reader := bufio.NewReader(os.Stdin)
		input, _ := reader.ReadString('\n')
		input = input[:len(input)-1]
		if input == "EOF" {
			break
		}

		if strings.HasPrefix(input, "$") {
			switch input[2:4] {
			case "cd":
				if path := input[5:]; path == "/" {
					pwd = root
				} else {
					pwd = pwd.dirStruct[path].(*dir)
				}
				isls = false
			case "ls":
				isls = true
			}
			continue
		}

		if isls {
			tokens := strings.Split(input, " ")
			if tokens[0] == "dir" {
				// directory in ls
				pwd.dirStruct[tokens[1]] = newDir(pwd)
			} else {
				// file in ls
				fileSize, _ := strconv.Atoi(tokens[0])
				pwd.dirStruct[tokens[1]] = &file{
					fileSize: fileSize,
				}
			}
		}
	}

	return root
}

func main() {
	root := parseOutput()
	var minsize int
	totalSize := root.size(minsize, &minsize)
	requiredSize := 30000000 - (70000000 - totalSize)

	minsize = totalSize
	root.size(requiredSize, &minsize)
	fmt.Println(minsize)
}
