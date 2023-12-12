package aocutils

import (
	"bufio"
	"io"
	"log"
	"net/http"
	"net/url"
	"os"
)

// ReadInput reads a file either from a URL or locally
func ReadInput(path string) string {
	if _, err := url.ParseRequestURI(path); err == nil {
		// URL
		response, err := http.Get(path)
		if err != nil {
			log.Fatalln(err)
		}

		defer response.Body.Close()

		// Copy data from the response to standard output
		b, err := io.ReadAll(response.Body)
		if err != nil {
			log.Fatalln(err)
		}

		return string(b)
	} else {
		// Not an URL
		dat, err := os.ReadFile(path)
		if err != nil {
			log.Fatalln(err)
		}
		return string(dat)
	}
}

func ScanInput(path string, scanFunc func(line string)) {
	file, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		scanFunc(scanner.Text())
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
