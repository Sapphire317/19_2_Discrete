# 19_2_Discrete_Math
19-2 Discrete Math class assignment.



# HOW TO EXECUTE


## PA102 - Fill a pix!

1. Compile pix_lia.c.

``` $ g++ pix_lia.c -o pixlia ```

2. Get input file using Standard I/O

``` $ ./pixlia < p1 > pixliaz3 ```

3. Using Z3 solver, you can get the result.

``` $ z3 pixliaz3 > pixliaz3_output ```

4. Compile parse.c

``` $ g++ parse.c -o parser ```

5. Using parser, get the final result.

``` $ ./parser < pixliaz3_output ```
