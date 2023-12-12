package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type monkey struct {
	op1, op2 string
	operator rune
	value    int64
	valueSet bool
}

type node struct {
	value    int64
	solved   bool
	name     string
	op1, op2 *node
	operator rune
}

func constructTree(monkeys map[string]*monkey, rootName string) *node {
	root := monkeys[rootName]
	if root.valueSet {
		return &node{
			name:   rootName,
			value:  root.value,
			solved: true,
		}
	}

	return &node{
		name:     rootName,
		op1:      constructTree(monkeys, root.op1),
		op2:      constructTree(monkeys, root.op2),
		operator: root.operator,
	}
}

func (n *node) yell() int64 {
	if n.solved {
		return n.value
	}
	op1Value := n.op1.yell()
	op2Value := n.op2.yell()
	var value int64
	switch n.operator {
	case '+':
		value = op1Value + op2Value
	case '-':
		value = op1Value - op2Value
	case '*':
		value = op1Value * op2Value
	case '/':
		value = op1Value / op2Value
	case '=':
		value = op1Value - op2Value
	}
	return value
}

func (n *node) solvePartialTree() {
	if n.name == "humn" {
		// do not solve further
		n.solved = false
		return
	}

	if n.solved {
		// nothing to do
		return
	}

	n.op1.solvePartialTree()
	n.op2.solvePartialTree()

	if n.op1.solved && n.op2.solved {
		op1Value := n.op1.value
		op2Value := n.op2.value
		switch n.operator {
		case '+':
			n.value = op1Value + op2Value
		case '-':
			n.value = op1Value - op2Value
		case '*':
			n.value = op1Value * op2Value
		case '/':
			n.value = op1Value / op2Value
		}
		n.solved = true
	}
}

func (n *node) solveDown(value int64) int64 {

	if n.name == "humn" {
		n.value = value
		n.solved = true
		return value
	}

	var nodeToSolve *node
	var nodeValue int64
	if n.op1.solved {
		nodeToSolve = n.op2
		op1Value := n.op1.value
		switch n.operator {
		case '+':
			// value = op1 + op2
			nodeValue = value - op1Value
		case '-':
			// value = op1 - op2
			nodeValue = op1Value - value
		case '*':
			// value = op1 * op2
			nodeValue = value / op1Value
		case '/':
			// value = op1Value / op2Value
			nodeValue = op1Value / value
		case '=':
			// op1Value = op2Value
			nodeValue = op1Value
		}
	} else if n.op2.solved {
		nodeToSolve = n.op1
		op2Value := n.op2.value
		switch n.operator {
		case '+':
			// value = op1 + op2
			nodeValue = value - op2Value
		case '-':
			// value = op1 - op2
			nodeValue = value + op2Value
		case '*':
			// value = op1 * op2
			nodeValue = value / op2Value
		case '/':
			// value = op1Value / op2Value
			nodeValue = value * op2Value
		case '=':
			// op1Value = op2Value
			nodeValue = op2Value
		}
	}

	return nodeToSolve.solveDown(nodeValue)
}

func (n *node) solveForHuman() {
	// solve the tree partially wherever the value is known
	n.solvePartialTree()
	fmt.Println(n.solveDown(0))
}

func main() {
	monkeys := make(map[string]*monkey)
	reader := bufio.NewReader(os.Stdin)
	for {
		input, _ := reader.ReadString('\n')
		if len(input) == 1 {
			break
		}
		input = input[:len(input)-1]

		tokens := strings.Split(input, ": ")
		name := tokens[0]

		var m1 monkey
		tokens = strings.Split(tokens[1], " ")
		if len(tokens) == 3 {
			m1.op1 = tokens[0]
			m1.op2 = tokens[2]
			m1.operator = rune(tokens[1][0])
		} else {
			m1.value, _ = strconv.ParseInt(tokens[0], 10, 64)
			m1.valueSet = true
		}

		monkeys[name] = &m1
	}

	// construct tree
	root := constructTree(monkeys, "root")
	fmt.Println(root.yell())

	root.operator = '='
	root.solveForHuman()
	fmt.Println(root.yell())
}
