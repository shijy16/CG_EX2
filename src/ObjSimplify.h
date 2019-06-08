#include <stdlib.h>
#include <stdio.h>

#ifndef OBJSIMPLIFY_H
#define OBJSIMPLIFY_H

class ObjSimplifier{
private:
char* infile;
char* outfile;
double scale;
public:
    ObjSimplifier(char* in, char* out,double s){
        infile = in;
        outfile = out;
        scale = s;
        printf("initing simplifier:\t<input>%s <output>%s <scale>%f\n",infile,outfile,scale);
    }
};
#endif