
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <time.h>
#include <sstream>
#include <stdio.h>
#include "Entho1.h"
#include "Neuron.h"
#include "Func.h"
using namespace std;
#define LINETYPE 220

#define TYPE 4

void
Entho1::joining( int line )
{

  algo=1;
  cerr<<"ALGO:"<<algo<<"\n";
  switch ( algo )
  {
    case 1:
      //add an additional seg
      joining1( line );
      break;
    case 2:
      //melting
      joining2( line );
      break;
    case 3:
      //melting 1.1+0.0005
      joining3( line );
      break;
    case 4:
      //melting as long as length <40um :1.1+0.0005
      joining4( line );
      break;
    case 5:
      //melting in order
      joining5( line );
      break;
  }
}

Entho1::Entho1( char* fileName , int a)
{
	
}

Entho1::Entho1( char* fileName , int a, double x, double y, double z)
{
	name=fileName;
	algo=a;
       time_t start_time,end_time;

        time( &start_time );                /* Get time as long integer. */
  //reset array
  for ( int i = 0; i < MAX; i++ )
  {
    added[i] = 0;
    for ( int h = 0; h < 3; h++ ){
      join[i][h] = -1;
	  type[i]=3;
	}
  }




  int line = readFile( fileName );

  joining( line );

  
  cerr<<line<<"<<\n";
  createNeuron(x,y,z);

	time( &end_time );                /* Get time as long integer. */
	cerr<<"Time:"<< (end_time-start_time)<<"\n";
};

void
Entho1::createNeuron(double x, double y, double z)
{
  
  int tip = 1;
  int found = 0;
  int id = 1;
  //max of 100 different type of lines
  int l[100][2];
  //# of line to be converted in SWC
  int lineType;
  int jj;
  for ( jj = 0; jj < 100; jj++ )
  {
    l[jj][0] = 0;l[jj][1] = 0;
  }

  //decide which lineNr to transform in SWC
  int m;
  int k;
  for ( k = 0; k < count; k++ )
  {
    m = lineNr[k];
    //lokk for m in array
    for ( jj = 0; jj < 100; jj++ )
    {
      if ( l[jj][0] == m || l[jj][0] == 0 )
      {
        l[jj][0] = m;l[jj][1]++;
        break;
      }
    }
  }
  //look for max in ll
  int max = 0;
  for ( jj = 0; jj < 100; jj++ )
  {
    if ( max < l[jj][1] )
    {
      max = l[jj][1];
      lineType = l[jj][0];
    }
  }


  //store info
  int old[MAX][3];
  //set soma point outside count range
  point[count][0] = x;
  point[count][1] = y;
  point[count][2] = z;

  double min = 100000;

  for ( k = 0; k < count; k++ )
  {
    old[k][0] = join[k][0];
    old[k][1] = join[k][1];
    old[k][2] = join[k][2];
    double tmp;
    //exclude wrong lineType
    if ( lineNr[k] != lineType )
      continue;
    //exclude excluded lines
    if ( join[k][0] == -1 && join[k][1] == -1 && join[k][2] == -1 )
      continue;

    added[k] = 0;
    ident[k] = 0;
   tmp = distance( k, count );
    if ( min > tmp )
    {
      tip = k;
      min = tmp;
    }
  }
  int sxDx = 1;
  int tmp[MAX];
  int oldTip = - 1;
  //reset
  for ( k = 0; k < count; k++ )
  {
    join[k][0] = old[k][0];
    join[k][1] = old[k][1];
    join[k][2] = old[k][2];
    added[k] = 0;
    tmp[k] = 0;
  }
  found++;

  n = new Neuron();
  
  id = addLine( 1, 3, tip, -1 );
  
  found++;

  cerr << "tip:" << tip << " ";
  int notPrinted = 0;
  for ( k = 0; k < count; k++ )
    
  {
    
    if ( added[k] == 0 && ( join[k][0] + join[k][1] + join[k][2] ) != -3 )
    {
      notPrinted++;
      
    }
    tmp[k] = 1;

    }
  cerr << "not Printed:" << notPrinted << "  lineType:" << lineType
       << "  Init Tip:" << tip << "\n";
  

  
  

  cerr << "\nend\n";
 
  
  if ( found == 0 )
    return;
  n->init();
  
  char nam[100];
  sprintf(nam,"%s33-%d",name,algo);
  n->setName(nam);
  
  n->printSWC();
  
  n->printDXF();
};

//sxDx add segment momving to the left/right of the array (adding -1 or +1)
int
Entho1::addLine( int id, int typ, int index, int pid )
{
  int ind = index;
	
  if ( added[ind] == 0 )
  {
    ident[ind] = id;
    
	if(point[ind][0]==2.3735 &&point[ind][1]== 1.52137){
		int hj=0;

	}
	   /*    Character 7, thick axon
          Character 6, thin axon
          Character 4 dendrite
          Character 1  cell layer
          Character 2  contour of the hippocampus or entorhinal cortex
    */
	int tmpType=5;
	double rad=1;
	if(point[ind][3]==4){
		//dendrite
		tmpType=3;
		rad=0.6;
	}else if (point[ind][3]==6){
		tmpType=2;
		rad=0.2;
	}else if(point[ind][3]==7){
		tmpType=2;
		rad=0.4;
	}

    
	if(pid==1){
		int iii=pid;
	}
    n->add( id, tmpType, point[ind][0]*1000, point[ind][1]*1000, point[ind][2]*1000, rad, pid );
    added[ind] = 1;
    
    pid = id;
    id++;
  }

  if ( join[index][0] != -1 && join[index][1] != -1 && join[index][2] != -1 )
  {
    int iii = pid;
  }
  for ( int i = 0; i < 3; i++ )
  {
    if ( join[index][i] != -1 )
    {
      //add segment 
      ind = join[index][i];
      join[index][i] = -1;

      //remove other direction link
      for ( int u = 0; u < 3; u++ )
        if ( join[ind][u] == index )
          join[ind][u] = -1;

      id = addLine( id, typ, ind, pid );
    }
  }

  return id;
}
;

void
Entho1::joining1( int line )
{
  int welded = 0;
  int closest[MAX];
  int joined = 0;
  double minDist[MAX];
  int p, i;

  for ( p = 0; p < MAX; p++ )
  {
    
    remove[p] = 0;

  }



  // look for closest points and join them if they:
  //1-belong to diffent lines
  //2-they do NOT form a triforcation
  double dist,mDist ,minim_distance = 0.0;

  for( int w=0; w < 100;w++)
  {
	  if(w==50){
		int yyy=0;
		}
    for ( p = 0; p < MAX; p++ )
    {
      closest[p] = -1;
      minDist[p] = 100;
    }
     mDist = 100;
    //compute distances
    for ( i = 0; i < count; i++ )
    {
		if ( remove[i] == 1 ){
			continue;
		}
		if ( join[i][0] == -1 ){
			continue;
		}
		if ( !( join[i][0] == -1 || join[i][1] == -1 || join[i][2] == -1 ) ){
		 continue;
		}
      for ( int j = 0; j < count; j++ )
      {
        if ( remove[j] == 1 )
          continue;
        //if same structure don't join
        if ( lineNr[i] == lineNr[j] )
          continue;
        
        
        if ( !( join[j][0] == -1 || join[j][1] == -1 || join[j][2] == -1 ) )
          continue;
        if ( join[j][0] == -1 )
          continue;
        dist = distance( i, j );

        //absolute min
        if ( dist < mDist )
          mDist = dist;
        //compute minm distace
        if ( dist < minDist[i] )
        {
          minDist[i] = dist;
          closest[i] = j;
        }
      }
    }
    
    int found = 0;
    for ( int k = 0; k < count; k++ )
    {
      
      if ( remove[k] != 0 )
        continue;

      int i1 = closest[k];
      int i2 = closest[i1];
      
      if ( i1 == -1 )
        continue;

      
      if ( k == i2 )
      {
      
        i2 = k;
      
        if ( lineNr[i2] == lineNr[i1] )
        {
      
          closest[i1] = -1;

          continue;
        }
      
        if ( minDist[i2] > minim_distance )
          continue;
        
        if ( minDist[i2] > MAX_DIST )
          return;


    
         if ( joinSeg( i1, i2 ) == -1 )
            continue;
 

        found++;
        joined++;
        
        closest[i1] = -1;
        closest[i2] = -1;
        
        int lType = ( int ) lineNr[i1];
        
        for ( int g = 0; g < count; g++ )
        {
          if ( lineNr[g] == lType )
            lineNr[g] = lineNr[i2];
        }
        
        line--;
        
      }
    }
    if ( line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      break;
    }
    if ( found == 0 || line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      minim_distance = minim_distance * 1.1 + 0.001;
		joined = 0;
    }
	//next distance is less then closest point enlarge it!
	if(minim_distance<mDist)
		minim_distance=mDist*1.01;
  }
  
  cerr << "\n point:" << count << "----------- Weld:" << welded
       << "------------\n";
}
;


void
Entho1::joining2( int line )
{
  int welded = 0;
  int closest[MAX];
  int joined = 0;
  double minDist[MAX];
  int p, i;

  for ( p = 0; p < MAX; p++ )
  {
    
    remove[p] = 0;
  }



  // look for closest points and join them if they:
  //1-belong to diffent lines
  //2-they do NOT form a triforcation
  double dist,minim_distance = 0.0;
        for( int w=0; w < 100;w++)
  {
    for ( p = 0; p < MAX; p++ )
    {
      closest[p] = -1;
      minDist[p] = 100;
    }
    double mDist = 100;
    //compute distances
    for ( i = 0; i < count; i++ )
    {
      if ( remove[i] == 1 )
        continue;
      if ( join[i][0] == -1 )
        continue;
      if ( !( join[i][0] == -1 || join[i][1] == -1 || join[i][2] == -1 ) )
        continue;

      for ( int j = 0; j < count; j++ )
      {
        if ( remove[j] == 1 )
          continue;
        
        if ( lineNr[i] == lineNr[j] )
          continue;
        
        if ( ( i == 11 && j == 40 ) || ( i == 40 && j == 11 ) )
          int hhhh = 0;
        
        if ( join[j][0] == -1 )
          continue;
        if ( !( join[j][0] == -1 || join[j][1] == -1 || join[j][2] == -1 ) )
          continue;
        dist = distance( i, j );

        
        if ( dist < mDist )
          mDist = dist;
        
        if ( dist < minDist[i] )
        {
          minDist[i] = dist;
          closest[i] = j;
        }
      }
    }
    
    int found = 0;
    for ( int k = 0; k < count; k++ )
    {
      if ( remove[k] != 0 )
        continue;

      int i1 = closest[k];
      int i2 = closest[i1];
     
      if ( i1 == -1 )
        continue;

      
      if ( k == i2 )
      {
        if ( i1 == 11 || i2 == 11 )
          int hh = 11;
     
        i2 = k;
     
        if ( lineNr[i2] == lineNr[i1] )
        {
     
          closest[i1] = -1;

          continue;
        }
     
        if ( minDist[i2] > minim_distance )
          continue;
     
        if ( minDist[i2] > MAX_DIST )
          return;

        //melt points!
        if ( minDist[i2] < 0.001 )
        {
          if ( meltSeg( i1, i2 ) == -1 )
            continue;
        }
        else
        {
          if ( meltSeg( i1, k ) == -1 )
            continue;
        }

        found++;
        joined++;
        
        closest[i1] = -1;
        closest[i2] = -1;
        
        int lType = ( int ) lineNr[i1];
       
        for ( int g = 0; g < count; g++ )
        {
          if ( lineNr[g] == lType )
            lineNr[g] = lineNr[i2];
        }
        
        line--;
        
      }
    }
    if ( line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      break;
    }
    if ( found == 0 || line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      minim_distance = minim_distance * 2 + 0.001;
      joined = 0;
    }
  }
  
  cerr << "\n point:" << count << "----------- Weld:" << welded
       << "------------\n";
}
;

void
Entho1::joining3( int line )
{
  int welded = 0;
  int closest[MAX];
  int joined = 0;
  double minDist[MAX];
  int p, i;

  for ( p = 0; p < MAX; p++ )
  {
    
    remove[p] = 0;
  }



 
  double dist,minim_distance = 0.0;
        for( int w=0; w < 100;w++)
  {
    for ( p = 0; p < MAX; p++ )
    {
      closest[p] = -1;
      minDist[p] = 100;
    }
    double mDist = 100;
    //compute distances
    for ( i = 0; i < count; i++ )
    {
      if ( remove[i] == 1 )
        continue;
      if ( join[i][0] == -1 )
        continue;
      if ( !( join[i][0] == -1 || join[i][1] == -1 || join[i][2] == -1 ) )
        continue;

      for ( int j = 0; j < count; j++ )
      {
        if ( remove[j] == 1 )
          continue;
        //if same structure don't join
        if ( lineNr[i] == lineNr[j] )
          continue;
        //avoid point which eventual connection would create a triforcation
        if ( ( i == 11 && j == 40 ) || ( i == 40 && j == 11 ) )
          int hhhh = 0;
        
        if ( join[j][0] == -1 )
          continue;
        if ( !( join[j][0] == -1 || join[j][1] == -1 || join[j][2] == -1 ) )
          continue;
        dist = distance( i, j );

        //absolute min
        if ( dist < mDist )
          mDist = dist;
        //compute minm distace
        if ( dist < minDist[i] )
        {
          minDist[i] = dist;
          closest[i] = j;
        }
      }
    }
    
    int found = 0;
    for ( int k = 0; k < count; k++ )
    {
      if ( remove[k] != 0 )
        continue;

      int i1 = closest[k];
      int i2 = closest[i1];
     
      if ( i1 == -1 )
        continue;

      
      if ( k == i2 )
      {
        if ( i1 == 11 || i2 == 11 )
          int hh = 11;
     
        i2 = k;
     
        if ( lineNr[i2] == lineNr[i1] )
        {
     
          closest[i1] = -1;

          continue;
        }
     
        if ( minDist[i2] > minim_distance )
          continue;
     
        if ( minDist[i2] > MAX_DIST )
          return;


        
        if ( minDist[i2] < 0.001 )
        {
          if ( meltSeg( i1, i2 ) == -1 )
            continue;
        }
        else
        {
          if ( meltSeg( i1, k ) == -1 )
            continue;
        }

       

        found++;
        joined++;
        
        closest[i1] = -1;
        closest[i2] = -1;
        
        int lType = ( int ) lineNr[i1];
       
        for ( int g = 0; g < count; g++ )
        {
          if ( lineNr[g] == lType )
            lineNr[g] = lineNr[i2];
        }
       
        line--;
        
      }
    }
    if ( line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      break;
    }
    if ( found == 0 || line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      minim_distance = minim_distance * 1.1 + 0.0005;
      joined = 0;
    }
  }
  
  cerr << "\n point:" << count << "----------- Weld:" << welded
       << "------------\n";
}
;


void
Entho1::joining4( int line )
{
  int welded = 0;
  int closest[MAX];
  int joined = 0;
  double minDist[MAX];
  int p, i;

  for ( p = 0; p < MAX; p++ )
  {
    
    remove[p] = 0;
  }



 
  double dist,minim_distance = 0.0;
        for( int w=0; w < 100;w++)
  {
    for ( p = 0; p < MAX; p++ )
    {
      closest[p] = -1;
      minDist[p] = 100;
    }
    double mDist = 100;
    
    for ( i = 0; i < count; i++ )
    {
      if ( remove[i] == 1 )
        continue;
      if ( join[i][0] == -1 )
        continue;
      if ( !( join[i][0] == -1 || join[i][1] == -1 || join[i][2] == -1 ) )
        continue;

      for ( int j = 0; j < count; j++ )
      {
        if ( remove[j] == 1 )
          continue;
    
        if ( lineNr[i] == lineNr[j] )
          continue;
 
        if ( ( i == 11 && j == 40 ) || ( i == 40 && j == 11 ) )
          int hhhh = 0;
        
        if ( join[j][0] == -1 )
          continue;
        if ( !( join[j][0] == -1 || join[j][1] == -1 || join[j][2] == -1 ) )
          continue;
        dist = distance( i, j );

 
        if ( dist < mDist )
          mDist = dist;
     
        if ( dist < minDist[i] )
        {
          minDist[i] = dist;
          closest[i] = j;
        }
      }
    }
    
    int found = 0;
    for ( int k = 0; k < count; k++ )
    {
      if ( remove[k] != 0 )
        continue;

      int i1 = closest[k];
      int i2 = closest[i1];
    
      if ( i1 == -1 )
        continue;

      
      if ( k == i2 )
      {
        if ( i1 == 11 || i2 == 11 )
          int hh = 11;
    
        i2 = k;
    
        if ( lineNr[i2] == lineNr[i1] )
        {
    
          closest[i1] = -1;

          continue;
        }
     
        if ( minDist[i2] > minim_distance )
          continue;
     
        if ( minDist[i2] > MAX_DIST )
          return;

   

        if ( minDist[i2] < 0.04 )
        {
          if ( meltSeg( i1, i2 ) == -1 )
            continue;
        }
        else
        {
          if ( joinSeg( i1, k ) == -1 )
            continue;
        }

    

        found++;
        joined++;
        
        closest[i1] = -1;
        closest[i2] = -1;
        
        int lType = ( int ) lineNr[i1];
    
        for ( int g = 0; g < count; g++ )
        {
          if ( lineNr[g] == lType )
            lineNr[g] = lineNr[i2];
        }
    
        line--;
        
      }
    }
    if ( line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      break;
    }
    if ( found == 0 || line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      minim_distance = minim_distance * 1.1 + 0.0005;
      joined = 0;
    }
  }
  
  cerr << "\n point:" << count << "----------- Weld:" << welded
       << "------------\n";
}
;


void
Entho1::joining5( int line )
{
  int welded = 0;
  int closest[MAX];
  int joined = 0;
  double minDist[MAX];
  int p, i;

  for ( p = 0; p < MAX; p++ )
  {
    
    remove[p] = 0;
  }



  
  double dist,minim_distance = 0.0;
  
   for( int w=0; w < 100;w++)
  {
    for ( p = 0; p < MAX; p++ )
    {
      closest[p] = -1;
      minDist[p] = 100;
    }
    double mDist = 100;
    
    for ( i = 0; i < count; i++ )
    {
      if ( remove[i] == 1 )
        continue;
      if ( join[i][0] == -1 )
        continue;
      if ( !( join[i][0] == -1 || join[i][1] == -1 || join[i][2] == -1 ) )
        continue;

      for ( int j = 0; j < count; j++ )
      {
        if ( remove[j] == 1 )
          continue;
        
        if ( lineNr[i] == lineNr[j] )
          continue;
        
        if ( ( i == 11 && j == 40 ) || ( i == 40 && j == 11 ) )
          int hhhh = 0;
        
        if ( join[j][0] == -1 )
          continue;
        if ( !( join[j][0] == -1 || join[j][1] == -1 || join[j][2] == -1 ) )
          continue;
        dist = distance( i, j );


        if ( dist < mDist )
          mDist = dist;

        if ( dist < minDist[i] )
        {
          minDist[i] = dist;
          closest[i] = j;
        }
      }
    }

    double pOrder[MAX][2];
    int pOrderInd = 0;

    for ( int o = 0; o < MAX; o++ )
    {
      pOrder[o][0] = -1;pOrder[o][1] = -1;
    }

    int found = 0;

    for ( int k = 0; k < count; k++ )
    {
      if ( remove[k] != 0 )
        continue;
      int i1 = closest[k];
      int i2 = closest[i1];

      if ( i1 == -1 )
        continue;     
       if ( minDist[i2] > minim_distance)
          continue;
      
      if ( k == i2 )
      {

        i2 = k;

        if ( minDist[i2] > MAX_DIST ){
			cerr<<"Max Dist:"<<minDist[i2]<<"\n";
			return;
		}


        for ( int ll = 0; ll < MAX; ll++ )
        {
          if ( pOrder[ll][1] == -1 )
          {
            pOrder[ll][0] = i1;
            pOrder[ll][1] = minDist[i2];
            pOrderInd++;
            break;
          }
          else if ( minDist[i2] < pOrder[ll][1] )
          {
            
            for ( int u = pOrderInd; u > ll; u-- )
            {
              pOrder[u][0] = pOrder[u - 1][0];
              pOrder[u][1] = pOrder[u - 1][1];
            }
            pOrder[ll][0] = i1;
            pOrder[ll][1] = minDist[i2];
            pOrderInd++;
            break;
          }
        }


      }
    }

	

   

    for ( int oo = 0; oo < pOrderInd; oo++ )
    {
	

		int i1,i2;
		i1=pOrder[oo][0];
		i2=closest[i1];
		if(i1==-1 || i2==-1)
		   continue;
 		if(point[oo][0]==2.58547 || point[oo][0]==2.58291)
			int h=0;
		if(oo==73 ||oo==72 || oo==48)
			int i=oo+i1+i2;
		if(i1==73 ||i1==72 || i1==48)
			int i=oo+i1+i2;
		if(i2==73 ||i2==72 || i2==48)
			int i=oo+i1+i2;

     if ( lineNr[i2] == lineNr[i1] )
      {
        
        closest[i1] = -1;
		if(pOrder[oo][1]==0)
			int k=0;
        continue;
      }
	  
	  

	  double tt=pOrder[oo][1];
      
      if ( minDist[i2] < 0.009 )
      {
        if ( meltSeg( i1, i2 ) == -1 )
          continue;
		welded++;
      }
      else
      {
        if ( joinSeg( i1, i2 ) == -1 )
          continue;
      }


      found++;
      joined++;
      
      closest[i1] = -1;
      closest[i2] = -1;
      
      int lType = ( int ) lineNr[i1];
      
      for ( int g = 0; g < count; g++ )
      {
        if ( lineNr[g] == lType )
          lineNr[g] = lineNr[i2];
      }
      
      line--;
      
    }


    if ( line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined << "\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      break;
    }
    if ( found == 0 || line == 1 || minim_distance > MAX_DIST )
    {
      cerr << w << " LineT:\t" << line << "\tJoin:\t" << joined <<"\tm:"
           << mDist << "\tmDist:\t" << minim_distance << "\n";
      minim_distance = minim_distance * 5 + 0.001;
      joined = 0;
    }
  }
  
  
  cerr << "\n point:" << count << "----------- Weld:" << welded
       << "------------\n";
}
;


int
Entho1::readFile( char* fileName )
{

  double ax = 0,ay = 0,az = 0,a = 0;
  ifstream* in = new ifstream( fileName,
                               ios::in | ios::binary );

  if ( in == NULL )
  {

    cerr << "Unable to find:" << fileName
         << " !\n----------------------------------\n";
    exit( -1 );
  }

  double x,y,z,xx,yy,zz,id,type,oldType = - 1,newSegment = 0;
        
        int line = 0;
  count = 0;
  char c;

  int findEnd = 1;
  double dist = 0;
	int skip=0;
  while ( in->peek() != -1 )
  {
    *in >> x; 

    while ( in->peek() == 44 )
      c = in->get();
    *in >> y;

    while ( in->peek() == 44 )
      c = in->get();
    *in >> z;

    while ( in->peek() == 44 )
      c = in->get();
    *in >> id;
  
    while ( in->peek() == 44 )
      c = in->get();
    *in >> type;
  
    while ( in->peek() == 44 )
      c = in->get();

  
		if(x==3.41966 ){
			int hh=count;
		}
    
    int TYP = 4,TYP1 = 6,TYP2 = 7,iii = 1;
    if ( !( ( type == TYP || ( type == 0 && oldType == TYP ) ) ||
		( type == TYP1 || ( type == 0 && oldType == TYP1 ) )  ||
		( type == TYP2 || ( type == 0 && oldType == TYP2 ) ) ) ){
			oldType=type;
			continue;
	}
		
	    point[count][0] = x;
	    point[count][1] = y;
	    point[count][2] = z;
	    point[count][3] = type;


		skip=0;
		
		for(int hh=1;hh<(count-2);hh++){
				
				if((type!=0 && point[hh][0]==x && point[hh][1]==y && point[hh][2]==z && point[hh-1][0]==xx && point[hh-1][1]==yy && point[hh-1][2]==zz) ||
				   (oldType!=0 && point[hh-1][0]==x && point[hh-1][1]==y && point[hh-1][2]==z && point[hh][0]==xx && point[hh][1]==yy && point[hh][2]==zz)){
						int jj=point[hh][0]+point[hh][0]+point[hh][0]+count+hh;
						xx=x;yy=y;zz=z;oldType=type;
						skip=1;
						break;
				}
		}
		if(skip==1)
			continue;
		

	   
	    ax += x;ay += y;az += z;a++;
	   
	    startP[count] = 0;
	    if ( findEnd == 1 )
	    {
	      line++;
	      startP[count] = 1;
	    
	      if ( count > 1 )
	      {
	        findEnd = 0;
	        
	      }
	    }
	    else
	   {
			
	      joinSeg( count, count - 1 );
	      double tmp = distance( count, count - 1 );
	      if ( tmp > dist )
	        dist = tmp;
	      if ( tmp > 1 )
	       int oo = count;
	   }
	   //look if it defines an end
	   if ( id == 0 && endP[count - 1] == 1 )
		{
	     
	     startP[count] = 1;
	   }
	   // evry line has a differetn id
	   lineNr[count] = line;
	  
		   endP[count] = 0;
	  if ( type == 0 )
	  {
	    findEnd = 1;
	    point[count][3] = oldType;
	     endP[count] = 1;
	     //since it's the last point
	  }
	   count++;
	   	   xx=x;yy=y;zz=z;
	   oldType = type;
	  
	}
  
  ax = ax / a;
  ay = ay / a;
  az = az / a;
  return line;
}

int
Entho1::joinSeg( int i1, int i2 )
{
  int i, j;

  //check if already present
  for ( i = 0; i < 3; i++ )
  {
    if ( join[i1][i] == i2 )
      return 1;
  }

  for ( i = 0; i < 3; i++ )
  {
    if ( join[i1][i] == -1 )
      break;
  }
  for ( j = 0; j < 3; j++ )
  {
    if ( join[i2][j] == -1 )
      break;
  }
  if ( i > 2 || j > 2 ){
cout<< "----"<<i1<<","<<i2<<endl;

    return -1;
  }

  if(TYPE!=0){
	  type[i1]=TYPE;
	  type[i2]=TYPE;
  }
  join[i1][i] = i2;
  join[i2][j] = i1;

  return 1;
}
;


int Entho1::meltSeg( int i1, int i2 )
{
  int i, j;

  for ( i = 0; i < 3; i++ )
  {
    if ( join[i1][i] == -1 )
      break;
  }
  for ( j = 0; j < 3; j++ )
  {
    if ( join[i2][j] == -1 )
      break;
  }
 
  if ( ( i + j ) > 3 )
    return joinSeg( i1, i2 );

 
  for ( int y = i; y < 3; y++ )
  {
 
    int d = join[i2][y - i];

 
    join[i1][y] = d;

 
    for ( int u = 0; u < 3; u++ )
    {
      if ( join[d][u] == i2 )
        join[d][u] = i1;
    }

    join[i2][y - i] = -1;
  }

  
  return 1;
}
;

int
Entho1::printDXF()
{
  static int segment = 0;
  int start = 0;
  int stop = 750999;

  char* color = "62\n600\n";
  double x,y,z,x1,y1,z1,type = 3;
          int l;
 
  for ( int h = start; h < count; h++ )
    for ( int i = 0; i < 2; i++ )
    {
 
      l = join[h][i];
      if ( l == -1 )
        continue;
      if ( lineNr[h] != LINETYPE )
        continue;
      x = point[h][0];
      y = point[h][1];
      z = point[h][2];
      cerr << lineNr[h] << " ";
      x1 = point[l][0];
      y1 = point[l][1];
      z1 = point[l][2];
      if ( h > stop )
        return segment;


 
      if ( segment % 100 == 0 )
        cerr << h << "\n";
      segment++;
      printLine( type, x, y, z, x1, y1, z1 );
    }

  for ( int i = start; i < count; i++ )
  {
    for ( int j = 0; j <= 1; j++ )
      if ( join[i][j] != -1 )
      {
        int h = join[i][j];
        x = point[h][0];
        y = point[h][1];
        z = point[h][2];

        x1 = point[i][0];
        y1 = point[i][1];
        z1 = point[i][2];
        if ( h > stop )
          return segment;

     
        if ( x == x1 && y == y1 && z == z1 )
          continue;

        type = 20;
     
        if ( segment % 100 == 0 )
          cerr << h << "\n";
        segment++;
        printLine( type, x, y, z, x1, y1, z1 );
      }
  }


  return segment;
};

int
Entho1::printDXFOrg()
{
  static int segment = 0;
  int start = 0;
  int stop = 750999;

  char* color = "62\n600\n";
  double x,y,z,x1,y1,z1,type,end_point;
 
  for ( int h = start; h < count - 1; h++ )
  {
    
    x = point[h][0];
    y = point[h][1];
    z = point[h][2];

    x1 = point[h + 1][0];
    y1 = point[h + 1][1];
    z1 = point[h + 1][2];
    if ( h > stop )
      return segment;

    type = point[h][3];
    end_point = endP[h];
    if ( end_point == 1 )
      continue;

  
    if ( segment % 100 == 0 )
      cerr << h << "\n";
    segment++;
    printLine( type, x, y, z, x1, y1, z1 );
  }

  for ( int i = start; i < count - 1; i++ )
  {
    for ( int j = 0; j <= 1; j++ )
      if ( join[i][j] != -1 )
      {
        int h = join[i][j];
        x = point[h][0];
        y = point[h][1];
        z = point[h][2];

        x1 = point[i][0];
        y1 = point[i][1];
        z1 = point[i][2];
        if ( h > stop )
          return segment;

       
        if ( x == x1 && y == y1 && z == z1 )
          continue;

        type = 20;
       
        if ( segment % 100 == 0 )
          cerr << h << "\n";
        segment++;
        printLine( type, x, y, z, x1, y1, z1 );
      }
  }

  return segment;
};

void
Entho1::printLine( double type,
                   double x,
                   double y,
                   double z,
                   double x1,
                   double y1,
                   double z1 )
{
  cout << "3DFACE\n8\n2\n";
  type = 20;
  switch ( ( int ) type )
  {
    case 21:
      cout << "62\n340\n";
      break;
    case 20:
      cout << "62\n300\n";
      break;
    case 11:
      cout << "62\n200\n";
      break;
    case 7:
      //cout<<"62\n600\n";
      //break;
    case 6:
      cout << "62\n200\n";
      break;
    case 4:
      cout << "62\n2500\n";
      break;
    case 2:
      cout << "62\n300\n";
      break;
    case 1:
      cout << "62\n600\n";
      break;
  }

  //X1
  cout << "10\n" << x << "\n";
  //Y1
  cout << "20\n" << y << "\n";
  //Z1
  cout << "30\n" << z << "\n";



  double inc = 0;
  //X2
  cout << "11\n" << ( x + x1 ) * 0.5 + inc << "\n";
  //Y2
  cout << "21\n" << ( y + y1 ) * 0.5 + inc << "\n";
  //Z2
  cout << "31\n" << ( z + z1 ) * 0.5 + inc << "\n";

  //X3
  cout << "12\n" << x1 << "\n";
  //Y3
  cout << "22\n" << y1 << "\n";
  //Z3
  cout << "32\n" << z1 << "\n0\n";
}
;

void
Entho1::EnthoOld( char* fileName )
{
};
