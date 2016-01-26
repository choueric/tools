package main

import (
	"bufio"
	"fmt"
	"os"
	//"strconv"
	"strings"
	"time"
)

/*
 * ./time_parse | sort -n
 * can sort the first number (i.e. cost)
 */

const startMark = "-----"
const endMark = "+++++"

type logItem struct {
	index int
	cost  time.Duration
}

func (p logItem) String() string {
	return fmt.Sprintf("%d: %v", p.index, p.cost)
}

func parse_time(line string) time.Duration {
	var hour, min, sec, nanosec int64
	var str string
	strs := strings.Split(line[0:len(line)-1], " ")
	fmt.Sscanf(strs[0], "%d:%d:%d.%d", &hour, &min, &sec, &nanosec)
	str = fmt.Sprintf("%dh%dm%ds%dns", hour, min, sec, nanosec)
	dur, err := time.ParseDuration(str)
	if err != nil {
		fmt.Printf("parse duration failed: %v\n", err)
		return 0
	}
	return dur
}

func print_logItems(items []logItem) {
	var sum time.Duration
	var num int64
	sum = 0
	num = 0
	for _, item := range items {
		fmt.Printf("%v\n", item)
		sum = sum + item.cost
		num = num + 1
	}
	str := fmt.Sprintf("%dns", sum.Nanoseconds()/num)
	avr, err := time.ParseDuration(str)
	if err != nil {
		fmt.Printf("parse average duration failed: %v\n", err)
		return
	}
	fmt.Printf("average: %v\n", avr)
}

func parse_log(filename string) {
	found := false
	index := 0
	items := make([]logItem, 0)
	var end, start time.Duration

	f, err := os.Open(filename)
	if err != nil {
		fmt.Printf("open failed: %v", err)
	}
	defer f.Close()

	reader := bufio.NewReader(f)
	for {
		if str, err := reader.ReadString('\n'); err != nil {
			break
		} else {
			if strings.Contains(str, startMark) {
				start = parse_time(str)
			}
			if strings.Contains(str, endMark) {
				end = parse_time(str)
				found = true
			}

			if found {
				cost := end - start
				items = append(items, logItem{index: index, cost: cost})
				found = false
				index = index + 1
			}
		}
	}
	print_logItems(items)
}

func print_usage() {
	fmt.Printf("Usage: ./time_parse <log_file>\n")
}

func main() {
	if len(os.Args) != 2 {
		print_usage()
		return
	}
	parse_log(os.Args[1])
}
