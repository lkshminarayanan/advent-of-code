package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type Monkey struct {
	items []int
	// op is the operation and opValue is the value
	op      rune
	opValue int
	// test value to be divided with and the resulting monkey
	testValue           int
	testTrue, testFalse int
	// inspect count
	inspect int
}

func processItems(monkeys []Monkey, id, supermodulo int) {
	for len(monkeys[id].items) > 0 {
		// inspect item
		monkeys[id].inspect++
		item := monkeys[id].items[0]
		if len(monkeys[id].items) > 1 {
			monkeys[id].items = monkeys[id].items[1:]
		} else {
			monkeys[id].items = nil
		}

		// increase worry
		switch monkeys[id].op {
		case '+':
			item += monkeys[id].opValue
		case '*':
			item *= monkeys[id].opValue
		case '^':
			item *= item
		}

		// get bored
		// item = item / 3
		item %= supermodulo

		// throws it
		if item%monkeys[id].testValue == 0 {
			monkeys[monkeys[id].testTrue].items = append(monkeys[monkeys[id].testTrue].items, item)
		} else {
			monkeys[monkeys[id].testFalse].items = append(monkeys[monkeys[id].testFalse].items, item)
		}
	}
}

func main() {
	var monkeys []Monkey
	reader := bufio.NewReader(os.Stdin)
	for {
		// Monkey 0: (ignored)
		reader.ReadString('\n')

		var m Monkey
		// Starting items: 83, 88, 96, 79, 86, 88, 70
		input, _ := reader.ReadString('\n')
		tokens := strings.Split(input[len("  Starting items: "):len(input)-1], ", ")
		for _, token := range tokens {
			val, _ := strconv.Atoi(token)
			m.items = append(m.items, val)
		}

		// Operation: new = old * 5
		input, _ = reader.ReadString('\n')
		var opValStr string
		fmt.Sscanf(input, "  Operation: new = old %c %s\n", &m.op, &opValStr)
		if m.op == '*' && opValStr == "old" {
			m.op = '^'
		} else {
			m.opValue, _ = strconv.Atoi(opValStr)
		}

		// Test: divisible by 11
		input, _ = reader.ReadString('\n')
		fmt.Sscanf(input, "  Test: divisible by %d\n", &m.testValue)

		//     If true: throw to monkey 2
		input, _ = reader.ReadString('\n')
		fmt.Sscanf(input, "     If true: throw to monkey %d\n", &m.testTrue)

		//     If true: throw to monkey 2
		input, _ = reader.ReadString('\n')
		fmt.Sscanf(input, "     If false: throw to monkey %d\n", &m.testFalse)

		monkeys = append(monkeys, m)

		// Read empty line
		input, _ = reader.ReadString('\n')
		if strings.HasPrefix(input, "EOF") {
			break
		}
	}

	supermodulo := 1
	for _, monkey := range monkeys {
		supermodulo *= monkey.testValue
	}

	for i := 0; i < 10000; i++ {
		for id := range monkeys {
			processItems(monkeys, id, supermodulo)
		}
	}

	fmt.Println("-----")
	for _, m := range monkeys {
		fmt.Println(m.inspect)
	}
	fmt.Println("-----")

	var inspectCount []int
	for _, m := range monkeys {
		inspectCount = append(inspectCount, m.inspect)
	}

	sort.Ints(inspectCount)
	fmt.Println(inspectCount[len(inspectCount)-1] * inspectCount[len(inspectCount)-2])
}
