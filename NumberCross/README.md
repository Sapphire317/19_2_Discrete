# PA#1 NumberCross


## Build & Excute

1. ``` $ make ```

Enter make command to compile the source code.  

2. 
```
      $ ./NumberCross 
     20 23 30 29 34 6 9 21 19   
      8 2 1 8 1 3 5 7 6 18
      9 1 4 2 5 6 3 1 7 28
      3 5 1 4 9 1 3 9 1 8
      8 6 6 3 5 1 1 4 1 4       
      8 6 6 2 6 8 3 3 9 31
      8 7 8 8 4 5 2 1 1 18
      4 8 3 5 5 2 1 2 8 24
      1 2 8 8 8 3 7 2 7 22
      8 3 9 5 9 2 1 4 9 35 
  ```
      
Press CTRL+D after you finished putting data.  

3. You can get the result.  

If puzzle doesn't have any solution, it will print "No solution".  
If puzzle has a solution, it will be printed.   

## Input
All program receives the inputs by standard input.  
"NumbCross", its grid size is not fixed. Program doesn't know whether EOF. (Maximum 50 X 50) .
Except for sum of column and row, the input number should be between   1 ~ 9. or error will be occured.   
So, please tester enter "Ctrl+D" after all inputs are entered.  
Or Just use ```$ ./NumCross < input```.    



## Files
Makefile - Build script for CMake  
NumberCross.c - source code.  

