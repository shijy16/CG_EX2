#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ObjSimplify.h"

int main(int argc, char** argv){
    time_t start, stop;
	start = time(NULL);
    if(argc < 4){
        printf("main.exe <in obj file name> <out obj file name> <currency>\n");
        return -1;
    }
    ObjSimplifier* simplifier;
    simplifier = new ObjSimplifier(argv[1],argv[2],atof(argv[3]));
    simplifier->run();
    stop = time(NULL);
	printf("Used Time:%ld\n", (stop - start));
}