                                 // Chapter 11 - Program 1 - PERSON.H
#ifndef POINT_H
#define POINT_H


class Point {
protected:    // Make these variables available to the subclasses

   double x;

   double y;

   double z;
public:

	Point();
	//Removing Point:: due to gcc 4.1 issue
	Point(Point* a);
	Point(double x, double y, double z);



	double distance(Point* b);

	void setX(double v){x=v;}

	void setY(double v){y=v;}

	void setZ(double v){z=v;}

	double getX(){return x;}

	double getY(){return y;}

	double getZ(){return z;}	

};


#endif
