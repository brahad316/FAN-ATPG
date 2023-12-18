# FAN-ATPG
an implementation of the FAN ATPG algorithm in c++ and verilog.

# Running the Code

## Compiling
Compile the code by running 
```
g++ parse.cpp


```

To change the verilog file, go to line 24 and change

## Running Code
Simply run -
```
./a.out
```

## Giving the input
The user is prompted for the input twice
1. First give the wire where the fault is without any trailing whitespaces. 
2. Enter 0 or 1 for the stuck-at fault value.

That's it, this will run the FAN algorithm to generate the test patterns to check that fault.
