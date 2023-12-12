package main

import (
	"fmt"
)

var digits = map[rune]int{
	'=': -2,
	'-': -1,
	'0': 0,
	'1': 1,
	'2': 2,
}

func toDecimal(num string) int {
	var value int
	place := 1
	for i := len(num) - 1; i >= 0; i-- {
		value += place * digits[rune(num[i])]
		place *= 5
	}

	return value
}

func replaceAtIndex(in string, r rune, i int) string {
	out := []rune(in)
	out[i] = r
	return string(out)
}

func toSnafu(num int) string {
	res := "2"
	for toDecimal(res) < num {
		res += "2"
	}

	for i := 0; i < len(res); i++ {
		var next rune
		for {
			if res[i] == '=' {
				break
			}
			switch res[i] {
			case '-':
				next = '='
			case '0':
				next = '-'
			case '1':
				next = '0'
			case '2':
				next = '1'
			}
			updated := replaceAtIndex(res, next, i)
			if toDecimal(updated) < num {
				break
			}
			res = updated
		}
	}

	fmt.Println(res)
	return ""
}

func main() {
	var ip string
	var sum int
	for {
		fmt.Scanf("%s\n", &ip)
		if ip == "EOF" {
			break
		}

		sum += toDecimal(ip)
	}

	fmt.Println(sum, toSnafu(sum))
}
