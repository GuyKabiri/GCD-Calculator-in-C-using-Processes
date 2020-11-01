# GCD Calculator in C using Processes

A small Unix / Linux assignment in the 'Operation Systems' course which calculates the GCD of pairs of numbers.
There are 2 versions, one that using a process for each pair, and one that uses a constant number of processes and communicates with the main process using pipes.


## V1

V1 reads pairs of numbers from STDIN and use the command line argument to pass the two numbers to the child process, then using the exit status of the child process to read calculated gcd.

#### Usage

Run `./v1_parent` in the command line in `v1` folder, or `./v1/v1_parent` in the main folder.

Press `ctrl + d` to stop.




## V2

V2 reads pairs of numbers from a file, the file name should be given in the command line as an argument.
The two numbers and the calculated gcd is passed through pipes.

#### Usage

Run `./v2_parent file_name` in the command line in `v2` folder, or `./v2/v2_parent file_name` in the main folder, where `file_name` is a file with pairs of numbers seperated with spaces or tabs, one pair in each row.
