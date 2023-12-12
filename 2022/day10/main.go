package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type cycle struct {
	id          int
	register    int
	interesting int
	answer      int
}

func (c *cycle) run(op ...int) {
	// running cycle
	c.id++

	// during cycle
	if c.id == c.interesting {
		c.answer += c.id * c.register
		c.interesting += 40
	}

	// after cycle
	if len(op) == 1 {
		c.register += op[0]
	}
}

type drawCycle struct {
	id       int
	register int
	answer   string
}

func (c *drawCycle) run(op ...int) {
	// running cycle
	c.id++
	if c.id == 40 {
		c.id = 0
	}

	// during cycle
	if c.id == 0 {
		c.answer += fmt.Sprintln()
	}
	if c.register-1 == c.id || c.register == c.id || c.register+1 == c.id {
		c.answer += fmt.Sprint("#")
	} else {
		c.answer += fmt.Sprint(".")
	}

	// after cycle
	if len(op) == 1 {
		c.register += op[0]
	}
}

func main() {
	reader := bufio.NewReader(os.Stdin)
	/*c := &cycle{
		register:    1,
		interesting: 20,
	}*/
	c := &drawCycle{
		id:       -1,
		register: 1,
	}
	for {
		//parse input
		input, _ := reader.ReadString('\n')
		input = input[:len(input)-1]
		if input == "EOF" {
			break
		}

		switch input[:4] {
		case "addx":
			op, _ := strconv.Atoi(input[5:])
			c.run()
			c.run(op)

		case "noop":
			c.run()
		}
	}

	fmt.Println(c.answer)
}
