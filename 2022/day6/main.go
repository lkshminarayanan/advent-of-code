package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	//parse input
	reader := bufio.NewReader(os.Stdin)
	input, _ := reader.ReadString('\n')

	// window begin, end
	var begin, end int
	m := make(map[rune]int)

	markerSize := 14

	for i, c := range input {
		m[c]++

		// remove old char before window
		if i-markerSize >= 0 {
			oldC := input[i-markerSize]
			if m[rune(oldC)] == 1 {
				delete(m, rune(oldC))
			} else {
				m[rune(oldC)]--
			}

			begin++
		}

		fmt.Print("[")
		for k, v := range m {
			fmt.Printf("%c:%d ", k, v)
		}
		fmt.Println("]")

		if len(m) == markerSize {
			// found the marker
			fmt.Println(i + 1)
			break
		}

		end++
	}

}
