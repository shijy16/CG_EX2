#include <stdlib.h>
#include <stdio.h>
#include "ObjSimplify.h"

int main(int argc, char** argv){
    if(argc < 4){
        printf("main.exe <in obj file name> <out obj file name> <currency>\n");
        return -1;
    }
    ObjSimplifier* simplifier;
    simplifier = new ObjSimplifier(argv[1],argv[2],atof(argv[3]));

}