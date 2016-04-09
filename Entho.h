#ifndef ENTHO_H
#define ENTHO_H

#include "Vector.h"
#include "Neuron.h"

class Point;
class Triangle;
#define ENTHO_MAX_POINT 20000
#define WRL_MAX_TRIANGLE 35000
#define MAX 15000
//max distance to join two close point
#define MAX_DISTANCE 10.5

class Entho: public  Neuron{
	//x,y,z, type,id, line numebr, end point
	double point[MAX][4], connect[MAX],lineNr[MAX],endP[MAX],startP[MAX],remove[MAX];
	
	int count, join[MAX][2];
	//store the info as a series of segments:list of segments to be joined
	Segment * seg[MAX];
	//list of end segment to which joins can be possible!
	Segment * available[MAX];
	int segIndex;

public:
	Entho(char* file);
	int readFile(char *fileName);
	void joining(int line);
	double distance(int h, int pt){
		double x,y,z,x1,y1,z1;
						x=point[h][0];
						y=point[h][1];
						z=point[h][2];
						
						x1=point[pt][0];
						y1=point[pt][1];
						z1=point[pt][2];
		return sqrt((x1-x)*(x1-x)+ (y1-y)*(y1-y)+ (z1-z)*(z1-z));
	}
	double distance(Segment * t1, Segment *t2){
		double x,y,z,x1,y1,z1;
						x=t1->getEnd()->getX();
						y=t1->getEnd()->getY();
						z=t1->getEnd()->getZ();
						
						x1=t2->getEnd()->getX();
						y1=t2->getEnd()->getY();
						z1=t2->getEnd()->getZ();
		return sqrt((x1-x)*(x1-x)+ (y1-y)*(y1-y)+ (z1-z)*(z1-z));
	}
	
}
;


#endif
