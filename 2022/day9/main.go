package main

import (
	"fmt"
	"math"
)

type rope struct {
	// hx, hy, tx, ty int
	knots         int
	x, y          []int
	visitedByTail map[string]interface{}
}

func (r *rope) markTailVisited() {
	loc := fmt.Sprintf("%d/%d", r.x[r.knots-1], r.y[r.knots-1])
	r.visitedByTail[loc] = nil
}

func newRope(knots int) *rope {
	r := &rope{
		knots:         knots,
		x:             make([]int, knots),
		y:             make([]int, knots),
		visitedByTail: make(map[string]interface{}),
	}
	r.markTailVisited()

	return r
}

func (r *rope) moveHead(d string) {
	switch d {
	case "L":
		r.x[0]--
	case "R":
		r.x[0]++
	case "U":
		r.y[0]++
	case "D":
		r.y[0]--
	}
}

/*
func (r *rope) moveTail() {
	xdiff := r.hx - r.tx
	ydiff := r.hy - r.ty
	if xdiff > 1 || (xdiff == 1 && math.Abs(float64(ydiff)) > 1) {
		r.tx += 1
	} else if xdiff < -1 || (xdiff == -1 && math.Abs(float64(ydiff)) > 1) {
		r.tx -= 1
	}

	if ydiff > 1 || (ydiff == 1 && math.Abs(float64(xdiff)) > 1) {
		r.ty += 1
	} else if ydiff < -1 || (ydiff == -1 && math.Abs(float64(xdiff)) > 1) {
		r.ty -= 1
	}

	r.markVisited()
}
*/

func (r *rope) moveKnots() {

	for i := 1; i < r.knots; i++ {
		xdiff := r.x[i-1] - r.x[i]
		ydiff := r.y[i-1] - r.y[i]

		if xdiff > 1 || (xdiff == 1 && math.Abs(float64(ydiff)) > 1) {
			r.x[i] += 1
		} else if xdiff < -1 || (xdiff == -1 && math.Abs(float64(ydiff)) > 1) {
			r.x[i] -= 1
		}

		if ydiff > 1 || (ydiff == 1 && math.Abs(float64(xdiff)) > 1) {
			r.y[i] += 1
		} else if ydiff < -1 || (ydiff == -1 && math.Abs(float64(xdiff)) > 1) {
			r.y[i] -= 1
		}

	}

	r.markTailVisited()
}

func (r *rope) process(d string, n int) {
	for n != 0 {
		n--

		// move head
		r.moveHead(d)

		// move knots
		r.moveKnots()

		// fmt.Println(r.hx, r.hy, r.tx, r.ty)
	}
}

func main() {
	var d string
	var n int

	var knots int
	fmt.Scanf("%d\n", &knots)
	r := newRope(knots)

	for {
		nr, _ := fmt.Scanf("%s %d\n", &d, &n)
		if nr == 0 {
			break
		}

		r.process(d, n)
	}

	fmt.Println(len(r.visitedByTail))
}
