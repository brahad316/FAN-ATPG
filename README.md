# FAN-ATPG
An implementation of the FAN ATPG algorithm in c++ and verilog. the current version works on combinations circuits with any number of gates, primary inputs/outputs. However, all the logic gates needs to have precisely two inputs.
The circuit under test (CUT) should be described in a verilog file.

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
