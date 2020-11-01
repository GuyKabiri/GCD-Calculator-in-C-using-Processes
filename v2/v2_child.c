// Guy Kabiri 312252224

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "v2_child.h"

int main(int argc, char* argv[])
{
    int a, b;
    int ioCheck = 0, result;
    while (! feof(stdin))
    {
        // read first number
        ioCheck = read(STDIN_FILENO, &a, sizeof(int));
        // if pipe is close
        if (ioCheck == 0)
        {
            break;
        }
        else if (ioCheck == -1)   //  error reading
        {
            exit(EXIT_FAILURE);
        }

        // read second number
        ioCheck = read(STDIN_FILENO, &b, sizeof(int));
        // if pipe is close
        if (ioCheck == 0)
        {
            break;
        }
        else if (ioCheck == -1)   //  error reading
        {
            exit(EXIT_FAILURE);
        }

        result = calcGCD(a, b);
        ioCheck = write(STDOUT_FILENO, &result, sizeof(int));
        if (ioCheck == -1)
        {
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int calcGCD(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
	return calcGCD(b, a % b);
}