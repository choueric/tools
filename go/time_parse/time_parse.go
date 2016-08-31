package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

/*
 * ./time_parse | sort -n
 * can sort the first number (i.e. cost)
 */

type logItem struct {
	name    string
	process string
	cost    int
}

func (p logItem) String() string {
	return fmt.Sprintf("%d\t:%s\t\t- %s", p.cost, p.name, p.process)
}

func handle_line(line string) (strs []string, time int64) {
	strs = strings.Split(line[0:len(line)-1], ":")
	for i, str := range strs {
		strs[i] = strings.TrimSpace(str)
	}
	time, _ = strconv.ParseInt(strs[0], 10, 64)
	return
}

func print_logItems(items []logItem) {
	for _, item := range items {
		fmt.Printf("%v\n", item)
	}
}

func parse_log(filename string) {
	items := make([]logItem, 0)

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
			start, start_time := handle_line(str)
			str, err := reader.ReadString('\n')
			if err != nil {
				break
			}
			_, end_time := handle_line(str)
			cost := int(end_time - start_time)
			items = append(items, logItem{name: start[3], process: start[2], cost: cost})
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
