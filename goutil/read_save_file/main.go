package main

import (
	"fmt"
	"os"
)

const fileName = "/home/zhs/workspace/TK1/nfs/raw_nv12"

func saveFileBin(data []byte, filename string) {
	f, err := os.OpenFile(filename, os.O_RDWR|os.O_CREATE, os.ModePerm)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer f.Close()
	f.Write(data)
}

func main() {
	f, err := os.Open(fileName)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer f.Close()

	frameSize := 1920 * 1080 * 3 / 2
	data := make([]byte, frameSize)

	count, err := f.Read(data)
	if err != nil {
		fmt.Println(err)
	}
	if count != frameSize {
		fmt.Println("size not match")
	}
	saveFileBin(data, "./nv12_1")

	f.Seek(int64(100*frameSize), os.SEEK_SET)
	count, err = f.Read(data)
	if err != nil {
		fmt.Println(err)
	}
	if count != frameSize {
		fmt.Println("size not match")
	}
	saveFileBin(data, "./nv12_2")
}
