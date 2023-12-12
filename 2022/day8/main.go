package main

import (
	"bufio"
	"fmt"
	"os"
)

func parseInput() [][]int {
	var matrix [][]int
	for {
		//parse input
		reader := bufio.NewReader(os.Stdin)
		input, _ := reader.ReadString('\n')
		input = input[:len(input)-1]
		if input == "EOF" {
			break
		}

		var row []int

		for _, c := range input {
			row = append(row, int(c-'0'))
		}

		matrix = append(matrix, row)
	}

	return matrix
}

func problem1(input [][]int) {
	var output [][]bool
	for i := 0; i < len(input); i++ {
		output = append(output, make([]bool, len(input[0])))
	}

	// m x n
	m, n := len(input), len(input[0])

	for i := 0; i < m; i++ {
		// left edge
		output[i][0] = true
		leftMax := input[i][0]
		// right edge
		rj := n - 1
		output[i][rj] = true
		rightMax := input[i][rj]
		rj--
		// loop
		for j := 1; j < n-1; j++ {
			// from left
			if input[i][j] > leftMax {
				output[i][j] = true
				leftMax = input[i][j]
			}

			//from right
			if input[i][rj] > rightMax {
				output[i][rj] = true
				rightMax = input[i][rj]
			}
			rj--
		}
	}

	for j := 0; j < n; j++ {
		// top edge
		output[0][j] = true
		topMax := input[0][j]
		// bottom edge
		ri := m - 1
		output[ri][j] = true
		bottomMax := input[ri][j]
		ri--
		// loop
		for i := 1; i < m-1; i++ {
			// from top
			if input[i][j] > topMax {
				output[i][j] = true
				topMax = input[i][j]
			}

			//from bottom
			if input[ri][j] > bottomMax {
				output[ri][j] = true
				bottomMax = input[ri][j]
			}
			ri--
		}
	}

	var count int
	for _, row := range output {
		for _, visible := range row {
			if visible {
				count++
			}
		}
	}

	fmt.Println(count)
}

type op struct {
	l, r, u, d int
}

func problem2__(input [][]int) {
	var output [][]op

	// m x n
	m, n := len(input), len(input[0])

	for i := 0; i < m; i++ {
		row := make([]op, len(input[0]))
		output = append(output, row)
	}

	for i := 0; i < m; i++ {
		output[i][0].l = 0
		for j := 1; j < n; j++ {
			if input[i][j-1] < input[i][j] {
				output[i][j].l = output[i][j-1].l + 1
			} else {
				output[i][j].l = 1
			}
		}

		output[i][n-1].r = 0
		for j := n - 2; j >= 0; j-- {
			if input[i][j] > input[i][j+1] {
				output[i][j].r = output[i][j+1].r + 1
			} else {
				output[i][j].r = 1
			}
		}
	}

	for j := 0; j < n; j++ {
		output[0][j].u = 0
		for i := 1; i < m; i++ {
			if input[i][j] > input[i-1][j] {
				output[i][j].u = output[i-1][j].u + 1
			} else {
				output[i][j].u = 1
			}
		}

		output[m-1][j].d = 0
		for i := m - 2; i >= 0; i-- {
			if input[i][j] > input[i+1][j] {
				output[i][j].d = output[i+1][j].d + 1
			} else {
				output[i][j].d = 1
			}
		}
	}

	/*for i := 0; i < m; i++ {
		// left edge
		output[i][0].l = 0
		// right edge
		rj := n - 1
		output[i][rj].r = 0
		rj--
		// loop
		for j := 1; j < n; j++ {
			// from left
			if input[i][j-1] < input[i][j] {
				output[i][j].l = output[i][j-1].l + 1
			} else {
				output[i][j].l = 1
			}

			//from right
			if input[i][rj] > input[i][rj+1] {
				output[i][rj].r = output[i][rj+1].r + 1
			} else {
				output[i][rj].r = 1
			}
			rj--
		}
	}*/

	/*for j := 0; j < n; j++ {
		// top edge
		output[0][j].u = 0
		// bottom edge
		ri := m - 1
		output[ri][j].d = 0
		ri--
		// loop
		for i := 1; i < m; i++ {
			// from top
			if input[i][j] > input[i-1][j] {
				output[i][j].u = output[i-1][j].u + 1
			} else {
				output[i][j].u = 1
			}

			//from bottom
			if input[ri][j] > input[ri+1][j] {
				output[ri][j].d = output[ri+1][j].d + 1
			} else {
				output[ri][j].d = 1
			}
			ri--
		}
	}*/

	var maxscore int
	for i := 0; i < m; i++ {
		for j := 0; j < n; j++ {
			ops := output[i][j]
			score := ops.l * ops.r * ops.d * ops.u
			if maxscore < score {
				fmt.Println(i, j, ops)
				maxscore = score
			}
		}
	}

	// fmt.Println(output)

	fmt.Println(maxscore)
}

func problem2(input [][]int) {
	// m x n
	m, n := len(input), len(input[0])

	var maxScore int

	for i := 0; i < m; i++ {
		for j := 0; j < n; j++ {
			score := 1

			if i == 0 || j == 0 || i == m-1 || j == n-1 {
				// edge => 0
				continue
			}

			// 5 4 3 7 => 3
			// 5 4 7 => 2
			// 5 7 => 1
			// 5 3 => 1
			tmp := j - 1
			count := 0
			for tmp >= 0 && input[i][tmp] < input[i][j] {
				tmp--
				count++
			}
			if tmp != -1 {
				count++
			}
			score *= count

			tmp = j + 1
			count = 0
			for tmp < n && input[i][tmp] < input[i][j] {
				tmp++
				count++
			}
			if tmp != n {
				count++
			}
			score *= count

			tmp = i - 1
			count = 0
			for tmp >= 0 && input[tmp][j] < input[i][j] {
				tmp--
				count++
			}
			if tmp != -1 {
				count++
			}
			score *= count

			tmp = i + 1
			count = 0
			for tmp < m && input[tmp][j] < input[i][j] {
				tmp++
				count++
			}
			if tmp != m {
				count++
			}
			score *= count

			if maxScore < score {
				maxScore = score
			}
		}
	}

	fmt.Println(maxScore)
}

func main() {
	input := parseInput()
	problem2(input)
}
