
## Build & Excute

1. ''' $ make '''


2. ''' $ ./NumberCross '''
Entering the Input
All program receives the inputs by standard input.
"NumbCross", its grid size is not fixed. Program doesn't know whether EOF.
So, please tester enter "Ctrl+D" after all inputs are entered.
Or Just use $ ./NumCross < input

"NumCross" consists of label and grid. Label is first row and last Column.
Except them, grid is N * M and Input is N+1 *, M+1.
The values inside of grid are from 1 to 9. If any grid's value exceed 10, program makes error message and terminated.
When this programs receive first row, that is row_label(because label's shape is row), it will set up the column number M.
So, this program check whether next input consist of M cells + 1 column label(All input is entered, label's shape is column).
When it's not satisfied, program makes error message and receives input again.

Example
Build with Makefile. $ make all

Input your data that you want to solve

  $ ./NumCross                        $./NumCross < input.txt
      20 23 30 29 34 6 9 21 19   
      8 2 1 8 1 3 5 7 6 18
      9 1 4 2 5 6 3 1 7 28
      3 5 1 4 9 1 3 9 1 8
      8 6 6 3 5 1 1 4 1 4       or 
      8 6 6 2 6 8 3 3 9 31
      8 7 8 8 4 5 2 1 1 18
      4 8 3 5 5 2 1 2 8 24
      1 2 8 8 8 3 7 2 7 22
      8 3 9 5 9 2 1 4 9 35
Press CTRL+D after you finished putting data, if you chose first method.

You can get the result.

If puzzle don't have any solution, it will print "No solution"
If puzzle have a solution, it will be printed.
Files
Makefile - Build script for CMake
[hw1.c] - source codes.
After excute program. Some of below files are generated.
Input shown as grid.
Logic and constraints represented by Z3 syntax for launching Z3
Raw output from Z3 launched inside C program. Model is given if satisfiable, else error is given.
Parsed info also printed in console.
