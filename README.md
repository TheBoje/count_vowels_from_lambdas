# Bad code #3 - Submission 2

## Context

Create a program to display the number of vowels contained in a string. Programming language is free of choise, efficiency and readability are not required.

## Build from source

To build this project, you require `g++` and `make`, you can then run : 

```bash
make
```

## Run

Once the project is built, run with:

```bash
./aeif "<your sentence>"
```

or 

```bash
make run ARGS="<your sentence>"
```

For example:

```bash
$ make run ARGS="Id rather jump from a cliff than debug shit again"
Id rather jump from a cliff than debug shit again
...
14
```

## How this works ?

This program relies on Thread, for each character provided, a thread is created to handle it. Then for each created thread, 256 threads are created, each one responsible for an ASCII character. Each sub-thread then checks if its ASCII character and the "to check" character are the same, and if so, check if the character is a vowel. Then the thread completes a reduce function. This program creates $n * 256$ threads, as $n$ the length of the input.

## License

My submission for the third edition of the Bad code from "La click" is distributed under the DWTFYW License, see [LICENSE](LICENSE) for more details.

