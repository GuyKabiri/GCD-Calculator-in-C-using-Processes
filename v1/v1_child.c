#include <stdio.h>
#include <stdlib.h>

#include "v1_child.h"

int main(int argc, char* argv[])
{
	if (argc != 3)	//	if number of arguments is not 3 (too much numbers or not enough)
	{
		printf("not enough or too many arguments for GCD\n");
		exit(EXIT_FAILURE);
	}

	int a = atoi(argv[1]);	//	convert char into int
	int b = atoi(argv[2]);	//	convert char into int

	return calcGCD(abs(a), abs(b));		//	gcd(a, b) = gcd(-a, b) = gcd(a, -b) = gcd(-a, -b)
}


///////////////////////////////////////////////////////////////////////
//// Aim:       Calculate the GCD of 2 numbers
//// Input:     2 numbers to calculate the GCD for
//// Output:    The result of the GCD of the 2 numbers
///////////////////////////////////////////////////////////////////////
int calcGCD(int a, int b)	// Euclidean recursive algorithm
{
	if (b == 0)
		return a;
	
	return calcGCD(b, a % b);
}