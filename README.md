# BSH
A simple shell written in C for research purposes

<img src="https://i.imgur.com/tGnivG3.png" width="400">\
**BSH running on MacOS in ITerm2**

## About
This is the reincarnation of an old shell I write in C a couple of years ago. While my aim with this project isn't for it to be a fully feature complete and standard complying shell, I am expecting it to work just as well as any other shell would, with the added features I plan on implementing.


## Features
- [x] Basic input and execute
- [ ] Better input
- [x] Better parsing
- [ ] Command history
- [ ] Pipe processing
- [ ] STD In/Out redirect
- [ ] Tab auto complete
- [x] Config file
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
BSH has been tested on MacOS & Linux, I don't see any good reason why other Unix Operating Systems would struggle to build BSH but you never know.

BSH can be built simply with the following commands:
```shell
mkdir build && cd build
cmake ..
make
```

#### Windows
BSH has not been confirmed to compile or run on windows, however, it can still be used using WSL perfectly fine.
