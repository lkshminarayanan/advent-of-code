package aocutils

import "testing"

func Test_readInput_URL(t *testing.T) {
	s := ReadInput("https://adventofcode.com/2022/day/1/input")

	t.Log(s)
}

func Test_readInput_file(t *testing.T) {
	s := ReadInput("input_reader.go")

	t.Log(s)
}
