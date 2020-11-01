// Guy Kabiri 312252224

#ifndef __V2_PARENT__
#define __V2_PARENT__

int main(int, char**);
void createProcesses(int*, int*, int*, int*);
void prepareExecChild(int*, int*);
void parentProcessMainFunction(int*, int*, int*, int*, FILE*);
int readNumbersFromFile(FILE* f, int*, int*);
int calcGCD(int*, int*, int, int);

#endif