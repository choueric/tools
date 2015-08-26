package goutil

import (
	"os"
)

func saveFile(data []byte, filename string) {
	f, err1 := os.OpenFile(filename, os.O_RDWR|os.O_CREATE|os.O_APPEND, os.ModePerm)
	if err1 != nil {
		return
	}
	defer f.Close()
	f.WriteString(string(data[:len(data)]))
}
