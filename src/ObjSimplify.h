#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "vector3.h"

#ifndef OBJSIMPLIFY_H
#define OBJSIMPLIFY_H

class Face{
    public:
    int vertice[3]; //连接的点
    bool exist;
    Face(){
        vertice[0] = vertice[1] = vertice[2] = 0;
        exist = false;
    }
    Face(int a,int b,int c){
        vertice[0] = a;
        vertice[1] = b;
        vertice[2] = c;
        exist = true;
    }
    void del(){exist = false;}
};

class Vertex{
public:
    Vector3 pos;
    std::vector<int> link_vertices; //连接的点
    std::vector<int> link_faces;    //连接的面
    bool exist;
    Vertex(){
        pos = Vector3();
        exist = false;
    }
    Vertex(Vector3 p){
        pos = p;
        exist = true;
    }
    void addLink(int v){
        link_vertices.push_back(v);
    }
    void cleanRepeat(){
        std::sort(link_vertices.begin(),link_vertices.end());
        for(int i = 0;i < link_vertices.size();i++){
            link_vertices[i/2] = link_vertices[i];
        }
         int size = link_vertices.size();
        for(int i = 0;i < size/2;i++){
            link_vertices.pop_back();
        }
    }
    void addLinkFace(int f){
        link_faces.push_back(f);
    }
    void show(){
        printf("pos:");
        pos.show();
        printf("\nlink vertices:");
        for(int i = 0; i < link_vertices.size();i++){
            std::cout<<link_vertices[i]<<',';
        }
        printf("\n");
        printf("link faces:");
        for(int i = 0;i < link_faces.size();i++){
            std::cout<<link_faces[i]<<',';
        }
        printf("\n");
    }
    void del(){exist = false;}
};

class ObjSimplifier{

private:
    char* infile;
    char* outfile;
    double scale;
    int vertex_cnt;
    int face_cnt;

    std::vector<Vertex> vertices;
    std::vector<Face> faces;

public:
    ObjSimplifier(char* in, char* out,double s){
        infile = in;
        outfile = out;
        scale = s;
        printf("initing simplifier:\t<input>%s <output>%s <scale>%f\n",infile,outfile,scale);
        readObj();
        buildVertexLinks();
        writeObj();
    }

    void readObj(){
        std::ifstream in(infile);
        if (! in.is_open()){
            in.close();
            printf("Error Reading File. Abort\n");
        }
        int cnt = 0;
        printf("reading obj...\n");
        while (!in.eof()){
            cnt+=1;
            char buffer[256];
            in.getline (buffer,100);
            if (buffer[0] == 'v'){
                double a = 0,b = 0,c = 0;
                sscanf(buffer,"v %lf %lf %lf",&a,&b,&c);
                Vertex x = Vertex(Vector3(a,b,c));
                vertices.push_back(x);
            }else if(buffer[0] == 'f'){
                int a = 0,b = 0,c = 0;
                sscanf(buffer,"f %d %d %d",&a,&b,&c);
                Face x = Face(a - 1,b - 1,c - 1);
                faces.push_back(x);
            }else if(buffer[0] == '#'){
                sscanf(buffer,"# %d %d",&vertex_cnt,&face_cnt);
                printf("vertex:%d\tface:%d\n",vertex_cnt,face_cnt);
            }
        }
        in.close();
    }

    void buildVertexLinks(){
        printf("building links...\n");
        for(int i = 0; i < faces.size();i++){
            vertices[faces[i].vertice[0]].addLink(faces[i].vertice[1]);
            vertices[faces[i].vertice[0]].addLink(faces[i].vertice[2]);
            vertices[faces[i].vertice[0]].addLinkFace(i);
            vertices[faces[i].vertice[1]].addLink(faces[i].vertice[0]);
            vertices[faces[i].vertice[1]].addLink(faces[i].vertice[2]);
            vertices[faces[i].vertice[1]].addLinkFace(i);
            vertices[faces[i].vertice[2]].addLink(faces[i].vertice[0]);
            vertices[faces[i].vertice[2]].addLink(faces[i].vertice[1]);
            vertices[faces[i].vertice[2]].addLinkFace(i);
        }
        for(int i = 0;i < vertices.size();i++){
            vertices[i].cleanRepeat();
        }
    }

    void writeObj(){
        std::ofstream out(outfile);
        if (! out.is_open()){
            out.close();
            printf("Error Open Write File. Abort\n");
        }
        printf("Writing Object...\n");
        for(int i = 0;i < vertices.size();i++){
            out << "v " << vertices[i].pos.x << " " << vertices[i].pos.y << " " << vertices[i].pos.z << std::endl;
        }
        for(int i = 0;i < faces.size();i++){
            out << "f " << faces[i].vertice[0] + 1 << " " << faces[i].vertice[1] + 1 << " " << faces[i].vertice[2] + 1 << std::endl;
        }
        printf("Object write finished.\n");
    }
};
#endif