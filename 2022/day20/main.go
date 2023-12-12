package main

import "fmt"

type number struct {
	value      int64
	next, prev *number
}

func newNumber(value int64) *number {
	return &number{
		value: value,
	}
}

const (
	KEY = 811589153
	// KEY   = 1
	LOOPS = 1
)

func (num *number) move(n int64) {

	numOfMoves := num.value % (n - 1)
	fmt.Println(num.value, numOfMoves)
	tmp := num
	if num.value > 0 {
		for numOfMoves > 0 {
			numOfMoves--
			tmp = tmp.next
		}
	} else if num.value < 0 {
		for numOfMoves < 0 {
			numOfMoves++
			tmp = tmp.prev
		}
		tmp = tmp.prev
	}

	if num == tmp || num == tmp.next {
		// nothing to do
		return
	}

	num.prev.next = num.next
	num.next.prev = num.prev
	num.next = tmp.next
	num.prev = tmp
	tmp.next = num
	num.next.prev = num
}

func (num *number) find(idx int) int64 {
	tmp := num
	for idx != 0 {
		idx--
		tmp = tmp.next
	}

	return tmp.value
}

func main() {
	var input []*number
	var zero *number
	for {
		var n int
		if n, _ := fmt.Scanf("%d\n", &n); n == 0 {
			break
		}

		num := &number{
			value: int64(n) * KEY,
		}

		if len(input) > 0 {
			lastNum := input[len(input)-1]
			num.prev = lastNum
			lastNum.next = num
		}

		input = append(input, num)

		if n == 0 {
			zero = num
		}
	}

	lastNum := input[len(input)-1]
	firstNum := input[0]
	firstNum.prev = lastNum
	lastNum.next = firstNum

	n := int64(len(input))

	for _, num := range input {
		fmt.Print(num.value, ", ")
	}
	fmt.Println()

	for i := 0; i < LOOPS; i++ {
		for _, num := range input {
			num.move(n)
		}
	}

	x1 := zero.find(1000 % int(n))
	x2 := zero.find(2000 % int(n))
	x3 := zero.find(3000 % int(n))

	fmt.Println(x1 + x2 + x3)
}
