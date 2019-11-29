![](https://github.com/mary060196/CISC3160_Project/blob/master/The%20Toy%20Language.png?raw=true)
# <p align="center">The Toy Language Interpreter Project</p>
## Details

**Author**: Miriam Briskman

**Supervised by**: Professor Neng-Fa Zhou

**Course**: CISC 3160, "Programming Languages", of CUNY Brooklyn College

**Semester**: Fall 2019

**Used Language:** C++

**Assignment's Description**: http://www.sci.brooklyn.cuny.edu/~zhou/teaching/cis3160/project.html
## Getting Started
- Save all the files into a single folder on your computer.
- Create a text file, say `myProg.txt`, with a program that you want to interpret in that same folder.
- Using a command-line processor:
  - Set the current directory to where the files were downloaded.
  - If you prefer to use g++ as a compiler, type

    `g++ -o Toylang.exe Toylang.cpp`

    to compile and create an executable named `Toylang.exe`.
  - Type

    `Toylang myProg` or `Toylang myProg.txt` (on Windows)

    or

    `./Toylang myProg` or `./Toylang myProg.txt` (on Unix / Linux and MAC)

    to run the intepreter on the file `myProg.txt` that you created.
  - Alternatively, type

    `Toylang` (on Windows)

    or

    `./Toylang` (on Unix / Linux and MAC)

    and follow the instructions to enter the name of the textfile to interpret.

## Examples
- See program examples at http://www.sci.brooklyn.cuny.edu/~zhou/teaching/cis3160/project.html.
- Try the interpreter on `errors1.txt`, `errors2.txt` and `test.txt` found in this repository. 
  - The files `errors1.txt` :
  
    ```
    ampersand = &;
    y++;
    6 = 6;
    w z = 3;
    anotherVar = 7 8;
    num = 15*8+(16+ -4*9));
    c = 0-8;;
    res = 2+2*;
    g;
    = 14;
    x = ((5+6);
    cat = 5(2+3);
    _ == 0;
    temp = 2+*5;
    e = (3-);
    y = t;
    y = 4+9
    ```
    and `errors2.txt` :
    
    ```
    x
    ```
    test for 17 different syntactic errors and 1 semantic error (uninitialized variables). 
  - The file `test.txt` :
  
    ```
    _FOO = 9+9*7--5*7--8+9;
    bar007 = 9+0-5--_FOO;
    bla_bla = ---0*7+8+8+8;
    y=1+2+3+4+5;
    x=y;
    z = x-y-y+y-x;
    p=1;
    q=2;
    s= -p*-q+p*q*p;
    r=p*-++-+-p*(p+p+p);
    t=2*p+5*5+8+8;
    a=+t--t+t*(p+(q-p*-q));
    b=-(a)-(a);
    wow = 1;
    wow = wow + 1;
    ```
    contains _no_ errors. Use it to see how the interpreter successfully prints the values of all the variables.
<hr>

**Thank you for Reading!** Miriam Briskman
