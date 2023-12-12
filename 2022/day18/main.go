package main

import "fmt"

const (
	size = 1
)

func main() {

	m := make(map[string]interface{})
	for {
		var x, y, z int
		if n, _ := fmt.Scanf("%d,%d,%d\n", &x, &y, &z); n == 0 {
			break
		}

		for _, d := range []struct {
			x1, y1, z1 int
			x2, y2, z2 int
		}{
			// Diaganols of all surfaces
			{x, y, z, x, y + size, z + size},
			{x + size, y, z, x + size, y + size, z + size},
			{x, y, z, x + size, y, z + size},
			{x, y + size, z, x + size, y + size, z + size},
			{x, y, z, x + size, y + size, z},
			{x, y, z + size, x + size, y + size, z + size},
		} {
			key := fmt.Sprint(d.x1, d.y1, d.z1, d.x2, d.y2, d.z2)
			if _, exists := m[key]; exists {
				delete(m, key)
			} else {
				m[key] = nil
			}
		}
	}

	fmt.Println(len(m))
}
