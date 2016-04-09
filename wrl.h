#ifndef WRL_H
#define WRL_H

#include "Vector.h"
#include "Neuron.h"

class Point;
class Triangle;
#define WRL_MAX_POINT 20000
#define WRL_MAX_TRIANGLE 40000

class Wrl{
	double point[WRL_MAX_POINT][3];
	//0,1,2: give 3 vertices, 4 is the opposite vertices
	int triangle[WRL_MAX_TRIANGLE][4];
	Vector* vec[WRL_MAX_TRIANGLE];
	void add(int tria,Neuron * n,int number,int step,double alpha,double beta,double gamma);
	Neuron * getNeuron();
	int getNrNeuron();
	int random();
	double random01();
	int maxPoint,maxTriangle;
public:
	Point* p;
	Triangle* t;
	Wrl(char* file);
	void printWRL();
	void printIV();
	void printDXF();
	
}
;

class Vec{
	double ox,oy,oz,xx,yy,zz;
public:
	Vec(double x,double y,double z){ox=0;oy=0;oz=0;xx=x;yy=y;zz=z;};
	Vec(double x,double y,double z,double x1,double y1,double z1){
		ox=x;oy=y;oz=z;xx=x1;yy=y1;zz=z1;
	};
	double getOX(){return ox;}
	double getOY(){return oy;}
	double getOZ(){return oz;}
	double getX(){return xx;}
	double getY(){return yy;}
	double getZ(){return zz;}
	Vec* crossProduct(Vec * b){
		
	}


}
;

#endif
