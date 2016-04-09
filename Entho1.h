#ifndef ENTHO1_H
#define ENTHO1_H

#include "Vector.h"
#include "Neuron.h"

class Point;
class Triangle;
#define ENTHO_MAX_POINT 40000
#define WRL_MAX_TRIANGLE 40000
#define MAX 40000
#define MAX_DIST 100

class Entho1{
	int algo;
	char *name;
	
	double point[MAX][4], lineNr[MAX],endP[MAX],startP[MAX],remove[MAX],type[MAX];
	int added[MAX];
	int ident[MAX];
	
	int count, join[MAX][3];
	Neuron * n;
public:
	//input file, x,y,z of the soma
	Entho1( char* fileName , int algorithm);
	Entho1(char* file, int a=1, double x=0, double y=0, double z=0);
	void EnthoOld(char* file);
	int addLine(int id, int type, int index, int pid);
	int readFile(char *fileName);
	int joinSeg(int i1, int i2);
	int meltSeg(int i1, int i2);
	void joining(int lineType);
	void joining1(int lineType);
	void joining2(int lineType);
	void joining3(int lineType);
	void joining4(int lineType);
	void joining5(int lineType);
	//print original structure
	int printDXFOrg();
	//print joined struct
	int printDXF();
	void createNeuron(double x=0, double y=0, double z=0);
	void printLine(double type, double x,double y,double z,double x1,double y1,double z1);
	double distance(int h, int pt){
		double x,y,z,x1,y1,z1;
						x=point[h][0];
						y=point[h][1];
						z=point[h][2];
						
						x1=point[pt][0];
						y1=point[pt][1];
						z1=point[pt][2];
						//if on the same plane add 1/3 of plane distance
						if(z==z1)
							z+=0.065*.33;
		return sqrt((x1-x)*(x1-x)+ (y1-y)*(y1-y)+ (z1-z)*(z1-z));
	}
	
}
;


#endif
