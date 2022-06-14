package main

// сюда писать код

func ExecutePipeline(jobs ...job) {
    in := make(chan interface{})
    out := make(chan interface{})
    go jobs[0](in, out)
    for i := range(out) {
        in_i := make(chan interface{})
        out_i := make(chan interface{})
        in_i <- i
        for j := 1; j < len(jobs); j++ {
            if(j % 2 == 1) {
                go jobs[j](in_i, out_i)
            } else {
                go jobs[j](out_i, in_i)
            }
        }
        close(in_i)
        close(out_i)
    }
    close(in)
    close(out)
}

func SingleHash(in, out chan interface{}) {
}

func MultiHash(in, out chan interface{}) {
}

func CombineResults(in, out chan interface{}) {
}


