
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <sstream>
#include <stdio.h>
#include "Entho.h"
#include "Neuron.h"
#include "Func.h"
using namespace std;

Entho::Entho(char *fileName){
	int line=readFile(fileName);
	joining(line);
			
};


void Entho::joining(int line){
	int welded=0;
	int closestAvailable[MAX];
	int closest[MAX];
	//store the location of end segment
	int end[MAX];
	int joined=0;
	double minDist[MAX],minDistAvailable[MAX],mini=100;
    int p, i,tempIndex=0;
	int availableIndex=0;
	// put all end point of segment in the array
	//while looking for segment with lowest z
	double zMin=100;
	int zIndex=-1;

	int tmp=segIndex;
	Segment *t;
	for( i=0;i<tmp;i++){
		if(seg[i]->getEnd()->getZ()<zMin){
			zMin=seg[i]->getEnd()->getZ();
			zIndex=i;
		}

		t=seg[i];
		//look for end seg
		while (t->getNext1()!=NULL)
			t=t->getNext1();
		segIndex++;
		seg[segIndex]=t;
		end[i]=segIndex;
		end[segIndex]=i;

	}
	line=segIndex;

	for(p=0;p<MAX;p++){
          join[p][0]=-1;
          join[p][1]=-1;
	}

	//prepare available segments, just end of first segment and make null in the list
	soma=seg[zIndex];
	soma->setPid(-1);
	available[availableIndex]=seg[end[zIndex]];
	availableIndex++;
	seg[zIndex]=NULL;
	seg[end[zIndex]]=NULL;
	

	// look for closest points and join them if they:
	//1-belong to diffent lines
	//2-they do NOT form a triforcation
	double dist,minim_distance=0.0;

	
	for(int w=0;w<100;w++){
		cerr<<"+";
	
		for(p=0;p<MAX;p++){
			closest[p]=-1;
			closestAvailable[p]=-1;
			minDist[p]=100;
			minDistAvailable[p]=100;
			remove[p]=0;
		}

		//compute distances from segment available
		for(i=0;i<availableIndex;i++){
			
				
			if(available[i]==NULL)
				continue;
			double min=100,max=-100;
			for(int j=0;j<segIndex;j++){
				if(seg[j]==NULL)
					continue;
				
				//PUT ON WAIT: THIS CONDITION!!!!!!!!!!
				//avoid point which eventual connection would create a triforcation

				
				dist=distance(available[i],seg[j]);
				if(dist<min)
					min=dist;
				if(dist>max)
					max=dist;
				//compute minm distace
				if(dist<minDist[i]){
					minDist[i]=dist;
					closestAvailable[i]=j;
				}
				if(dist<minDistAvailable[j]){
					minDistAvailable[j]=dist;
					closest[j]=i;
				}
			}
		}


		//check closest points that recognize each others
		// then set tip[i]=1
		int found=0;
		for(int k=0;k<availableIndex;k++){
			if(available[k]==NULL)
				continue;

			int i1=closestAvailable[k];
			int i2=closest[i1];
			int i3=closest[end[i1]];
			if(k==i3)
				int ok=i2;
			
			if(i1==-1 || seg[i1]==NULL)
				continue;
			i2=k;
			
			if(k==i2){
				//join node k and i1
				// check if lineNr is different since it changes dinamically!!

				//start joining the closest first
				// and then start joining farder away
				if( minDist[k]>minim_distance){
					if (mini>minDist[k]) mini= minDist[k];
					//continue;
				}
				//if the points are 0.5 mm far away... do not join them!!!
				if(minDist[k]>MAX_DISTANCE)
					return;
				found++;
				joined++;
				if(found%10==0){
					int i=found;
				}
				remove[i1]=1;
				
				//join from k, segment i1, then remove segment i1 and its end and add the end to available

			
				//reverse poitner if seg[i1] is end point
				if(seg[i1]->getPrev()!=NULL){
					Segment *t =seg[i1];
					Segment *p;
					while(t->getPrev()!=NULL){
						t->setNext1(t->getPrev());
						p=t;
						t=t->getPrev();
						p->setPrev(NULL);
					}
					if(t->getNext1()!=NULL)
						t->setNext1(NULL);

				}
				int trif=0;
					//join segment
					if(available[k]->getNext1()==NULL){		
						//join segment to available
						available[k]->setNext1(seg[i1]);		 
					}else if(available[k]->getNext2()==NULL){
						available[k]->setNext2(seg[i1]);
					} else{						
						//if HERE trifurcation found!
						trif=1;;
					}
				
					//if joined then remove from list
					if(trif==0){
						
						available[availableIndex]=seg[end[i1]];
						availableIndex++;
						//remove segment
						seg[i1]=NULL;
						seg[end[i1]]=NULL;
					}





				}
						
				
				//join j1 after j2 since possibly j2 is an end point;


				
				connect[k]+=connect[i1];
				closest[i1]=-1;
				int lType = (int)lineNr[i1];
				//change all line type lineNr[i1] in lineNr[k]
				for(int g=0;g<count-1;g++){
					if(lineNr[g]==lType)
						lineNr[g]=lineNr[k];
				}
				//move down stream 
				line--;
				

		
		}
		
			cerr<<"LineT:\t"<<line<<"\tJoined:\t"<<joined<<"\tMinDist:\t"<<minim_distance<<"  m:"<<mini<<" prob:"<<tempIndex<<"\n";
			minim_distance=minim_distance*5+0.001;//0.005;
			joined=0;
		

	
		if (line==1 || minim_distance>MAX_DISTANCE)
			break;
	}


	
		cerr<<"\n ----------- Weld:"<< welded<<"------------\n";

		//initialize neuron
		init();

}
;
int Entho::readFile(char *fileName){
	ifstream* in=new ifstream(fileName, ios::in | ios::binary );
	
	if(in == NULL )
          {
            
            cerr<<"Unable to find:"<<fileName<<" !\n----------------------------------\n";
            exit(-1);
          }

	//reset Segmetn array
	segIndex=-1;
	for(int i=0;i<MAX;i++)
		seg[i]=NULL;	

	double x,y,z,id,type,oldType=-1,newSegment=0;
	
	int line=0;
	int count=0;
	char c;

	while( in->peek() != -1 ){
			
			*in>>x;	
			//remove coma if present
			while (in->peek()==44 )	
				 c=in->get();
			*in>>y;
			//remove coma if present
			while (in->peek()==44 )	
				 c=in->get();
			*in>>z;
			//remove coma if present
			while (in->peek()==44 )	
				 c=in->get();
			*in>>id;
			//remove coma if present
			while (in->peek()==44 )	
				 c=in->get();
			*in>>type;
			//remove coma if present
			while (in->peek()==44 )	
				 c=in->get();

/*
			Character 7, thick axon
			Character 6, thin axon
			Character 4 dendrite
			Character 1  cell layer
			Character 2  contour of the hippocampus or entorhinal cortex
*/
			//consider only?
			int TYP=6,TYP1=7,iii=1,TYP2=-1;
			if ( !((type==TYP || (type==0 && oldType==TYP)) || (type==TYP1 || (type==0 && oldType==TYP1))|| (type==TYP2 || (type==0 && oldType==TYP2))))
				 continue;

			//consider only if z>1
			if(z<2)
				continue;
			
			
			point[count][0]=x;
			point[count][1]=y;
			point[count][2]=z;
			point[count][3]=type;
			//number of line connection
			connect[count]=2;
			//is a start point?
			startP[count]=0;

			if(id<0){
				startP[count]=1;
				connect[count]=1;
				segIndex++;
			}
			//create new Segment
			Segment * t=new Segment((line+1),3, new Vector(x,y,z), 1, 1);
			Segment * p=seg[segIndex];
			///look for right place too put the new segment
			if(p==NULL){
				seg[segIndex]=t;
			}else{
				//look for an empty spot
				while (p->getNext1()!=NULL)
					p=p->getNext1();
				p->setNext1(t);
				//set prev father!
				t->setPrev(p);
			}
			// every line has a differetn id
			lineNr[count]=line;
			//is an end point?
			endP[count]=0;
			if(type==0){
				point[count][3]=oldType;
				endP[count]=1;
				connect[count]=1;
				line++;
				//since it's the last point
			}
			count++;
			oldType=type;
	}
	return line;

}

