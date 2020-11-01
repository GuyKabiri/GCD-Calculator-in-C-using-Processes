// Guy Kabiri 312252224

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "v2_parent.h"

#define INPUT 0
#define OUTPUT 1
#define childFileName "./v2_child"
#define MAX_CHARS_IN_LINE 250
#define ILLEGAL_INPUT -2

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Not enough or too much rguments\n");
        exit(EXIT_FAILURE);
    }

    FILE* inputFile = fopen(argv[1], "r");
    //  if can not open the input file
    if (!inputFile)
    {
        fprintf(stderr, "Error open the input file '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    //  R = READ = parent reading pipe to read from a child
    //  W = WRITE = parent writing pipe to write to a child
    int pipeR1[2], pipeR2[2], pipeW1[2], pipeW2[2];


    //  create pipes and execute child processes
    createProcesses(pipeR1, pipeR2, pipeW1, pipeW2);    


    //  start parent process
    parentProcessMainFunction(pipeR1, pipeR2, pipeW1, pipeW2, inputFile);  


    //  close pipes so children processes will exit
    if (close(pipeR1[0]) < 0 || close(pipeR2[0]) < 0 || close(pipeW1[1]) < 0 || close(pipeW2[1]) < 0)
    {
        fprintf(stderr, "Error close parent pipe\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}


//  create read and write pipes for each child process and merge them into STDIN and STDOUT
//  close unused end of pipes in both parent and children processes
//  prepare the child processes arguments vector, and execute them
void createProcesses(int* pipeR1, int* pipeR2, int* pipeW1, int* pipeW2)
{
    //  pointer to user in the loop
    int *writingPipe = NULL, *readingPipe = NULL;
    pid_t pid;

    for (int i = 1; i <= 2; i++)    //  2 child processes
    {
        //  switch to assign the right pipes to the right child
        switch (i)
        {
        case 1:
            writingPipe = pipeW1;
            readingPipe = pipeR1;
            break;
        
        case 2:
            writingPipe = pipeW2;
            readingPipe = pipeR2;
            break;
        }

        //  if creating writing pipe failed
        if (pipe(writingPipe) != 0)
        {
            fprintf(stderr, "Error create writing pipe\n");
            exit(EXIT_FAILURE);
        }

        //  if creating reading pipe failed
        if (pipe(readingPipe) != 0)
        {
            fprintf(stderr, "Error create reading pipe\n");
            exit(EXIT_FAILURE);
        }

        pid = fork();

        if (pid < 0)
        {
            fprintf(stderr, "Error fork\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)  //  child process
        {
            prepareExecChild(readingPipe, writingPipe);
        }
        else    //  parent
        {
            //  close reading end in the pipe the parent writes to
            if (close(writingPipe[0]) < 0)
            {
                fprintf(stderr, "Error closing unused pipe\n");
                exit(EXIT_FAILURE);
            }

            //  close writing end in the pipe the parent reads from
            if (close(readingPipe[1]) < 0)
            {
                fprintf(stderr, "Error closing unused pipe\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


//  get all the parent pipes and the input file
//  read each time 2 numbers from the file, and calculate their GCD using child process and pipes
//  read pipes are the pipes the parent reads from (child process write to)
//  write pipes are the pipes the parent writes to  (child process read from)
void parentProcessMainFunction(int* pipeR1, int* pipeR2, int* pipeW1, int* pipeW2, FILE* file)
{
    int ioCheck;
    int num1, num2, gcd;
    int countResults = 0;

    //  while not end of the input file
    while(!feof(file))
    {
        //  read 2 numbers from file
        ioCheck = readNumbersFromFile(file, &num1, &num2);
        countResults++;

        //  if input is illegal
        if (ioCheck == ILLEGAL_INPUT)
            printf("Illegal input at line %d\n", countResults);
        else
        {
            //  calculate the GCD using read and write pipes
            gcd = calcGCD(pipeW1, pipeR1, num1, num2);
            printf("%3d %3d gcd: %3d\n", num1, num2, gcd);
        }


        ioCheck = readNumbersFromFile(file, &num1, &num2);
        countResults++;

        //  if reached to end of file
        if (ioCheck == EOF)
            break;
        //  if input is illegal
        if (ioCheck == ILLEGAL_INPUT)
            printf("Illegal input at line %d\n", countResults);
        else
        {
            //  calculate the GCD using read and write pipes
            gcd = calcGCD(pipeW2, pipeR2, num1, num2);
            printf("%3d %3d gcd: %3d\n", num1, num2, gcd);
        }
    }
}


//  read 2 numbers from the file
int readNumbersFromFile(FILE* f, int* num1, int* num2)
{
    char temp[MAX_CHARS_IN_LINE];
    char line[MAX_CHARS_IN_LINE];
    if (fscanf(f, "%[^\n]\n", line) == EOF)  //  read entire line
        return EOF;
    if (sscanf(line, "%d %d %s\n", num1, num2, temp) != 2)
        return ILLEGAL_INPUT;
    return 1;
}


//  get read and write pipe, send 2 numbers to the child process and read the result
int calcGCD(int *pWpipe, int* pRpipe, int num1, int num2)
{
    int check, gcd;
    // write first number to the the first pipe
    check = write(pWpipe[1], &num1, sizeof(int));
    //  if failed to write
    if (check == -1)
    {
        fprintf(stderr, "Error writing to child process\n");
        exit(EXIT_FAILURE);
    }

    //  write second number to the first pipe
    check = write(pWpipe[1], &num2, sizeof(int));
    //  if faild to write
    if (check == -1)
    {
        fprintf(stderr, "Error writing to child process\n");
        exit(EXIT_FAILURE);
    }

    //  read the result from the pipe
    check = read(pRpipe[0], &gcd, sizeof(int));
    if (check == -1)
    {
        fprintf(stderr, "Error read from child process\n");
        exit(EXIT_FAILURE);
    }

    return gcd;
}

void prepareExecChild(int* readingPipe, int* writingPipe)
{
    //  if the pipe that the child reads from is not set to be STDIN
    if (writingPipe[0] != STDIN_FILENO)
    {
        if ((dup2(writingPipe[0], STDIN_FILENO) < 0) || (close(writingPipe[0]) < 0))
        {
            fprintf(stderr, "Error connect child pipe to stdin\n");
            exit(EXIT_FAILURE);
        }
    }

    //  if the pipe that the child writes to is not set to be STDOUT
    if (readingPipe[1] != STDOUT_FILENO)
    {
        if ((dup2(readingPipe[1], STDOUT_FILENO) < 0) || (close(readingPipe[1]) < 0))
        {
            fprintf(stderr, "Error connect child pipe to stdout\n");
            exit(EXIT_FAILURE);
        }
    }

    //  close the writing end in the pipe the child reads from
    if (close(writingPipe[1]) < 0)
    {
        fprintf(stderr, "Error closing child unused pipe\n");
        exit(EXIT_FAILURE);
    }

    //  close the reading end in the pipe the child writes to
    if (close(readingPipe[0]) < 0)
    {
        fprintf(stderr, "Error closing child unused pipe\n");
        exit(EXIT_FAILURE);
    }

    //  if execute child failed
    if (execlp(childFileName, childFileName, (char *)NULL) < 0)
    {
        fprintf(stderr, "Error executing child process\n");
        exit(EXIT_FAILURE);
    }
}