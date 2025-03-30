# Visualizer in C
Using Raylib

## Supported Functions
This program currently supports the following mathematical functions:

- `sin(x)`  - Sine function
- `cos(x)`  - Cosine function
- `tan(x)`  - Tangent function
- `arcsin(x)` - Inverse sine
- `arccos(x)` - Inverse cosine
- `arctan(x)` - Inverse tangent
- `log2(x)` - Logarithm base 2
- `log10(x)` - Logarithm base 10
- `log(x)` - Natural logarithm (base e)

Any function other than these will plot just the argument of the function

## Limitations
- Only **single-variable functions** are supported at the moment.
- Multi-variable or parametric equations are **not yet supported**.
- Don't put unnecessary right paranthesis `)` it may crash the program.

## Requirements
- Raylib must be installed on your system.
- Compatible with **Ubuntu** and **Windows**.

## Building Raylib on Ubuntu
Run
```git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake .. -DPLATFORM=Desktop
make -j$(nproc)
sudo make install
cd ../..```
## How to Run
1. Compile the program with:
   ```sh
   gcc -o plotter visualizer.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   ```
2. Run the program:
   ```sh
   ./plotter
   ```
## Running on Windows
- Open the `visualizer.exe` file  inside `./visualizer` 

## Future Plans
- Add multi-variable function support.
- Improve UI for function input.
- Implement additional mathematical functions.

Please suggest improvements!
