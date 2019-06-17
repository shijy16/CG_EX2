#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "vector3.h"
#include <omp.h>

#ifndef OBJSIMPLIFY_H
#define OBJSIMPLIFY_H

double t = 10;

class MatrixK{
public:
    double m[4][4];
    MatrixK(double a,double b,double c,double d){
        double q[4] = {a,b,c,d};
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                m[i][j] = q[i]*q[j];
            }
        }
    }
    MatrixK(){
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                m[i][j] = 0;
            }
        }
    }

    void add(MatrixK a){
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
               m[i][j] = a.m[i][j] + m[i][j];
            }
        }
    }


    static std::pair<Vector3,double> getBestV(MatrixK a,MatrixK b,Vector3 pos1,Vector3 pos2){
        std::pair<Vector3,double> res;
        double Q[4][4];
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
               Q[i][j] = a.m[i][j] + b.m[i][j];
            }
        }
        // Q[3][0] = 0;
        // Q[3][1] = 0;
        // Q[3][2] = 0;
        // Q[3][3] = 1;
        //计算最佳位置
        Vector3 bestV = Vector3();
        double det = Q[0][0]*Q[1][1]*Q[2][2] + Q[0][1]*Q[1][2]*Q[2][0] 
                + Q[0][2]*Q[1][0]*Q[2][1] - Q[0][0]*Q[1][2]*Q[2][1] 
                - Q[0][1]*Q[1][0]*Q[2][2] - Q[0][2]*Q[1][1]*Q[2][0];
        if(det == 0){
           bestV = (pos1 + pos2)/2.0f;
        }else{
            bestV.x = Q[0][3]*Q[1][2]*Q[2][1] + Q[0][2]*Q[1][1]*Q[2][3] 
                    + Q[0][1]*Q[1][3]*Q[2][2] - Q[0][1]*Q[1][2]*Q[2][3] 
                    - Q[0][2]*Q[1][3]*Q[2][1] - Q[0][3]*Q[1][1]*Q[2][2];
            bestV.x /= det;
            bestV.y = Q[0][0]*Q[1][2]*Q[2][3] + Q[0][2]*Q[1][3]*Q[2][0] 
                    + Q[0][3]*Q[1][0]*Q[2][2] - Q[0][3]*Q[1][2]*Q[2][0] 
                    - Q[0][2]*Q[1][0]*Q[2][3] - Q[0][0]*Q[1][3]*Q[2][2];
            bestV.y /= det;
            bestV.z = Q[0][3]*Q[1][1]*Q[2][0] + Q[0][1]*Q[1][0]*Q[2][3] 
                    + Q[0][0]*Q[1][3]*Q[2][1] - Q[0][0]*Q[1][1]*Q[2][3] 
                    - Q[0][1]*Q[1][3]*Q[2][0] - Q[0][3]*Q[1][0]*Q[2][1];
            bestV.z /= det;
        }
        res.first = bestV;
        //计算cost
        double cost = 0.0;
        double q1,q2,q3,q4;
        q1 = bestV.x*Q[0][0] + bestV.y*Q[1][0] + bestV.z*Q[2][0] + Q[3][0];
        q2 = bestV.x*Q[0][1] + bestV.y*Q[1][1] + bestV.z*Q[2][1] + Q[3][1];
        q3 = bestV.x*Q[0][2] + bestV.y*Q[1][2] + bestV.z*Q[2][2] + Q[3][2];
        q4 = bestV.x*Q[0][3] + bestV.y*Q[1][3] + bestV.z*Q[2][3] + Q[3][3];
        cost = q1*bestV.x + q2*bestV.y + q3*bestV.z + q4;
        res.second = cost;
        return res;
    }

    void show(){
        for(int i = 0 ; i < 4 ; i ++){
            for(int j = 0 ; j < 4 ; j ++){
                std::cout << m[i][j] << " " ;
            }
            std::cout << std::endl ;
        }
    }
};


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
    MatrixK Qv;
    bool exist;
    Vertex(){
        pos = Vector3();
        exist = false;
        Qv = MatrixK();
    }
    Vertex(Vector3 p){
        pos = p;
        exist = true;
        Qv = MatrixK();
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

    void setQv(MatrixK m){
        Qv = m;
    }

    void del(){exist = false;}
};

class Pair{
public:
    int x,y;
    double cost;
    Vector3 bestV;
    bool exist;
    Pair(int a,int b){
        exist = true;
        x = a;
        y = b;
        bestV = Vector3();
    }
    void setCost(double c){
        cost = c;
    }
    void setBestV(Vector3 p){
        bestV = p;
    }
    void del(){
        exist = false;
    }
};

class ComparePair{
public:
    bool operator()(Pair a, Pair b)
    {
        return a.cost > b.cost;
    }
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
    std::vector<Pair> pairs;
    ComparePair pair_cmp = ComparePair();
public:
    ObjSimplifier(char* in, char* out,double s){
        infile = in;
        outfile = out;
        scale = s;
        printf("initing simplifier:\t<input>%s <output>%s <scale>%f\n",infile,outfile,scale);
    }

    void run(){
        readObj();
        buildVertexLinks();
        getQvs();
        getPairs();
        getCostAndBestPos();
        std::make_heap(pairs.begin(),pairs.end(),pair_cmp);
        printf("running... pairs cnt:%d\n",pairs.size());

        int goal = scale*face_cnt;
        int index = face_cnt;
		std::vector<int> change;
		for (int i = 0; i < vertices.size(); i++) {
			change.push_back(i);
		}
		// printf("%d,%d\n", index,pairs.size());
        while(index > goal && pairs.size() > 0){
            Pair lowCostPair = popPairHeap();
            
            int v1 = lowCostPair.x;
            int v2 = lowCostPair.y;
            Vector3 v = lowCostPair.bestV;
            // printf("%d,%d,%d\n",index,v1,v2);
            
            vertices[v1].del();
            vertices[v2].del();
			vertices.push_back(Vertex(v));
			change[v1] = vertices.size() - 1;
			change[v2] = vertices.size() - 1;
			change.push_back(vertices.size() - 1);
            //删去旧面，加入新的面
            for(int i = 0;i < vertices[v1].link_faces.size();i++){
                if(!faces[vertices[v1].link_faces[i]].exist) continue;
                Face temp = faces[vertices[v1].link_faces[i]];
                //若面中两个点都有，删去
				bool flag = true;
                for(int j = 0;j < 3;j++){
                    if(temp.vertice[j] == v2){
                        faces[vertices[v1].link_faces[i]].del();
                        index--;
						flag = false;
                    }
					// 替换其中v1为v并将面连接到v
					if (temp.vertice[j] == v1) {
						faces[vertices[v1].link_faces[i]].vertice[j] = vertices.size() - 1;
					}
                }
				if(flag)
					vertices[vertices.size() - 1].addLinkFace(vertices[v1].link_faces[i]);
            }
            //  printf("%f\n",lowCostPair.cost);
            for(int i = 0;i < vertices[v2].link_faces.size();i++){
                if(!faces[vertices[v2].link_faces[i]].exist) continue;
                Face temp = faces[vertices[v2].link_faces[i]];
                //替换面中的v2，v1时已经删去应该删去的面
                for(int j = 0;j < 3;j++){
                    if(temp.vertice[j] == v2){    
                        faces[vertices[v2].link_faces[i]].vertice[j] = vertices.size() - 1;
						break;
                    }
                }
				vertices[vertices.size() - 1].addLinkFace(vertices[v2].link_faces[i]);
            }

             //新点的误差矩阵
            vertices[vertices.size() - 1].setQv(getQv(vertices.size() - 1));

            //处理pair
            for(int i = 0;i < pairs.size();i++){
                if(change[pairs[0].x] == change[pairs[0].y]){
                     popPairHeap();
                     continue;
                }
                if(!vertices[pairs[0].x].exist || !vertices[pairs[0].y].exist){
					Pair p = popPairHeap();
					int newV1 = change[p.x];
                    int last = p.x;
                    while(newV1 != last){
                        last = newV1;
                        newV1 = change[newV1];
                    }
                    last = p.y;
					int newV2 = change[p.y];
                    while(newV2 != last){
                        last = newV2;
                        newV2 = change[newV2];
                    }
					if (newV1 == newV2) continue;
                    double tt = (vertices[newV1].pos - vertices[newV2].pos).getLength();
                    if(tt < t){
                        Pair newPair = Pair(newV1,newV2);
                        //新的cost
                        std::pair<Vector3,double> temp = MatrixK::getBestV(vertices[newPair.x].Qv,vertices[newPair.y].Qv,vertices[newPair.x].pos,vertices[newPair.y].pos);
                        newPair.setBestV(temp.first);
                        newPair.setCost(temp.second);
                        insertPairHeap(newPair);
                    }
                }else{
                    break;
                }
            }
        }


		printf("Writing Object... faces:%d\n",index);
		std::vector<Vector3> temp_vertices;
		std::vector<int> temp_v_point;
		int numv = 0;
		int numt = 0;
		for (int i = 0; i < vertices.size(); i++){
			temp_v_point.push_back(i);
		}
		for (int i = 0; i < vertices.size(); i++){
			if (vertices[i].exist){
				temp_vertices.push_back(vertices[i].pos);
				temp_v_point[i] = temp_vertices.size() - 1;
				++numv;
			}
		}

		for (int i = 0; i < faces.size(); i++){
			if (faces[i].exist){
				for (int j = 0; j < 3; j++){
					int vinv = temp_v_point[faces[i].vertice[j]];
					faces[i].vertice[j] = vinv;
				}
				++numt;
			}
		}

		std::ofstream out(outfile);
		if (!out.is_open()) {
			out.close();
			printf("Error Open Write File. Abort\n");
		}
		for (int i = 0; i < temp_vertices.size(); i++) {
			out << "v " << temp_vertices[i].x << " " << temp_vertices[i].y << " " << temp_vertices[i].z << std::endl;
		}
		for (int i = 0; i < faces.size(); i++) {
			if (faces[i].exist)
				out << "f " << faces[i].vertice[0] + 1 << " " << faces[i].vertice[1] + 1 << " " << faces[i].vertice[2] + 1 << std::endl;
		}
		printf("Object write finished.\n");


  //      printf("%d\n",index);
    }


    void insertPairHeap(Pair p){
        pairs.push_back(p);
        std::push_heap(pairs.begin(),pairs.end(),pair_cmp);
    }

    Pair popPairHeap(){
        std::pop_heap(pairs.begin(),pairs.end(),pair_cmp);
        Pair res = pairs.back();
        pairs.pop_back();
        return res;
    }

    void getCostAndBestPos(){
        printf("getting bestV and cost of pairs...\n");
        for(int i = 0;i < pairs.size();i++){
            std::pair<Vector3,double> temp = MatrixK::getBestV(vertices[pairs[i].x].Qv,vertices[pairs[i].y].Qv,vertices[pairs[i].x].pos,vertices[pairs[i].y].pos);
            // if(i == 0){
            //     printf("%d,%d\n",pairs[i].x,pairs[i].y);
            //     vertices[pairs[i].x].Qv.show();
            //     vertices[pairs[i].y].Qv.show();
            // }
            pairs[i].setBestV(temp.first);
            pairs[i].setCost(temp.second);
        }
    }

    void getPairs(){
        printf("getting Pairs with t=%f...\n",t);
        for(int i = 0;i < vertices.size();i++){
            for(int j = 0;j < vertices[i].link_vertices.size();j++){
                if(vertices[i].link_vertices[j] > i){
                    double temp = (vertices[i].pos - vertices[vertices[i].link_vertices[j]].pos).getLength();
                    if(temp < t){
                        pairs.push_back(Pair(i,vertices[i].link_vertices[j]));
                    }
                }
            }
        }
    }

    inline MatrixK getQv(int id){
        MatrixK ans = MatrixK();
        for(int i = 0;i < vertices[id].link_faces.size();i++){
            //计算法向量
            Vector3 v_t1 = vertices[faces[vertices[id].link_faces[i]].vertice[0]].pos - vertices[faces[vertices[id].link_faces[i]].vertice[1]].pos;
            Vector3 v_t2 = vertices[faces[vertices[id].link_faces[i]].vertice[0]].pos - vertices[faces[vertices[id].link_faces[i]].vertice[2]].pos;
            Vector3 N = Vector3::cross(v_t1,v_t2);
            N.normalize();
            double d = Vector3::dot(N,vertices[id].pos);
            d = -d;
            MatrixK t = MatrixK(N.x,N.y,N.z,d);
            ans.add(t);
        }
        return ans;
    }

    void getQvs(){
        printf("calculating Qv...\n");
        // #pragma omp parallel for
        for(int i = 0;i < vertices.size();i++){
            vertices[i].setQv(getQv(i));
        }
    }


    void readObj(){
        std::ifstream in(infile);
        if (! in.is_open()){
            in.close();
            printf("Error Reading File. Abort\n");
			return;
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
				vertex_cnt++;
            }else if(buffer[0] == 'f'){
                int a = 0,b = 0,c = 0;
                sscanf(buffer,"f %d %d %d",&a,&b,&c);
                Face x = Face(a - 1,b - 1,c - 1);
                faces.push_back(x);
				face_cnt++;
            }
        }
		printf("vertex:%d\tface:%d\n", vertex_cnt, face_cnt);
        in.close();
    }

    void buildVertexLinks(){
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
 /*       std::ofstream out(outfile);
        if (! out.is_open()){
            out.close();
            printf("Error Open Write File. Abort\n");
        }
        printf("Writing Object...\n");
        for(int i = 0;i < vertices.size();i++){
            out << "v " << vertices[i].pos.x << " " << vertices[i].pos.y << " " << vertices[i].pos.z << std::endl;
        }
        for(int i = 0;i < faces.size();i++){
            if(faces[i].exist)
                out << "f " << faces[i].vertice[0] + 1 << " " << faces[i].vertice[1] + 1 << " " << faces[i].vertice[2] + 1 << std::endl;
        }
        printf("Object write finished.\n");*/
    }
};
#endif