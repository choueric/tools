package goutil

import (
	"os"
	"strconv"
	"time"
)

func SaveFile(data []byte, filename string) {
	f, err1 := os.OpenFile(filename, os.O_RDWR|os.O_CREATE|os.O_APPEND, os.ModePerm)
	if err1 != nil {
		return
	}
	defer f.Close()
	f.WriteString(string(data[:len(data)]))
}

func Str2Time() {
	var testDate = "1439557418"
	testInt, _ := strconv.ParseInt(testDate, 10, 64)
	tm := time.Unix(testInt, 0)
	fmt.Println(tm)
}

func Time2Str() {
	tstr := strconv.FormatInt(time.Now().Unix(), 10)
	fmt.Println(tstr)
}
