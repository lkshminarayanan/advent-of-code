package main

import "fmt"

func result(opp, self string) int {
	var result int
	switch self {
	case "A": // we played rock
		result = 1
		switch opp {
		case "A": // rock
			result += 3
		case "C": // scissor
			result += 6
		}
	case "B": // we played paper
		result = 2
		switch opp {
		case "A": // rock
			result += 6
		case "B": // paper
			result += 3
		}
	case "C": // we played scissor
		result = 3
		switch opp {
		case "B": // paper
			result += 6
		case "C": // scissor
			result += 3
		}
	}
	return result
}

func decideMove(opp, self string) string {

	switch self {
	case "X":
		// lose
		switch opp {
		case "A":
			return "C"
		case "B":
			return "A"
		case "C":
			return "B"
		}
	case "Y":
		// draw
		return opp
	case "Z":
		switch opp {
		case "A":
			return "B"
		case "B":
			return "C"
		case "C":
			return "A"
		}
	}

	return ""
}

func main() {
	var opp, self string
	var score int
	fmt.Scanf("%s %s", &opp, &self)
	for opp != "END" {
		score += result(opp, decideMove(opp, self))
		fmt.Scanf("%s %s", &opp, &self)
	}

	fmt.Println(score)
}
