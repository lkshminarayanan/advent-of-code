package main

import (
	"advent-of-code/aocutils"
	"fmt"
	"strconv"
	"strings"
)

const (
	INPUT = "input"
	SPACE = ' '
)

func move(m [][]rune, i, j *int, steps int, direction rune) {

	rowLen := len(m[0])
	colLen := len(m)

	for steps > 0 {
		steps--

		nexti, nextj := *i, *j

		for {
			switch direction {
			case '>':
				nextj = (nextj + 1) % rowLen
			case 'v':
				nexti = (nexti + 1) % colLen
			case '<':
				nextj--
				if nextj < 0 {
					nextj = rowLen - 1
				}
			case '^':
				nexti--
				if nexti < 0 {
					nexti = colLen - 1
				}
			}

			if m[nexti][nextj] != SPACE {
				// valid tile
				break
			}
		}

		if m[nexti][nextj] == '#' {
			// wall cannot move further
			return
		}

		// move
		*i, *j = nexti, nextj
	}
}

func main() {
	// Parse input
	var m [][]rune
	var doneReadingMap bool
	var path string
	var maxLength int
	aocutils.ScanInput(INPUT, func(line string) {
		if len(line) == 0 {
			doneReadingMap = true
			return
		}

		if doneReadingMap {
			path = line
			return
		}

		m = append(m, []rune(line))
		if len(line) > maxLength {
			maxLength = len(line)
		}
	})

	for i := range m {
		if len(m[i]) < maxLength {
			m[i] = append(m[i], []rune(strings.Repeat(string(SPACE), maxLength-len(m[i])))...)
		}
	}

	var x, y int
	for j := 0; j < len(m[0]); j++ {
		if m[0][j] == '.' {
			// Found starting pos
			x = 0
			y = j
		}
	}

	direction := '>'

	i := 0
	for i < len(path) {
		switch path[i] {
		case 'R':
			switch direction {
			case '>':
				direction = 'v'
			case 'v':
				direction = '<'
			case '<':
				direction = '^'
			case '^':
				direction = '>'
			}
			i++
		case 'L':
			switch direction {
			case '>':
				direction = '^'
			case '^':
				direction = '<'
			case '<':
				direction = 'v'
			case 'v':
				direction = '>'
			}
			i++
		default:
			start := i
			for i < len(path) && path[i] != 'R' && path[i] != 'L' {
				i++
			}
			n, _ := strconv.Atoi(path[start:i])
			move(m, &x, &y, n, direction)
		}
	}

	password := 1000*(x+1) + 4*(y+1)

	switch direction {
	case '>':
		password += 0
	case 'v':
		password += 1
	case '<':
		password += 2
	case '^':
		password += 3
	}

	fmt.Println(password)
}
