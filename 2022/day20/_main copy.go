package main

import "fmt"

type number struct {
	value int64
	index int64
}

const (
	// KEY = 811589153
	KEY   = 1
	LOOPS = 1
)

func (n *number) move(output []*number) {

	newIndex := n.index + n.value
	for newIndex < 0 {
		newIndex += int64(len(output))
	}

	if newIndex > int64(len(output)) {
		newIndex %= int64(len(output))
	}

	if n.index == newIndex {
		return
	} else if n.index < newIndex {
		for i := n.index; i < newIndex; i++ {
			output[i] = output[i+1]
			output[i].index = i
		}
		output[newIndex] = n
		n.index = newIndex
	} else {
		for i := n.index; i > newIndex; i-- {
			output[i] = output[i-1]
			output[i].index = i
		}
		output[newIndex] = n
		n.index = newIndex
	}
}

func main() {
	var input []*number
	var output []*number
	for {
		var n int
		if n, _ := fmt.Scanf("%d\n", &n); n == 0 {
			break
		}

		num := &number{
			value: int64(n) * KEY,
			index: int64(len(output)),
		}

		input = append(input, num)
		output = append(output, num)
	}

	for i := 0; i < LOOPS; i++ {
		for _, n := range input {
			n.move(output)
		}

		for _, op := range output {
			fmt.Print(op.index, ":", op.value, ", ")
		}
		fmt.Println()
	}

	var id int64
	for _, op := range output {
		if op.value == 0 {
			id = op.index
		}
	}

	oplen := int64(len(output))

	x1 := (id + 1000) % oplen
	x2 := (id + 2000) % oplen
	x3 := (id + 3000) % oplen

	fmt.Println(output[x1].value + output[x2].value + output[x3].value)
	fmt.Println(output[x1].value, output[x2].value, output[x3].value)
}
