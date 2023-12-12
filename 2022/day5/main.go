package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func problem_1() {

	var stacks [][]rune

	reader := bufio.NewReader(os.Stdin)

	for {
		input, _ := reader.ReadString('\n')
		if strings.HasPrefix(input, " 1") {
			break
		}

		// stack id
		var sid int

		for i := 0; i < len(input); i = i + 4 {
			// there is a stack
			if len(stacks) <= sid {
				stacks = append(stacks, []rune{})
			}

			if input[i] == '[' {
				// there is a box
				stacks[sid] = append([]rune{rune(input[i+1])}, stacks[sid]...)
			}

			sid++
		}
	}

	// stacks are ready. top is at the stack[n-1]

	for i, s := range stacks {
		fmt.Printf("%d : ", i)
		for _, item := range s {
			fmt.Printf("%c, ", item)
		}
		fmt.Println()
	}

	// skip the empty line inbetween inputs
	reader.ReadString('\n')

	// Start processing moves
	var boxes, source, dest int
	for {
		if n, _ := fmt.Scanf("move %d from %d to %d\n", &boxes, &source, &dest); n == 0 {
			break
		}

		// adjust for index starting from 0
		source--
		dest--

		for boxes != 0 {
			boxes--
			//pop from source
			stack := stacks[source]
			box := stack[len(stack)-1]
			stacks[source] = stack[:len(stack)-1]
			// push into dest
			stacks[dest] = append(stacks[dest], box)
		}
	}

	for _, stack := range stacks {
		fmt.Printf("%c", stack[len(stack)-1])
	}

}

// problem2
func main() {

	var stacks [][]rune

	reader := bufio.NewReader(os.Stdin)

	for {
		input, _ := reader.ReadString('\n')
		if strings.HasPrefix(input, " 1") {
			break
		}

		// stack id
		var sid int

		for i := 0; i < len(input); i = i + 4 {
			// there is a stack
			if len(stacks) <= sid {
				stacks = append(stacks, []rune{})
			}

			if input[i] == '[' {
				// there is a box
				stacks[sid] = append([]rune{rune(input[i+1])}, stacks[sid]...)
			}

			sid++
		}
	}

	// stacks are ready. top is at the stack[n-1]

	for i, s := range stacks {
		fmt.Printf("%d : ", i)
		for _, item := range s {
			fmt.Printf("%c, ", item)
		}
		fmt.Println()
	}

	// skip the empty line inbetween inputs
	reader.ReadString('\n')

	// Start processing moves
	var boxes, source, dest int
	for {
		if n, _ := fmt.Scanf("move %d from %d to %d\n", &boxes, &source, &dest); n == 0 {
			break
		}

		// adjust for index starting from 0
		source--
		dest--

		// Append the boxes being moved into dest
		sourceStack := stacks[source]
		stacks[dest] = append(stacks[dest], sourceStack[len(sourceStack)-boxes:]...)
		stacks[source] = sourceStack[:len(sourceStack)-boxes]
	}

	for _, stack := range stacks {
		fmt.Printf("%c", stack[len(stack)-1])
	}

	fmt.Println()

}
