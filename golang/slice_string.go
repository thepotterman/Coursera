package main

import (
	"fmt"
    "math"
)

func main() {
	var text string
	var width int
	fmt.Scanf("%s %d", &text, &width)

	// Возьмите первые `width` байт строки `text`,
	// допишите в конце `...` и сохраните результат
	// в переменную `res`
	// ...

    var res string

    for i := 0; i < int(math.Min(float64(width), float64(len(text)))); i += 1 {
        res += string(text[i])
    }
    
    if len(res) != len(text) {
        res += "..."
    }

	fmt.Println(res)
}
