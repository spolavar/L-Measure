
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <sstream>
#include <stdio.h>
#include "wrl.h"
#include "Neuron.h"
#include "Func.h"
#define STEP 1000
using namespace std;
Wrl::Wrl(char *fileName){
	Vector *A,*B,*D;
	A=new Vector();
	B=new Vector();
	D=new Vector();
	double area,areaTot=0;
	// 0:WRL    1:IV    2:DXF
	int output=2;
	double NrNeuron=1000000;
	int start=0;
	int jump=1;
	//number of neuron involved
	int numero=50;
	//golgi percentage
	double golgi=numero/NrNeuron;
	numero*=1.2;

	srand(1212);
			

	double max=0;
	
	char* NeuronName="n221.swc";
	//store center of mass
	double cx=0,cy=0,cz=0;
	//store min & max value of points
	double mx,my,mz,Mx,My,Mz;

	Vector * centerMass;
	maxPoint=0;maxTriangle=0;
	ifstream* in=new ifstream(fileName, ios::in | ios::binary );

	if( in == NULL )
          {
            //problem found
            cerr<<"Unable to find:"<<fileName<<" !\n----------------------------------\n";
            return;
          }

	//looks for
	int found=0;
	char k[100],c;
	double x,y,z;
	while(found==0 && in->peek()!=-1){
		*in>>k;
		//found end tree: reset variable
		if(strcmp(k,"point")==0)
			found=1;
	}
	if(found==0){
		cerr<<"Problem reading file:"<<fileName<<"\n";
	}

	cerr<<"\nRead Point:";
		//remove everythink till the end of the line
		while (in->peek()!=13 ){ 
			char c=in->get();
			
		}
		//remove all returns
		while(in->peek()==13)
			if(in->peek()==13){
				in->get();
				if(in->peek()==10)
					in->get();
			}
		//remove beginning space
		while (in->peek()==32 ){ 
			in->get();
		}
		//stop when c find a ]
		while(in->peek()!=-1 && c!=']'){

			*in>>x;	
			//remove coma if present
			while (in->peek()==44 )	
				char c=in->get();
			*in>>y;
			//remove coma if present
			while (in->peek()==44 )	
				char c=in->get();
			*in>>z;
			
			*in>>c;
			
			point[maxPoint][0]=x;
			point[maxPoint][1]=y;
			point[maxPoint][2]=z;
			cx+=x;
			cy+=y;
			cz+=z;
			if(maxPoint==0) {
				//first point
				mx=x;Mx=x;my=y;My=y;mz=z;Mz=z;
			}
			maxPoint++;
			if(maxTriangle>WRL_MAX_POINT){
				cerr<<"Overflow array WRL_POINT!\n";
				exit(-1);
			}
			//set external box
			if(mx>x) mx=x;
			if(Mx<x) Mx=x;
			if(my>y) my=y;
			if(My<y) My=y;
			if(mz>z) mz=z;
			if(Mz<z) Mz=z;
		}
	

		//compute center of mass
		if(maxPoint>0){
			cx=cx/maxPoint;
			cy=cy/maxPoint;
			cz=cz/maxPoint;
			centerMass=new Vector(cx,cy,cz);
		}
		//look for triangle data
		cerr<<"\nRead Triangle:";
		found=0;
	while(found==0 && in->peek()!=-1){
		*in>>k;
		//found end tree: reset variable
		if(strcmp(k,"coordIndex")==0)
			found=1;
	}
	if(found==0){
		cerr<<"Problem reading file:"<<fileName<<"\n";
	}
		//remove everythink till the end of the line
		while (in->peek()!=13 ){ 
			char c=in->get();
			
		}
		//remove all returns
		while(in->peek()==13)
			if(in->peek()==13){
				in->get();
				if(in->peek()==10)
					in->get();
			}
		//remove beginning space
		while (in->peek()==32 ){ 
			in->get();
		}

		int a,b,d,neg;
	
		while(in->peek()!=-1){

			*in>>a;	
			if(in->peek()==-1)
				break;
			//remove coma if present
			while (in->peek()==44 )	
				char c=in->get();
		
			*in>>b;
			while (in->peek()==44 )	
				char c=in->get();
			*in>>d;

			while (in->peek()==44 )	
				char c=in->get();
			*in>>neg;
			//add Triangle with vertex a,b,d
			triangle[maxTriangle][0]=a;
			triangle[maxTriangle][1]=b;
			triangle[maxTriangle][2]=d;
			triangle[maxTriangle][3]=-1;



			// compute perpendicular
					A->setX(point[a][0]);
					A->setY(point[a][1]);
					A->setZ(point[a][2]);
					B->setX(point[b][0]);
					B->setY(point[b][1]);
					B->setZ(point[b][2]);
					D->setX(point[d][0]);
					D->setY(point[d][1]);
					D->setZ(point[d][2]);

			//computing areas
					area=A->angle(B,D);
					area=fabs(sin(area));
					area=0.5*area*A->distance(B)*A->distance(D);
					areaTot+=area;

			//for each triangle look for perpendicular
			vec[maxTriangle]=(new Vector());
			vec[maxTriangle]->crossProduct(A,B,D);
			
			maxTriangle++;
			if(maxTriangle>WRL_MAX_TRIANGLE){
				cerr<<"Overflow array WRL_TRIANGLE!\n";
				exit(-1);
			}
			if(maxTriangle%STEP==0)
				cerr<<"+";
		}

	cerr<<maxTriangle<<"\nArea:"<<areaTot<<"\n";	
		
		Neuron * n;



		cerr<<t<<"\t"<<maxPoint<<"\t"<<maxTriangle<<"\n";
		switch(output){
		case 0:
			printWRL();
			break;
		case 1:
			printIV();
			break;
		case 2:
			//print header of DXF file
			cout<<"999\nL-Measure\n999\n3d faces List\n0\nSECTION\n2\nENTITIES\n0\n";
			printDXF();
			;
		}

			Vector* Perp=new Vector();

			int t;
			int countNeuron=0;
			//origin
			Vector* Trans;
			Trans=new Vector();
		
			int nrNeuronTriangle=0;

			for(int l=0;l<maxTriangle;l++){
				
				t=start+l;
					if(t>maxTriangle)
						break;
					if(countNeuron>=numero)
						break;
					//get the point
					a=triangle[t][0];
					b=triangle[t][1];
					d=triangle[t][2];
					//get coord
					x=(point[a][0]+point[b][0]+point[d][0])/3;
					y=(point[a][1]+point[b][1]+point[d][1])/3;
					z=(point[a][2]+point[b][2]+point[d][2])/3;
					Trans->set(x,y,z);
					A->set(point[a][0],point[a][1],point[a][2]);
					B->set(point[b][0],point[b][1],point[b][2]);
					D->set(point[d][0],point[d][1],point[d][2]);
					
					//computing areas
					area=A->angle(B,D);
					area=fabs(sin(area));
					area=0.5*area*A->distance(B)*A->distance(D);
					//how many neurons for this triangle
					double nr=((area/areaTot*NrNeuron)*golgi);
					//if golgi applyed nr can be <1, if so then generate a random number
					if(nr<1){
						if(nr>random01())
							nr=1;
					}
					nrNeuronTriangle=(int)nr;
					//skip actual triangle if no neuron is to be placed
					if(nrNeuronTriangle==0) continue;
					


					
					Perp->crossProduct(A,B,D);
					
				if(t%(jump)==0){
					//end when printed the right amount of neuron

						if(countNeuron%100==0){
							cerr<<"+";
						}
						if(countNeuron%1000==0){
							cerr<<countNeuron;
						}
					for(int h=0;h<nrNeuronTriangle;h++){
						if(countNeuron>=numero)
							break;
						//generate a random point within the triangle
						double px,py,pz,tot;
						px=random();
						py=random();
						pz=random();
						tot=px+py+pz;
						px/=tot;py/=tot;pz/=tot;
						//create the point
						x=(px*point[a][0]+py*point[b][0]+pz*point[d][0]);
						y=(px*point[a][1]+py*point[b][1]+pz*point[d][1]);
						z=(px*point[a][2]+py*point[b][2]+pz*point[d][2]);
						Trans->set(x,y,z);
					
						//generate neuron 
						n=getNeuron();
						//orient and place it
						n->orientTrans1(Trans ,Perp,centerMass);
						

						switch(output){
						case 0:
							n->printFatherLineWRL();
							
							break;
						case 1:
							n->printFatherIV();
							break;
						case 2:
							
							n->printDXF();
							break;
						}
						
						delete (n);
						
						countNeuron++;
					}
					
				}

			}
			cerr<<"AreaTot :"<<areaTot<<"  #:"<<countNeuron<<"\n";	
			
			//print tail of file
		switch(output){
		case 0:
			cout<<"\n#";
			break;
		case 1:
			cout<<"\n#";
			break;
		case 2:
			//print tail of DXF file
			cout<<"ENDSEC\n0\nEOF";
			cout<<"\n#";
			;
		}

			
		
}
;

int Wrl::getNrNeuron(){
	
	return (int)(((double) random ()) / (RAND_MAX+1)*56.0);

};
int Wrl::random(){
	return rand();
};
double Wrl::random01(){
	return (rand()/ (RAND_MAX+1.0));
}

Neuron * Wrl::getNeuron(){
	static char name[20];//="                 ";
	static char num[20];//="                 ";
	
	static int NR=56;
	static int initialize=0;
	
	static Neuron *list[56];
	if(initialize==0){
		initialize=1;
		for(int i=0;i<NR;i++){
			if(i==17){
				int hh=0;
			}
			strcpy(name,"");
			strcat(name,"H:\\research\\lmeasure\\gc2\\");
			itoa1(i,num,10);
			strcat(name,num);
			strcat(name,".swc");
			list[i]=new Neuron(name);

		}

	}
	//choose which neuron
	int neuron=getNrNeuron();
	
	return list[neuron]->clone();
	
};

void Wrl::printWRL(){
			
			cout<<"#VRML V2.0 utf8\n";
			cout<<"\n";
			cout<<"Viewpoint {\n";
			cout<<"  position 0 0 0\n";
			cout<<"  orientation 0 0 0 0\n";
			cout<<"  fieldOfView 0.6\n";
			cout<<"  description \"Initial ViewPoint\"\n";
			cout<<"}\n";
			cout<<"NavigationInfo {\n";
			cout<<"  type \"EXAMINE\"\n";
			cout<<"}\n";
			cout<<"Shape {\n";
			cout<<"  appearance DEF A Appearance {material Material {diffuseColor 1 0.4 0.4} }\n";
			cout<<"  geometry DEF IFS IndexedFaceSet { \n";
			cout<<"coord    Coordinate    { point [ \n";
		//print point
			cout<<point[0][0]<<" "<<point[0][1]<<" "<<point[0][2];
			for(int p=1;p<maxPoint;p++){
				cout<<"  ,\n"<<point[p][0]<<" "<<point[p][1]<<" "<<point[p][2];
			}
			
			cout<<"\n]\n}\n";
			cout<<"\n";
			cout<<"coordIndex [\n";
		
			
			for(int h=0;h<maxTriangle;h++){
				
				{
					cout<<triangle[h][0]<<" "<<triangle[h][1]<<" "<<triangle[h][2]<<" -1\n";
					
				}

				
				// draw perpendicular as line
				if(triangle[h][3]!=-1){
					cout<<triangle[h][0]<<" "<<triangle[h][1]<<" "<<triangle[h][3]<<" -1\n";
				}else{
					int ggg=h;
				}

			}

			
			cout<<"		     ]\n";
			cout<<"  }\n";
			cout<<"}\n";
			
			


};

void Wrl::printIV(){
	cout<<"#Inventor V2.0 ascii\n";
				cout<<"DEF object Separator {\n";
    	cout<<"Coordinate3 {\n";
      	cout<<"point  [\n";

		
			cout<<point[0][0]<<" "<<point[0][1]<<" "<<point[0][2];
			for(int p=1;p<maxPoint;p++){
				cout<<"  ,\n"<<point[p][0]<<" "<<point[p][1]<<" "<<point[p][2];
			}
			
    cout<<"]}\n";
	cout<<"IndexedFaceSet {\n";
    cout<<"coordIndex [\n";
		
			
			for(int h=0;h<maxTriangle;h++){
				
				
					cout<<triangle[h][0]<<","<<triangle[h][1]<<","<<triangle[h][2]<<",-1,\n";
					cout<<triangle[h][2]<<","<<triangle[h][1]<<","<<triangle[h][0]<<",-1";
					if(h<maxTriangle){
						cout<<",\n";
					} else{
						cout<<"\n";
						
					}
				
			}

			// draw perpendicular as line
cout<<"    ]\n";
cout<<"  }\n";
cout<<"}\n";


};


void Wrl::printDXF(){
		char* color="62\n600\n";;
		
		for(int h=0;h<maxTriangle;h++){
			double t=-5;
			if( point[triangle[h][0]][2]<t || 	
				point[triangle[h][1]][2]<t || 	
				point[triangle[h][2]][2]<t  ){
				int hh=0;
			}
			//print rectangles
			cout<<"3DFACE\n8\n2\n";
			cout<<color;
			//X1
			cout<<"10\n"<<point[triangle[h][0]][0]<<"\n";
			//Y1
			cout<<"20\n"<<point[triangle[h][0]][1]<<"\n";
			//Z1
			cout<<"30\n"<<point[triangle[h][0]][2]<<"\n";

			//X2
			cout<<"11\n"<<point[triangle[h][1]][0]<<"\n";
			//Y2
			cout<<"21\n"<<point[triangle[h][1]][1]<<"\n";
			//Z2
			cout<<"31\n"<<point[triangle[h][1]][2]<<"\n";

			//X3
			cout<<"12\n"<<point[triangle[h][2]][0]<<"\n";
			//Y3
			cout<<"22\n"<<point[triangle[h][2]][1]<<"\n";
			//Z3
			cout<<"32\n"<<point[triangle[h][2]][2]<<"\n0\n";


		}

}
;
