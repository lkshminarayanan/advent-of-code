package main

import (
	"fmt"
	"strings"
)

const (
	width = 7
)

type stone struct {
	points [][][]int
	x, y   int
}

func (s *stone) height() int {
	return len(s.points)
}

func (s *stone) move(m [][]rune, x, y int) bool {
	newx, newy := s.x+x, s.y+y
	for _, row := range s.points {
		for _, point := range row {
			tx, ty := newx+point[0], newy+point[1]
			if tx < 0 || tx == width || ty < 0 {
				// invalid coordinates, cant move
				return false
			}

			if m[ty][tx] != '.' {
				// Cell not free movement not possible
				return false
			}
		}
	}

	// update stone
	s.x = newx
	s.y = newy
	return true
}

func (s *stone) rest(m [][]rune) {
	for _, row := range s.points {
		for _, point := range row {
			tx, ty := s.x+point[0], s.y+point[1]
			m[ty][tx] = '#'
		}
	}
}

func (s *stone) prune(m [][]rune) ([][]rune, int) {
	for k := s.height() - 1; k >= 0; k-- {
		for _, point := range s.points[k] {
			ty := s.y + point[1]
			var i int
			for i = 0; i < width; i++ {
				if m[ty][i] != '#' {
					// can't prune this line
					break
				}
			}
			if i == width {
				// prune!
				// fmt.Println("Pruning!", ty)
				return m[ty+1:], ty + 1
			}
		}
	}

	return m, 0
}

var stonePoints = [][][][]int{
	{
		// ####
		{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
	},
	{
		// .#.
		// ###
		// .#.
		{{1, 0}},
		{{0, 1}, {1, 1}, {2, 1}},
		{{1, 2}},
	},
	{
		// ..#
		// ..#
		// ###
		{{0, 0}, {1, 0}, {2, 0}},
		{{2, 1}},
		{{2, 2}},
	},
	{
		// #
		// #
		// #
		// #
		{{0, 0}},
		{{0, 1}},
		{{0, 2}},
		{{0, 3}},
	},
	{
		// ##
		// ##
		{{0, 0}, {1, 0}},
		{{0, 1}, {1, 1}},
	},
}
var nextStoneId int

func getNextStone(height int) *stone {

	s := &stone{
		points: stonePoints[nextStoneId],
		x:      2,
		y:      height + 3,
	}

	if nextStoneId++; nextStoneId == len(stonePoints) {
		nextStoneId = 0
	}

	return s
}

const directions = ">>><><<>>><<><<>>>><<<<>>><<<<><>>><>>>><<>>>><<<>>>><<>>><>>><<>>><>>><>>>><<>>>><<<>><>>><<<><<><>><<><>><<>>>><<>>><>>>><<<<>><<<>><<>>><<>><<<>>>><>>>><<<<><<<<>>><<<<>>><<<><<<>><<<<>>>><<<<>>><<<<>><>>><<<<>>><><<<>>>><><<<<>>><<<>><<<<>><<<>>>><<>><<<<>>>><<<><<>>><<>>>><>>><<>>><<>>><>>>><<<>>><<>>><<>>><>>>><<<<><<><<<><<><<<>>><<<>>>><>><<<>>><<<>><<<><<<<>>>><<<<>>><<<>><>>><>>>><<<>>>><>>>><<<<>>>><<<<><<<<><<<<>>><>>>><<<>><<<<>>><<<><<<>>>><<>>>><<<<>>><<<>><<<<><<<><<>>>><<<>>><>>><><<<>>><>><<<>><<>><<<>><<<>>>><<<<><<<<>>>><<>>><><<<>><<>><<>>>><<<><>><<<>><<<<>>>><<>><<<><<<<>><<>>><<<>><<><<<<>>>><<<<><<<<><<<><<<><<<<>><<<>><<<<>>>><><>>>><<<<><>><<>>><<>><>>><<>><<>>>><>><<<><>>><<<<>>><<>>><>><>>><<>><<<><<<<>>>><>>>><>>><<<>>><<<<>>><<<<>>><<<>>>><<<>><<<><>>>><<<<>><<<<><<<>>><<>>>><<><><<<><<<>><<<>>>><<<><<<<>>>><<<<>>>><><>>><>>>><<><<><<>><<><<<><<<<>>><>>>><<>>><>>>><<<>>>><<>>>><<<>><<<<><<><<<>>>><<<<>><<<>>>><>>><><<<<>>><<<><<>><<>>>><<>>>><>><<<>>>><>><<<<>>><<<>><>>>><<>><><<>>><<>><<>><<<><>>><<<>>>><>>>><<><<<<><<<<>>>><<<<>><<<>><<<>><<<>><<><<<<>>><<>>>><><>><<<<>>>><<>>>><>>>><<>>><<<<>><<><<<><<<<><<><<<<><<>>>><<<<>>>><>>>><<>>><<><<><>>>><>><<<<>><<>>>><<>>>><<<<>>>><<<<>><<<<>>>><<<>>><<>>><<><<<<>><>><<<>>><<<>>><>>>><>><>>>><<<<>><<<>>>><<>>><>><<<<>>><>><<<>>><<><<><<<<>>><<<<>>><<<<>>><<>><<>>><<>><<>><><<<<><<<>><><<<<>>>><<<><<<<>>>><<<>>><>><<<><<<>>><<<<>>><<<<>>><<>>><<<>>>><<<>>><<><<<>>><<>>><<>>><<>>>><<<>>><>><<>><<<>>><<<<>>>><>><<><<>>><<<>>><<<>><<<<>>>><<<<><<>>><<<>>><<>>>><<<>>>><>><<<<>>>><>>><<<<><<<>>>><>>><<>>><<><<>><<<><<<>>>><>>>><>>>><<<<>>><<<>>><<<>>>><<>>>><>><<<><>><<<>><<<>>>><<<>>><>>>><<<<><<<<>><>>>><<><<<>>><<><<<>>>><<<<>>>><<<<>>><<<<><>>>><<><>>><<>>><<>>><>><<<>><<<<>><<<<>><<>>><<><<>><<<>><<><<><>>><<<<><<<<>>>><>><<>>><<><<<<>>>><<<>><<<>>>><<<<>>><<<<>>><>>><<>><<<<>>>><<<>>><<><<<<>>>><<<<>>>><><<<>>><<<><<<>>><<<<><<<>>><<<>><<><>>>><><<>><<><<><<<>><>>>><>>>><>>>><<<<>>>><<>><<<><<<<><<>><><<<>><<<<>><<<<>>><<<>>>><<<<>><<<<>>>><<<<><>><<>>><<<<><>>>><<>>>><<<<>><>>>><><<<>><>>><<><><><<<>>>><<<>>><<><<<<>>>><>><<>>>><>>>><<<<>>>><<<<>>>><<<>>>><<<>><>><<<<><<<<>>>><>><<<><<>><<>>>><>><<><<<>>><<<><>>>><<>><<<<>>>><<<><<<><<<<>>><<>>>><>><<<<>>><<<>>><<<<><>>><>>><>>><>>><<>><<<>><<<>>><<<>><><<<<>><<<<><<>><>><<<>><<><>>><><<>>><<<<>>>><<<<>>>><>><<>>>><<<<>>><<>>>><<<<>>><<<<>>>><<>>>><><>><><<>><<<<>><>><<<<>>><>>><<<>>><<<<>>>><<>><<<>><<<<>>><<<<>>><<<<>>><<<<><<><><>>>><<<>>>><<<<>><<>>>><>><<><<<<><<<>><<<<><<>>><<<>>><><<<<>><<>>>><<<>><<<>>><<<>>><<<<><>>>><<<><<<><<<<>>><<<<><>>><>>><<>>>><<<>>>><<><<><<>><<>><<><>>><<<>>><<><<><<<<>><<<<>><<<>>>><>>>><<<<>>><>>><<<<>><<><<>>>><<<<>>>><>><>>><<<<>>><<<><<>><<><<>><>>><>><<>>><<<<>>>><<<>><<<>>><<>>>><<>><<<<>>>><<<>>>><<>><<<>>><<<><<<>><<<<>>>><<<>><<<<><<<<>>>><><>>><<><<>>><><<>>>><<<>>><<>>>><<<<><>>><<<<>>>><<<<>>>><>>><<>><<<>>>><<>>><<<><>>><<>>><<>>><<>>>><>>>><<>><>><<>>><<<>>>><<<>>><<<>><<<<><<<>>>><<>>><<<<><<<>>><><<<<><<>>><<<<>>><<>>><><<<<>>><>>>><>>>><<>>><<<>>>><>>>><<<>>><<<><<<><<<<><<>>>><<<<>><<>>>><<<>>><><<><<<<><<<><<<>><<<<>>>><<>><<>><<<>><<>><<<>>><<<<>><<<<>><<<>><<<<>>>><<>>>><><<<<>><>>>><<>>>><<<><<><>><<<>>>><<<>>><<><>>>><<<<><<<><<<<>>>><<>><<<<><<<<>>>><<>>>><<>>><<><>>>><><<<>>><<>><>><<<<>>>><<<>>>><<<>>><<<<>>><<<<>>>><<<>><<<<><<<<>>><>><<<>><<><<>><><<<>>>><>>>><<><<<<>>>><<<><<<<>>><<><>>>><>>><><<<<>>><><<<>>><<>>>><>>>><>>>><<<<><<>><>><<<><<<<><<>><>>>><>><<<>>><>>><<>>><<>><<<>>>><<<<>>>><<<>><<>><<>><>>>><<<<>><<>><<>><<<>><<>><<>>>><>>>><<>><<<<>>><<<<><<<>>><<><>>>><<<>><<<>>>><<<>><><>>><<<>><><>>>><<>><<<>>>><><>>><<>>><>>>><>>><<<>><<>>>><<>>>><<><<<>>><<><<<<>><<>><>>>><><<<>>>><<<<>><<<>><<<<>><<>><><<<<>>>><<><<<>><><<<<>>>><>>>><<<<>><>>><<<<>>>><>>>><>>>><><<<>><<<>>>><<<><<<>>><<<<>>>><<>>><<><<<<>>>><<<>><<<<><<<>>><<<>><<>>><<>><<>><<>>>><<<>><<>>><<<<>><>><<<>><<<<>>>><<<>>><>><<>>>><<<<>>>><<<><<>>><<>>><<<><<<<><<>>><<<<><<>>>><<<>>><<<>>>><<>>><<<>>><<>>><<<>><<<<>>>><><<<>>><>>>><<>><<<<>>>><<<>>>><>><<<>><<<><<><<<<>><<<<>>><<<<><<<<>>><<<<><<>><<>>><<<>><<<<>>><<<>><>>><<<<>><<<<><<><>>><<<<><<<>>>><<<<>><<<<>>>><<>>>><<>>>><<><<>>>><<<>>><<<<>><<>>>><<<<><<>>><<>><<><>>>><<>>>><<<>><<<>>><<><><<<<><<>>>><>>>><<<>>><><<<<>>>><<>>>><<<<><>><<>>>><>>>><<<<>>><>><<>><<<>>><<<><<<<>>>><<<<><<<>>>><<><>>><<<>>>><<><>>>><<<>><<>>>><<<<>><<<><<>>>><<>>><>>><>>>><<>>>><<<><>>>><<>>><<<<>>>><<>>><<<<>><<<<><>>><<>><>>><<><<<>><>>><<<<>><<>>><<><<<>><<<>>><<<<>>>><<>>><>><<><>>>><><><>>>><<>>>><<<>><<<<>>>><<>>>><<>>>><>>>><<>>>><<<<>><<>>>><<>><<<<>>><<<><>>>><<<>>><<<>><><>>>><>><<<<>>>><<><>>><<>>>><<>>>><<<>><<<>><><>><<>>>><<<>><<<>>><<>><<<<>>><<<<>>><>>>><<<><>><<<>><<><>>>><<<<>>><>>>><>>>><>>><>><>>><<<<>>>><>>>><>><<<<>><<<>><<<>>><>>><>>><<<><<<><<<<>>><<<<><<>><><>>><>>>><>>><><>>>><><<<><>>><<<<>>>><<>>>><><>>>><<>>>><>>>><<<>>><<><>><>>><>>><<<<><<<<>>><<<<><<<>>>><<<>><<>>><<<<>>><<><<<<>>>><>><<<<><><><<<><>>><<<>>>><<<>>><<<<>><<>>><<>><<<<>><>>>><<<><<>>><<>>>><>>><<<>>><<<>>>><<<<>>>><>>><<<><>>>><<<>>>><<>><<<>>><>>>><<<>>><<<><<<><<<<>><<<>>><<<>>><<<<>>>><<<<>>>><<<<>>>><<<<>><<<>><<>><<>>>><<><<>>>><><<<>><><<<><>>><<<<>>>><<>>><<>>>><<<<><>>><><<<>><<<><<>>>><<<>>><<<<>>>><<<<>>>><<<>>>><<<>><<<>>>><>><<<>>><<<>>>><<>>>><<<>>><<<<>>>><>><><<<<>>><<<>><<>><<>>>><<>><><<<<>>><<<>>>><><<>>>><<<>><<><<>><>>>><<<<>>><<<<><<>>>><<<>>><<<<>>>><>>><><<<>>><<<<>>>><<<>><<<<>>>><>>>><<<>>>><>>><<<<>>>><<>>>><><>><>><<<>><<>>>><<<>>><<>>>><<<><<<<>><<<<>><<<<>>>><<<><>><<<>>>><<>>><<<<>>><>>>><<<<>><>>><>>><<<>>>><<>>>><<<<><><<<>><><<><><><<<><<><<<<><<<<>>>><<<>>>><<<>>>><<<<><<>>>><<<><<>>>><<<>><<<<><<<>>><>>><>>><<<<>>><><<>><>>><<<<>>>><<>>><<<>>><<<>><<<<>>>><>><<>>>><<<>>>><<<><><<<>><<<>><<>><>>>><<>><<<>>>><<>>><<<>>><<><>><<>><>>><<>>><><<<>>>><<>>>><<>><>><>>><<<<><<<<>>><<<>>>><>>>><<<<>><>><<<>>>><<<<><<<<>><<>>>><<<<><<<<>>><<<><<>>><<><>><>>>><>>><<<<>>>><<>><>>>><<<>>><>>>><>>>><<<>><<>>><<<<>><<<<>>><>>><<<<>>>><>><<<<>>>><<<<>>><<<>><<>>><<<<>>>><>>><>><<>>><<<>>><><>><<<>><><>><<<><<<>><<<>>><<>>><<<>><<><<<<>>>><<<<>>><<<>><<>><<<<>><<<>>>><>>><<<>>><<<<>>><><<>><>><>><<<>>><<<<>><<<><<><<<<>>>><<>>><<<<>>><<<><<<>><<<>>>><<<<>><<><>><<<><<<>>>><<<<>><<<>>>><><<<<>>><<<>><>><>>><<><<<<><>><<>>>><>>>><>><<<<>>>><<<>>><<<<>>>><>><<><<>><<<<>>>><<<><>>>><<<<>>>><><<<><<<<>>>><<<<>>><<<>>><<>><<>>>><><>>>><<<>><<<<>>>><<<>><<<>><<<>><<<>>><><<<<><>>><<<<>>>><<<>><><<<>>><<><<>>>><<><<>>><<<>><<<<>>>><>>><><<<<>>>><<<>>>><<<<>>><>><<>>>><><<<<>>><<<>>><<>>><<<><<<>>>><>>>><<<<><<<<>><<<<>><<<<>>><<><>>><<<>><>><<<>>><><>>><>><<><<><<><<<<><<<><<><>>>><<>>><<<<>><<<>><<>>><<<<>><<<>>><<>>>><<>><<<<><<<<>><<<<>><<>><>>>><<<<>>>><<<<><<<><<<<>>>><<<<>>>><<<>>><<<>>>><<>>>><<>>>><<<>>>><<<>>>><<<>><>><<<<>>><<<><>><<<><<<<>>>><<<>><>>><<<>>>><<<<>><<<>>>><>>><<<>>>><<>>><<<<>><<<<>>>><<<>><<>><>>><>>>><<<<>>><<<<>>><<<><<>>><<<>>>><<<<>>><><<><<<<><>>>><>>><<<<>>>><>>>><<<<><<>><<>>><><>>><<<>>><<<><<<<>>>><<>>>><>>>><<>>><<>>>><<<><><<<><<<>><<<>>><<<<>><<<<><<<>><<<>><<<>><<<>>>><>>><<>><<<<>><<<<><<>>>><<>><<><<<><<<<>>><>>><<<>>>><<<<>>><<<<>>>><><<<>>>><>><<<><<><<<<>>>><><<>>>><<<>><<<<>>>><<>>><<>>>><<<>><<<<><<<<>><<<<>>>><<<>><<<<>>><><<><>>>><><<<<>>>><<<><<>>>><<<<>>><>><<<<>>>><<>><<<><<<>>>><<>>><<>>>><<<<>>>><<<><<><<<<>><<<>>>><><<><<<<>>>><<>>>><<<<>>><>>>><><<<>>>><<>><>>><<>>><<<<><<<>>><>>><<><>><<<<>>>><<<<>><<<>>><<<<><<<>>><<<><>>>><<>>>><<<<>>>><<<<>><<<><<>><>>><<<>>>><<<<>>>><<<<><<>>>><>><<<<>>><<><<>>>><><<<<>>><<<<>><<<><<<<>><<<<>>>><<><<<><<>>>><><<><<<>><<<<>><<<><<<>>><<<<>>><<><>><<<>>>><<>><<>>>><<><<>><<<<>><<<<>><><>>>><<<<>>>><<>>><<<>><<<<><<><>>>><<<>>><<>>>><<<<>>><<<><<>><<<>>>><<>>><>>>><<<<><<<>>>><<<>><>>>><<<<>><><<<><<<>>>><<<>>><<>>><<><>>>><<<><<<<>>><<<><<>><<<>>><><<><<><<>><<<>>><<<<><<>><<<>><>>><<<>><<<<>><><<><><<<<><<<>>>><<>><>>>><<<><<>><<<<>>><<<>>><<>><>>>><<<>><<>><><<<<>>>><<<>>>><>><<<><><>><<<<><<<>>>><<<>>><<<<>>>><><>>>><><<<<>><<<><<>>>><<<>>>><<>>>><>>>><<><>>>><>><<<<>>><<><<<<>>>><<>>>><<>>><<<<>><<<><>>><<>>><>><<><<><<<<><<<<><<><<<<><>>><>>>><<<>><<<<>><<<<>><<<>>><>>>><<<>>>><>>><<<<>>><>>>><<<><<<>>>><>>>><<<<>>><>>><<>>>><>>><<>>>><<<<>>>><<<<>>>><<<>><>>>><<>>>><>><><<>>>><>><<><<<<>>><<<<>>><<<<>>>><<>><<<<>>>><<<<>><<>><>><<<>><>><<>>>><<<<><<<<><<>>><<<<>><<>>><<<>>><<<<>>>><<><<<>>>><<<<>>><<<>><>>>><<<>>><<<<>>>><<<<>><<<<>>>><<<>>>><<>><<<<>>>><>>><<<<>><<<<><>>>><<<>>><>>>><<>><>>><<<>>><>>>><<<>><<><>><<>>>><>><<<<><<><<<<>>>><>><<>>>><<>><<<<>><<<<>>><<<<>>><<>>><>><<><>><<>>>><<<<>><<>>>><<<<>>>><>>><<<>>>><>>><>>><<><<<<>>>><>><<><<>>><>>>><<>>>><<>><>>>><<><<>>>><<>><<>>>><<<>><<><>>><<<<><<<><>><<><><<<>><<<<>>>><<<<>><<<>>>><<<<>>><<<<><<<<>>><<<<><<<><<<>>><<>>>><<>><><<>>><<<<>>>><<<<><<>>>><>><<>>>><<>><>>>><<<>>>><<<<>><<>><>>><<<<>>>><<<<>>><<>><<<>>>><<<><><<>>>><>>><<<>>>><<<>><<<<>>>><<>>><<>>><><<<<>>><<<>>><<<<>><>><<<><<<<>>>><>><<<<>>><<><<<>>><<<<>><>>><><<>>>><<>>>><<>><<><<<>><<<<>><><><>><<<>>><<><<<<>>><<>>>><<<>>>><>>>><>>>><<<<>>><<<<>>>><<<<>><>>><<>><<>><<>><<<>>><>>><><<<>>><<<<><<<><><<><<<>><<<<>>>><<<>>><>>>><<<>>><<<<>>>><<<<>><<<<>>><>>><>>><<>>><<>>>><>>><<<<>>><<><<<>>><<<<><<<>>><<<<>>><<><<<>>>><<<>><<<<>>><<<<>>>><<<><<<<><<<>>><<<>>>><<<<>>><>>>><<<<>>><<>>><<<>>>><<<<>>><<><<>>><<>>><><<><<<>>>><>>>><<<<>>><<<<>>><>>><<>>><<>>>><><<<<>>><<<><<<<>>>><<<><<<><<>>>><<<>>><<<<>>><<<>>>><<<><<<<>>>><<<>><<>>><<<<>><<>>>><<<<>>><>><<<<>>>><<>>>><>><<><<<>><>>>><<>>><<<><<>><>>><<<<><<<>><<<<>>><<>>><<<>>>><<<>><<<><<><<><<>>>><>>><<>>><<<<>><<<<>>><<<>>><<><>><>>><<><<<>>>><<<><>><<<><<<<>><<<>>>><<>>><>>><<<>>>><<>>><<<><<<<>>><>><<>>><><<><<<<>>><<<>>>><<<<>><<<>>>><><>>>><>><<>>><><><<>>>><<<>>>><<<<><<<>>><<<<><>>>><<><><<>>>><><<<<>>>><><<<<>>>><>>>><<<<>>><<<><<<>><<<><<<>>>><<<<><<<>>><<<>>><>>><>>>><<><>>>><<>>><<>>><<>><<<<><<>>>><<>>><<<><<<>><<<<>>>><>>>><<<>>><<>>>><<>>>><<<>>><<><<<>>><<<<>>><<<><>>><<<>>><>>>><<>><<<<>>>><<<<>>><<<>><<<<><"

// const directions = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>"

var nextDirectionId int

func getNextDirection() (x, y int) {
	c := directions[nextDirectionId]
	nextDirectionId++
	if nextDirectionId == len(directions) {
		nextDirectionId = 0
	}

	switch c {
	case '<':
		return -1, 0
	case '>':
		return 1, 0
	}

	return 0, 0
}

func print(m [][]rune) {
	for i := len(m) - 1; i >= 0; i-- {
		for _, c := range m[i] {
			fmt.Printf("%c", c)
		}
		fmt.Println()
	}
}

// const loops = 1000000000

const loops = 1000000000000

func main() {
	var doneCycle bool
	var cycleStart string
	var cycleStartHeight, cycleStartIndex int64

	var totalHeight int64

	var height int

	var m [][]rune

	for i := int64(0); i < loops; i++ {

		if i%10000000 == 0 {
			fmt.Println("-->", i, totalHeight)
		}

		s := getNextStone(height)
		// check if well height has to be increased
		for len(m) <= s.y+s.height() {
			m = append(m, []rune(strings.Repeat(".", width)))
		}

		for {
			// jet of gas pushes rock
			x, y := getNextDirection()
			// ignore return value as if they can't move doesn't matter
			s.move(m, x, y)

			// gravity pulls
			x, y = 0, -1
			if !s.move(m, x, y) {
				// unable to go down further
				s.rest(m)
				if currHeight := s.y + s.height(); height < currHeight {
					height = currHeight
				}

				var p int
				if m, p = s.prune(m); p != 0 {
					height -= p
					totalHeight += int64(p)
				}

				if height == 0 && !doneCycle {
					fmt.Println(height, nextDirectionId, nextStoneId, totalHeight, i)
					key := fmt.Sprint(height, nextDirectionId, nextStoneId)
					done := false
					if cycleStart == "" {
						cycleStart = key
						cycleStartHeight = totalHeight
						cycleStartIndex = i
					} else if cycleStart == key {
						done = true
					}

					if done {
						fmt.Println("I think we are done : ", totalHeight, ". Num of turns : ", i)
						cycleLength := i - cycleStartIndex
						tloops := loops - cycleStartIndex
						numCycles := tloops / cycleLength
						i = (int64(cycleLength)*int64(numCycles) + int64(cycleStartIndex))
						cycleHeight := totalHeight - cycleStartHeight
						totalHeight = int64(cycleHeight)*int64(numCycles) + int64(cycleStartHeight)
						fmt.Println(i, totalHeight, numCycles, cycleLength, cycleHeight)
						doneCycle = true
					}
				}

				break
			}
		}

	}

	// print(m)

	totalHeight += int64(height)
	fmt.Println(totalHeight)
}
