# BSH
A simple shell written in C for research purposes

<img src="https://i.imgur.com/tGnivG3.png" width="400">\
**BSH running on MacOS in ITerm2**

## Notice
At the minute, BSH isn't too good and I can't imagine why you would want to use it. But if you want to then go for it, or maybe you just want it to study the source code.

This is also by first project written in C rather than C++ so mind the bad code practices and anything I do wrong. If you spot stuff, feel free to correct.

This project was made possible by studying the code of [`lsh`](https://github.com/brenns10/lsh) by brenns10. Big thanks to his project, it was a great starting point.

## Features
- [x] Basic input and execute
- [x] Better input
- [x] Better parsing
- [x] Command history
- [ ] Pipe processing
- [ ] STD In/Out redirect
- [x] Tab auto complete
- [ ] Config file
- [ ] Support for ^Z (currently disabled)
- [ ] Scripting language (Lua like? maybe)

## Built in commands
|    |                                               |
|----|-----------------------------------------------|
|cd  | Change current directory                      |
|help| Shows built in commands and other information |
|exit| Exit out of the shell                         |

## Build
#### Unix
BSH has been confirmed to work on Mac, Linux and other Unix Operating Systems. It can be compiled with the simple command:\
`gcc -o bsh *.c`

#### Windows
BSH has not been confirmed to compile or run on windows, however, it can still be used using WSL perfectly fine.
