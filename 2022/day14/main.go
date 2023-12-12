package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const (
	/*
		maxX   = 50
		maxY   = 25
		startX = 47
		startY = 0
	*/

	maxX   = 1000
	maxY   = 1000
	startX = 500
	startY = 0

	ROCK = '#'
	AIR  = '.'
	SOIL = 'o'
)

func print(m [][]rune) {
	fmt.Println()
	for i := 0; i < maxY; i++ {
		for j := 0; j < maxX; j++ {
			fmt.Printf("%c", rune(m[i][j]))
		}
		fmt.Println()
	}
}

func updateSoil(m [][]rune, prevX, prevY *int, x, y int) {
	m[*prevY][*prevX] = AIR
	m[y][x] = SOIL
	*prevX, *prevY = x, y
}

func fillSoil(m [][]rune) int {
	count := 0
	for {
		count++
		soilPrevX, soilPrevY, x, y := 0, 0, startX, startY
		distMoved := 0
		for {
			// time.Sleep(50 * time.Millisecond)
			if y+1 == maxY {
				// No more space to go down
				return count - 1
			}
			if m[y+1][x] == AIR {
				y++
			} else if x > 0 && m[y+1][x-1] == AIR {
				y++
				x--
			} else if x+1 < maxX && m[y+1][x+1] == AIR {
				y++
				x++
			} else {
				break
			}

			updateSoil(m, &soilPrevX, &soilPrevY, x, y)
			distMoved++

			// print(m)
		}

		if distMoved == 0 {
			break
		}
	}

	return count
}

func main() {
	var m [][]rune
	for i := 0; i < maxY; i++ {
		var row []rune
		for j := 0; j < maxX; j++ {
			row = append(row, AIR)
		}
		m = append(m, row)
	}

	reader := bufio.NewReader(os.Stdin)

	// scan input
	var maxReadY int
	for {
		input, _ := reader.ReadString('\n')
		input = input[:len(input)-1]
		if input == "EOF" {
			break
		}

		lines := strings.Split(input, " -> ")
		prevX, prevY := -1, -1
		for _, line := range lines {
			points := strings.Split(line, ",")
			x, _ := strconv.Atoi(points[0])
			y, _ := strconv.Atoi(points[1])
			if y > maxReadY {
				maxReadY = y
			}
			if prevX == -1 {
				prevX, prevY = x, y
				continue
			}
			for prevX != x {
				m[prevY][prevX] = ROCK
				if prevX < x {
					prevX++
				} else {
					prevX--
				}
			}
			for prevY != y {
				m[prevY][prevX] = ROCK
				if prevY < y {
					prevY++
				} else {
					prevY--
				}
			}
			m[prevY][prevX] = ROCK
		}
	}

	// Draw a Line at Y
	maxReadY += 2
	for j := 0; j < maxX; j++ {
		m[maxReadY][j] = ROCK
	}

	fmt.Println(fillSoil(m))
}
