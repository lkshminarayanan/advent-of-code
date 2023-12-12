package main

import (
	"bufio"
	"fmt"
	"math/big"
	"os"
	"sort"
	"strings"
)

type Monkey struct {
	items []*big.Int
	// op is the operation and opValue is the value
	op      rune
	opValue *big.Int
	// test value to be divided with and the resulting monkey
	testValue           *big.Int
	testTrue, testFalse int
	// inspect count
	inspected int
}

func (m *Monkey) print() {
	fmt.Print("[")
	for _, it := range m.items {
		fmt.Print(it.String(), ", ")
	}
	fmt.Println("]")
}

var zero = big.NewInt(0)

func processItems(monkeys []Monkey, id int) {
	for len(monkeys[id].items) > 0 {
		// inspect item
		monkeys[id].inspected++
		item := monkeys[id].items[0]
		if len(monkeys[id].items) > 0 {
			monkeys[id].items = monkeys[id].items[1:]
		} else {
			monkeys[id].items = nil
		}
		updatedItem := new(big.Int)
		switch monkeys[id].op {
		case '+':
			updatedItem.Add(item, monkeys[id].opValue)
		case '*':
			updatedItem.Mul(item, monkeys[id].opValue)
		case '^':
			updatedItem.Mul(item, item)
		}
		item = nil

		// get bored
		// updatedItem.Div(updatedItem, big.NewInt(3))

		// throw it
		remiander := new(big.Int).Mod(updatedItem, monkeys[id].testValue)
		if remiander.Cmp(zero) == 0 {
			monkeys[monkeys[id].testTrue].items = append(monkeys[monkeys[id].testTrue].items, updatedItem)
		} else {
			monkeys[monkeys[id].testFalse].items = append(monkeys[monkeys[id].testFalse].items, updatedItem)
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
			val, _ := new(big.Int).SetString(token, 0)
			m.items = append(m.items, val)
		}

		// Operation: new = old * 5
		input, _ = reader.ReadString('\n')
		var opValStr string
		fmt.Sscanf(input, "  Operation: new = old %c %s\n", &m.op, &opValStr)
		if m.op == '*' && opValStr == "old" {
			m.op = '^'
		} else {
			m.opValue, _ = new(big.Int).SetString(opValStr, 0)
		}

		// Test: divisible by 11
		input, _ = reader.ReadString('\n')
		var val int64
		fmt.Sscanf(input, "  Test: divisible by %d\n", &val)
		m.testValue = big.NewInt(val)

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

	for i := 0; i < 10000; i++ {
		fmt.Println(i)
		for id := range monkeys {
			processItems(monkeys, id)
		}
	}

	fmt.Println("-----")
	for _, m := range monkeys {
		fmt.Println(m.inspected)
	}
	fmt.Println("-----")

	var inspectCount []int
	for _, m := range monkeys {
		inspectCount = append(inspectCount, m.inspected)
	}

	sort.Ints(inspectCount)
	fmt.Println(inspectCount[len(inspectCount)-1] * inspectCount[len(inspectCount)-2])
}
