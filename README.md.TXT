#GCD Calculator in C using Processes

A small Unix / Linux assignment in 'Operation Systems' course which calculate the GCD of pairs of numbers.
There are 2 versions, one that using a process for each pair, and one that using a conatant number of processes and communicate with the main process using pipes.


## V1

V1 reads pairs of numbers from STDIN and use the command line argument to pass the two numbers to the child process, than using the exit status of the child process to read calculated gcd.

### Usage

In `v1` folder, run `./v1_parent` in the command line.
In the main folder run `./v1/v1_parent` in the command line.

Press `ctrl + d` to stop.



## V2

V2 reads pairs of numbers from a file, the file name should be given in the command line as an argument.
The two numbers and the calculated gcd is passes through pipes.

### Usage

In `v1` folder, run `./v2_parent` in the command line.
In the main folder run `./v2/v2_parent` in the command line.