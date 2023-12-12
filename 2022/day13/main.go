package main

import (
	"fmt"
	"sort"
)

type item interface {
	// compare(right item) int
}

type list struct {
	val []item
}

func newList() *list {
	return &list{}
}

func (l *list) append(n item) {
	l.val = append(l.val, n)
}

type num int

func (n num) asList() *list {
	l := newList()
	l.append(num(n))
	return l
}

func compare(left, right item) int {
	lList, leftIsList := left.(*list)
	rList, rightIsList := right.(*list)

	if !leftIsList && !rightIsList {
		// Both numbers
		return int(left.(num)) - int(right.(num))
	}

	if !leftIsList {
		lList = left.(num).asList()
	} else if !rightIsList {
		rList = right.(num).asList()
	}

	// Both lists
	for i := 0; i < len(lList.val); i++ {
		if i == len(rList.val) {
			// rlist has run out
			return 1
		}

		if r := compare(lList.val[i], rList.val[i]); r != 0 {
			return r
		}
	}

	if len(lList.val) < len(rList.val) {
		return -1
	}

	return 0
}

func scanList(input string, start int) (l *list, end int) {
	l = newList()
	i := start
	for i < len(input) {
		switch input[i] {
		case '[':
			var node *list
			node, i = scanList(input, i+1)
			l.append(node)
		case ',':
			i++
		case ']':
			return l, i + 1
		default:
			// number
			var n int
			fmt.Sscanf(input[i:], "%d", &n)
			l.append(num(n))
			for input[i] != ']' && input[i] != ',' {
				i++
			}
		}
	}

	return l, i
}

func main() {
	var listOfLists []*list
	index := 0
	sum := 0
	for {
		var ls, rs string
		fmt.Scanf("%s\n%s\n\n", &ls, &rs)
		if ls == "EOF" {
			break
		}
		index++

		ll, _ := scanList(ls, 1)
		rl, _ := scanList(rs, 1)
		listOfLists = append(listOfLists, ll)
		listOfLists = append(listOfLists, rl)

		val := compare(ll, rl)
		// fmt.Println(val)
		if val < 0 {
			sum += index
		}
	}

	fmt.Println("Sum : ", sum)

	// Append dividers
	d1 := newList()
	d1.append(num(2).asList())
	listOfLists = append(listOfLists, d1)

	d1 = newList()
	d1.append(num(6).asList())
	listOfLists = append(listOfLists, d1)

	// Now sort them for part 2
	sort.Slice(listOfLists, func(i, j int) bool {
		return compare(listOfLists[i], listOfLists[j]) <= 0
	})

	decoderKey := 1
	for i, l := range listOfLists {
		if len(l.val) == 1 {
			if innerList, ok := l.val[0].(*list); ok && len(innerList.val) == 1 {
				if n, ok := innerList.val[0].(num); ok && (n == 2 || n == 6) {
					decoderKey *= (i + 1)
				}
			}
		}
	}

	fmt.Println(decoderKey)

}
