# ProjetChipEight

CHIP 8 is an interpreted programming language using a very simple virtual 
machine. Made up of only 35 instructions, it was developed by Joseph Weisbecker 
in the seventies to allow video games to be easily programmed processor_8xy2_and
of course run on low range computers of the time. Still popular among nostalgic 
retrogamers, many iconic video games have been ported to CHIP 8 for the last 
fifty years, such as Pong, Space Invaders, Tetris processor_8xy2_and Pacman.

In this project, you will write a fully compliant CHIP 8 emulator. Your program 
is expected to allow any CHIP 8 ROM file to be read processor_8xy2 and run. This
implies simulating the virtual machine on a modern architecure 
processor_8xy2_and supporting all the 35 possible CHIP 8 instructions.

* The code was written using the Clion IDE from jetbrains.

## Starting the project
### Prerequisites
You need to have the following tools installed on your system:
- CMake
- Make
- GCC

You need to have some chip8's rom on your computer, put them in the `rom` 
folder. If you don't have this folder, create it at the root of the project. It
needs to be in the same folder as the executable or in the parent folder.
### Building the project

Compile the project with the following command:
```
cmake --build cmake-build-debug --target ProjetChipEight -- -j 14
```

### Running the project

Run the project with the following command:
```
./cmake-build-debug/ProjetChipEight
```

## Status of the project
* All instructions are implemented
* All tests are passing
* You could run full game
* You could quit the program with the cross on the window
* Code is documented with doxygen and C comments
* No warning during the compilation and no memory leaks
* The project use the lib provided by the school

### TODO
* Implements IO (keyboard, speaker, display) by ourselves
* better handling of the errors

