package main

import (
	"fmt"
)

func main() {
	var ip string
	var total int
	fmt.Scanf("%s", &ip)
	for ip != "e" {
		t1 := ip[:len(ip)/2-1]
		t2 := ip[(len(ip) / 2):]

		m := make(map[rune]interface{})

		for _, c := range t1 {
			m[c] = nil
		}

		for _, c := range t2 {
			if _, exists := m[c]; exists {
				var p int
				if c <= 90 {
					p = int(c-'A') + 27
				} else {
					p = int(c-'a') + 1
				}

				// fmt.Printf("\n%c %d\n", c, p)

				total += p

				break
			}
		}

		ip = ""
		fmt.Scanf("%s", &ip)
	}

	fmt.Println(total)
}
