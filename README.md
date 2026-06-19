# TerminalCube

TerminalCube is an NxNxN Rubik's Cube Simulator that can be used in the terminal with a C++ library for easy use in projects.



To compile:

* Clone the Repository (`git clone https://github.com/Wallball-App/TerminalCube.git`)
* Navigate to the cloned directory
* Configure CMake with `cmake --preset [YOUR_PRESET]`
    * The possible presets are:
        - Windows with Mingw: `windows-mingw`
        - Windows with MSVC (Visual Studio 17 2022): `windows-msvc`
        - MacOS with clang: `macos-clang`
        - Linux with GCC: `linux-gcc`
    * NOTE: A C++ Compiler is Required to Build This Project
* Build the project: `cmake --build build/[YOUR_PRESET]`
* Navigate to `build/[YOUR_PRESET]` to view the generated files

How to Use (App):

- Open tcube.exe
- Enter the cube size (i.e. 3 for the standard Rubik's cube)
- Enter algorithms to execute the moves (e.g. R U R' U')
- Use standard Singmaster Notation (R for Right layer etc.)
- For Larger cubes, use this format: [Layer][Face][Wide][Number][Prime]
Layer is a number less than or equal to half the cube size
Face is the face turned (R, U, F, etc.)
Wide moves should be written with a lowercase "w"
Number is the amount of turns
Prime is shown by the symbol " ' ". If a move is prime, it is rotated counterclockwise instead of clockwise.
Example: 3Rw2'

How to Use (Library):
