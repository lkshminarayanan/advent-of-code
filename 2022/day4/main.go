package main

import "fmt"

func fullyContain(a1, a2, b1, b2 int) bool {
	return (a1 <= b1 && a2 >= b2) || (b1 <= a1 && b2 >= a2)
}

func doOverlap(a1, a2, b1, b2 int) bool {
	return (a1 <= b1 && b1 <= a2) || (a1 <= b2 && b2 <= a2) || (b1 <= a1 && a1 <= b2) || (b1 <= a2 && a2 <= b2)
}

func main() {
	var a1, a2, b1, b2 int
	var total int

	for {
		fmt.Scanf("%d-%d,%d-%d", &a1, &a2, &b1, &b2)
		if a1 == -1 {
			break
		}

		if fullyContain(a1, a2, b1, b2) || doOverlap(a1, a2, b1, b2) {
			total++
		}
	}

	fmt.Println(total)
}
