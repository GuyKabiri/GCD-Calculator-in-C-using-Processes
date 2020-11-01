// Guy Kabiri 312252224

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#include "v1_parent.h"

#define childProcess "./v1_child"
#define numOfArgvForGCD 4
#define MAX_CHARS_IN_LINE 250
#define ILLEGAL_INPUT -2

int main(int argc, char* argv[])
{
	while (1)	//	continue calculate GCD for pair fo numbers until inserted ctrl+d
	{
		int num1, num2;
		int check = readNumbersFromFile(&num1, &num2);	//	get 2 numbers from user
		if (check == EOF)									//	if ctrl+d inserted -> finish
			break;
		else if (check == 0)		//	error reading from stdin
		{
			printf("error reading input");
			exit(EXIT_FAILURE);
		}
		else if (check == ILLEGAL_INPUT)	//	inserted illegal input
		{
			printf("Illegal input\n");
			continue;
		}

		pid_t pid = fork();					//	create new process
		
		if (pid == -1)						//	error forking
		{
			printf("error forking\n");
			exit(EXIT_FAILURE);	
		}
		else if (pid == 0)	//	child process
		{
			if (executeGCD(num1, num2) == -1)	//	if executing gcd faild -> exit.
			{									// otherwise do nothing (gcd success and parent will handle result
					printf("error executing child\n");
					exit(EXIT_FAILURE);
			}
		}
		else	//	parent -> pid = child process id
		{
			int status;
			waitpid(pid, &status , 0);
			if (!WIFEXITED(status))
			{
				printf("error from child\n");
				exit(EXIT_FAILURE);
			}
			status = WEXITSTATUS(status);
			printf("%5d, %5d, GCD: %5d\n", num1, num2, status);
		}
	}
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
//// Aim:       Reading 2 numbers from the input file
//// Input:     f = FILE* variable to read from
////            num1 = integer pointer to read into the first numer
////            num2 = integer pointer to read into the second number
//// Output:    1 if succeeded, EOF if reached to end of file,
////            ILLEGAL_INPUT if more then 2 numbers in a row or characters
///////////////////////////////////////////////////////////////////////
int readNumbersFromFile(int* num1, int* num2)
{
    char temp[MAX_CHARS_IN_LINE];
    char line[MAX_CHARS_IN_LINE];
	char* check=fgets(line, MAX_CHARS_IN_LINE-1, stdin);

    if (check == NULL)  //  read entire line
	{
		if(feof(stdin))
        	return EOF;
		else
			return 0;
	}
    if (sscanf(line, "%d %d %s\n", num1, num2, temp) != 2)
        return ILLEGAL_INPUT;
    return 1;
}

///////////////////////////////////////////////////////////////////////
//// Aim:       Get a number and insert it into a char array
//// Input:     numToCOnvert = the number to insert into the char array
////			buff = char array to insert the number into
//// Output:    -1 if there is error executing
///////////////////////////////////////////////////////////////////////
int convertIntToChar(int numToConvert, char* buff)
{
	return sprintf(buff, "%d", numToConvert);
}

///////////////////////////////////////////////////////////////////////
//// Aim:       Prepare the arguments array to pass to the child process and execute it
//// Input:     2 numbers to calculate the GCD for
//// Output:    -1 if there is error executing
///////////////////////////////////////////////////////////////////////
int executeGCD(int num1, int num2)
{
	char arg1[sizeof(int)], arg2[sizeof(int)];
	if(convertIntToChar(num1, arg1) == -1)	//	convert first number into char array
		return -1;
	if(convertIntToChar(num2, arg2) == -1)	//	convert second number into char array
		return -1;
	
	char* child_argv[numOfArgvForGCD];
	child_argv[0] = childProcess;
	child_argv[1] = arg1;
	child_argv[2] = arg2;
	child_argv[3] = NULL;
	
	return execv(childProcess, child_argv);	//	execute child
}

