package main

import (
	"fmt"
	"math"
	"sort"
)

const (
	key = 2000000
)

func distance(sx, sy, bx, by int) int {
	return int(math.Abs(float64(sx-bx)) + math.Abs(float64(sy-by)))
}

func main_problem1() {
	minx := math.MaxInt
	maxx := math.MinInt
	for {
		var sx, sy, bx, by int
		if n, _ := fmt.Scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by); n != 4 {
			break
		}

		xdist := distance(sx, sy, bx, by) - int(math.Abs(float64(sy-key)))

		if lx := sx - xdist; lx < minx {
			minx = lx
		}

		if rx := sx + xdist; rx > maxx {
			maxx = rx
		}
	}

	fmt.Println(maxx, minx)
}

const (
	size = 4000000
)

type row struct {
	intervals [][]int
}

func main() {

	rows := make([]row, size)
	for {
		var sx, sy, bx, by int
		if n, _ := fmt.Scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by); n != 4 {
			break
		}

		dist := distance(sx, sy, bx, by)

		for i := 0; i < size; i++ {
			xdist := dist - int(math.Abs(float64(sy-i)))

			if xdist < 0 {
				// There is no reach in this line
				continue
			}

			lx := sx - xdist
			rx := sx + xdist
			if lx > rx {
				lx, rx = rx, lx
			}
			rows[i].intervals = append(rows[i].intervals, []int{
				lx, rx,
			})
		}
	}

	// Sort
	for ii := 0; ii < size; ii++ {
		intervals := rows[ii].intervals
		sort.Slice(intervals, func(i, j int) bool {
			return intervals[i][0] < intervals[j][0] ||
				(intervals[i][0] == intervals[j][0] && intervals[i][1] < intervals[j][1])
		})
	}

	for i, row := range rows {
		start := row.intervals[0][0]
		end := row.intervals[0][1]
		for _, interval := range row.intervals {
			if interval[1] <= end {
				// interval already merged
				continue
			}

			// check if new interval head is within existing interval
			if start <= interval[0] && interval[0] <= end+1 {
				end = interval[1]
			} else {
				fmt.Println("Found :", i, start, end, interval)
				fmt.Println("Answer :", (end+1)*4000000+i)
				break
			}
		}
		// fmt.Println(i, ":", start, end, end-start)
	}

	/*for _, r := range rows {
		fmt.Println(r.intervals)
	}*/
}
