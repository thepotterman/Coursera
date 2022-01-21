package main

import (
	"fmt"
)

func main() {
	var source string
	var times int
	// гарантируется, что значения корректные
	fmt.Scan(&source, &times)

	// возьмите строку `source` и повторите ее `times` раз
	// запишите результат в `result`
    // ...
    var result string

    for j := 0; j < times; j += 1 {
        result += source
    }

	fmt.Println(result)
}
