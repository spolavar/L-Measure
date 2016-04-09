/*********************************************************************************
*
*                  Morphological Neuron Analysis
*                        Ruggero Scorcioni
*
*                  00-03-28: Start Date Implementation
*				   00-07-12: Version 0.91 alpha
*
*
***********************************************************************************/

#include "Neuron.h"
#include "Elaboration.h"

#include "Segment.h"  
#include "Vector.h"
#include <iostream>
#include "wrl.h"
#include "3D.h"
#include "Morph.h"
#include "Entho1.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "ParameterList.h"
#include <sys/types.h>
#include <dirent.h>


#include <unistd.h>


#include <stdio.h>


#include "RString.h"
using namespace std;

int _chdir( char* s ) {return -1;}


//store the specifity of the elaboration
Func* globalLimit;
	ListElaboration * l ;

Func * findFunc(int i,Func *ii);
void elabListNeuron(char * list[], char * param, int nClass[]);


//Raw Data Print
int RAW_DATA=0;
	FILE *stream;

void funcParam(char *par){
// check for function list
	int pos=0;int fun2=-1;
	int fun1=atoi(par);
	//cerr<<"Func:"<<fun1<<"\n";

	char * find=strchr(par,(int)'f');
	if(find==0){
		fun2=-1;
	}else {
		find++;
		fun2=atoi(find);
		par=find;
	}
	
	pos=strcspn(par,",");
	par=par+pos+1;
	//get third param: get average YES/NO
	int avg=atoi(par);
	
	pos=strcspn(par,",");
	par=par+pos+1;
	//get fourth param: get BinMode
	int binMode=atoi(par);
	
	pos=strcspn(par,",");
	par=par+pos+1;
	//get fifth param: get binPar
	double binPar=atof(par);
	
	if(fun2==-1){
		//single
		new Single(l,findFunc(fun1,globalLimit),1,RAW_DATA);
	}else{
		int direct=0;
		//if distribution over Order then set direct
		if(fun2==6) direct=1;
		new Distr(l,findFunc(fun1,globalLimit),findFunc(fun2,globalLimit),avg,direct,binMode,binPar,RAW_DATA);
	}
}
;

void saveParam(char *par){
   
	stream = freopen( par, "w+", stdout );
	
}

void limitParam(char *par){

	int bol=atoi(par);
	int pos=strcspn(par,",");
	par=par+pos+1;
	
	int val=atoi(par);
	pos=strcspn(par,",");
	par=par+pos+1;
	//get function
	int func=atoi(par);
	pos=strcspn(par,",");
	par=par+pos+1;
	//get parameter
	int param=(int)atof(par);
	Func * temp;
	temp=findFunc(func,0);
	//consider >,=,<
	if(val==1)
		temp=new Lesser(temp,param); 
	if(val==2)
		temp=new Equal(temp,param); 
	if(val==3)
		temp=new Greater(temp,param); 
	if(globalLimit==0){
		globalLimit=temp;
	}else{
		//insert actual limit with And/Or to the actual globalLimit
		if(bol==1){
			globalLimit=new And(globalLimit,temp);
		}else{
			globalLimit=new Or(globalLimit,temp);
		}
	
	}
}
;

int main(int argc, char* argv[])
{

	
	char  command[]="v_e_moto1.CNG.swc";
	if(false) {
		
		Entho1 * e= new Entho1("0hpca2", atoi(argv[1]), 3.6453, 2.0683, 2.47 );
		
		exit(0); 
	}

	
	if(false){
		
		Wrl* r=new Wrl("outsideOK.wrl");
		
		
		exit(0);
	}



	//Start of Lneuron
	if(false){
		ParameterList* p=new ParameterList();
		
		p->readParameter("c:\\research\\LMeasure\\tamori.prm");
		
		Neuron * n=new Neuron(p);
		n->printSWC();
		exit(0);
	}



	//Start of Lmeasure
	char param[50];
	char* par[MAX_NEURON];
	param[0]=0;
	globalLimit=0;
	   char * list[MAX_NEURON];
	   int neuroClass[MAX_NEURON];


	
	 l =new ListElaboration();
         
         int k;

	 cerr<<"LMeasure v"<<VERSION<<"\n";

	 if(argc==2){
		 //read parameters from file
       ifstream* in=new ifstream(argv[1], ios::in | ios::binary );

       if(in == NULL ){
			//problem found
			cerr<<"Unable to find:"<<argv[1]<<" !\n----------------------------------\n";
			exit(1);
		}
		int id=1;
			
		par[0]=new char[10];
		strcpy(par[0],"LM.exe");
		char *n=new char[450];
		while ( in->peek()!=-1 ){
			
			
			*in>>n;
			int len=strlen(n)+1;
			 par[id]=new char[len];
			 strcpy(par[id],n);
			if( in->peek()==-1) break;
			
			id++;
			if(id==27)
				int jj=id;
			if(id>MAX_NEURON){
				cerr<<"Too many input neuron!!!!\n";
				break;
			}
		}


		//check if last two files are the same
		if(strcmp(par[id],par[id-1])==0)
			id--;
		argv=par;
		argc=id+1;
		//if not neuron loaded then stop
		if(id==1){
			cerr<<"Error on File Param:"<<argv[1]<<"\n";
			exit(-1);
		}
		in->close();
	 }
	for(int kg=1;kg<argc;kg++){
		
		//replace '*' in the file path with ' '
		int i=0;
		while(argv[kg][i]!=0){
			if(argv[kg][i]=='*')
				argv[kg][i]=' ';
			i++;
		}

	}
	//look for limits & Raw data directive
	for(k=1;k<argc;k++){
		if(strncmp(argv[k],"-R",2)==0 )
			RAW_DATA=1;
		if(strncmp(argv[k],"-l",2)==0 ){
			//append list of parameters, but not "-"
			
			limitParam(argv[k]+2);	
			//remove param
			argv[k]="-";
		}
		//redirect output on file if -s param is present
		if(strncmp(argv[k],"-s",2)==0 ){
			 

			saveParam(argv[k]+2);	
			 

			//remove param
			argv[k]="-l";
		}
		//To use parameter:
		
	}

	//look for function and rest parameter
	cerr<<"argc:"<<argc;
	for( k=1;k<argc;k++){
		
		if(strncmp(argv[k],"-f",2)==0 ){
			funcParam(argv[k]+2);
			
			argv[k]="-l";
		}
		if(strncmp(argv[k],"-",1)==0){
			//append list of parameters, but not "-"
			strcat(param,argv[k]+1);
		}
		//To use parameter:
		
	}
	
	//add Class elaboration in case of MAchine Learning
	if(strchr(param,'M')!=NULL) {
		funcParam("99,0,0,10.0");
	}


	// HELP File
	if(strchr(param,'h')!=NULL ){ //|| argc==1) {
		cout<<"MoA: Morpho-Analysis by Ruggero Scorcioni, release 0.1 (alpha)\n";
		cout<<"Triforcations are always corrected!	\n";
		cout<<"a: set 1\n";
		cout<<"b: set 2\n";
		cout<<"H: Hillman's parameters\n";
		cout<<"e: expand(correct for shrinkage\n";
		cout<<"h: help\n";
		cout<<"f<#1>,f<#2>,<average>,<BinMode>,<BinPar>	\n";
		cout<<"l<#1>,<parameter>	\n";
		cout<<"s<fileNale> :save output in <fileName>	\n";
		cout<<"G :global Study	\n";
		cout<<"R :print Raw Data	\n";
		cout<<"c :compress output (one line for neuron)\n";
	//	cout<<"	\n";
		cout<<"p: print SWC format\n";
		return 0;
	}
	
	   
   {

	
	   for(int iy=0;iy<MAX_NEURON;iy++){
		   list[iy]=new char[1];
		   list[iy][0]=0;
		   neuroClass[iy]=0;
	   }
	   int indexList=0;
	//Build Neurons and compute Elaboration

           int neuronNr=0;
	for(int i=1;i<argc;i++)
		//run only on neuron name and avoid parameter list
		if(strncmp(argv[i],"-",1)!=0){
			
			neuronNr++;
			if(strchr(param,'c')!=NULL) { 
				cerr<<"Neuron#:"<<neuronNr<<" "<<argv[i]<<" ";
			} else {
				//one line for neuron
				cerr<<"Neuron#:"<<neuronNr<<" "<<argv[i]<<"\n";
			}

			//check for directory presence
			char direc[250];
			strcpy(direc,"");
			//substitute all '\' into'\\' in direc
			char * dir;
			dir=argv[i];
			int len=strlen(dir);

			//in case of Machine learning output consider every neuron as a directory
			if(strchr(param,'M')!=NULL  && _chdir(direc)!=-1) { 
				for(int u=(len-1);u>0;u--){
				
					if( strchr(dir+u,'//')!=NULL ){
						dir[u]=0;
						len=u+1;
						break;
					}
					
					
				}
			}
			
		
			
				for(int u=0;u<len;u++){
					strncat(direc,argv[i]+u,1);
					if(strncmp(argv[i]+u,"//",1)==0)
						strcat(direc,"/");
				}
			
			int first=0;
			//in case of ML or when a directory is passed then runon every file present
			
			if(chdir(direc)!=-1){
			if(true){
			
				chdir(direc);
				cerr<<"Inside Dir:"<<direc<<"\n";
				
				static int neuronalClass=0;
				len=strlen(direc);
				
				if(direc[len-1]!='/'){
					strcat(direc,"//");
				}
				
				neuronalClass++;
			
				DIR *dirp;
				struct dirent *dp;
				long hFile;
				
				cerr<<"after replacement"<<direc;
				if((dirp = opendir(direc))== NULL)
					cout<<"error!! directory not found";
                                   
					// process this file
                                   //while( false ) // GUIDO _findnext( hFile, &c_file ) == 0 )
					while((dp = readdir(dirp))!= NULL)
					{
						
						cerr<<dp->d_name<<"\n";
						if(strcmp(dp->d_name,"..")==0 || strcmp(dp->d_name,".")==0)
                                           		continue;
                                           // GUIDO 
							int len=strlen(direc)+strlen(dp->d_name);
							
							list[indexList]=new char[len+1];
							neuroClass[indexList]=neuronalClass;
							strcpy(list[indexList],direc);
							strcat(list[indexList],dp->d_name);
							indexList++;
						
					}
                                   // GUIDO _findclose( hFile ); 
				closedir(dirp);
				} // IF FALSE
			}else {
				//single Neuron
				cerr<<"Single Neuron:"<<direc<<"\n";




						int len=strlen(argv[i]);
						char *ops;
						ops=list[indexList];
						delete ops;
						list[indexList]=new char[len+1];
						neuroClass[indexList]=0;
						strcpy(list[indexList],argv[i]);
						indexList++;
			}			
		}

	}

	elabListNeuron(list,param,neuroClass);
	
	cout<<" ";
	cerr<<" ";
	if(stream!=0)
		fclose(stream);
	exit(0);
}
;
void elabListNeuron(char * list[], char * param, int nClass[]){
   int ind=0;


   //SG code to print time
   print_Current_Time();
   //precompute Step separate from other loop only for global study

   //Code to capture the diametre of the amira files
   char dia[50];
   bool idDiaPresent = false;
     if(strchr(param,'d')!=NULL){

  	   char * op = strchr(param,'d');
  	 int i;
  	   for(i = 1; i < strlen(op); i++){
  	  //char * str = param[found_d+i];
  		   dia[i-1] =  op[i];

  	   }
  	   dia[i-1] = '\0';
  	   idDiaPresent = true;
     }


   if(strchr(param,'G')!=NULL){
	   while(list[ind][0]!=0){
		   Neuron* n=new Neuron(list[ind],nClass[ind], idDiaPresent, dia);
							

							if(n==NULL){
								cerr<< "    above neuron NULL!\n";
								continue;
							}

							if(strchr(param,'e')!=NULL) 
								n->adjustShrinkage(n->getSoma());
							
							if(ind==0 ||(strchr(param,'G')==NULL)){							
								//reset once for global and always otherwise
								l->newNeuron();
							}
							l->preCompute(n);
							n->~Neuron();
							n=NULL;

							ind++;

							//SG code to print time
							print_Current_Time();
							}
	   }
   
   //Compute Step
   ind=0;
   while(list[ind][0]!=0){
	
	   // Modified by Akshay


	   	   	   	   	   	   	std::string str2;
	   	   					str2 = list[ind];

	   	   					std::string inputFileType;
	   	   					inputFileType = str2.substr(str2.length() - 4 );

	   	   					cerr << "\n\n\n\n" << "inputFileType.." << inputFileType << "\n\n\n\n";

	   	   	   	   	   	   	const int MAXITEMS = 10;
	   						string lastItem;
	   						string fileTypes[MAXITEMS] = {"swc","SWC","txt","TXT","asc","ASC","AM","am","out","OUT"};
	   						int typeCnt = 0;

	   						for (typeCnt = 0; typeCnt < MAXITEMS; typeCnt++) {
	   							if(inputFileType.find(fileTypes[typeCnt]) != string::npos) {
	   								break;
	   							}
	   						}

	   						if (typeCnt < MAXITEMS) {
	   							cerr << "\n\n\n\n" << "Valid file.." << list[ind] << "\n\n\n\n";

	   							Neuron* n = new Neuron(list[ind], nClass[ind], idDiaPresent, dia);

	   							if (strchr(param, 'e') != NULL)
	   								n->adjustShrinkage(n->getSoma());
	   							//C: doPCA()
	   							if (strchr(param, 'C') != NULL)
	   								n->doPCA();

	   							//p: Print SWC file format

	   							if (strchr(param, 'p') != NULL) {

	   								n->printSWC();

	   							}
	   							//w: Print WRL file format
	   							if (strchr(param, 'w') != NULL) {
	   								n->printWRL();
	   							}
	   							//x: Print DXF file format
	   							if (strchr(param, 'x') != NULL) {
	   								n->printDXF();
	   							}

	   							//g: Print PGM file format
	   							if (strchr(param, 'g') != NULL)
	   								n->printPGM();

	   							//if global do partial reset
	   							if (strchr(param, 'G') != NULL) {
	   								l->newRealNeuron();

	   							}

	   							if (n == NULL) {
	   								cerr << "    above neuron NULL!\n";
	   								continue;
	   							}

	   							if (strchr(param, 'e') != NULL)
	   								n->adjustShrinkage(n->getSoma());

	   							if (strchr(param, 't') != NULL)
	   								n->LookForTrees(n->soma, "sub_trees_dir");
	   							//n: print subtrees in temporary directory
	   							if (strchr(param, 'n') != NULL) {
	   								char result[20];
	   								strcpy(result, strchr(param, 'n') + 1);
	   								cerr << "dirname:" << result;
	   								n->LookForTrees(n->soma, result);
	   							}

	   							if (strchr(param, 'G') == NULL) {
	   								//no Global
	   								l->newNeuron();
	   								l->preCompute(n);

	   							}

	   							//compute all Elaborations for neuron n
	   							l->compute(n);

	   							n->~Neuron();
	   							n = NULL;


	   						std::string str;
	   						str = list[ind];

	   						if (strchr(param, 'G') == NULL) {
	   							//no Global
	   							if (strchr(param, 'M') != NULL) {
	   								l->printDistrDB(str);
	   							} else {
	   								l->printDistr(str);
	   							}
	   						}


	   				} else {
	   					cerr << "\n\nWrong FileName:" << str2 << " \n";
	   					cerr << "Given File type is not supported!!\n\n";
	   				} //If-else block Added by Akshay
						ind++;

						//SG code to print time
						print_Current_Time();
   }

					if(strchr(param,'G')!=NULL){

						if(strchr(param,'M')!=NULL ) { 
					
							l->printDistrDB();
						} else {
					
							l->printDistr("Global");
						}
					}

}

Func * findFunc(int n,Func *f){
switch(n) {
        case 0: return new Soma_Surface(f);
        case 1: return new N_stems(f);
        case 2: return new N_bifs(f);
        case 3: return new N_branch(f);
        case 4: return new N_tips(f);
        case 5: return new Width(f);
        case 6: return new Height(f);
        case 7: return new Depth(f); 
        case 8: return new Type(f);
        case 9: return new Diameter(f);
	case 10: return new Diameter_pow(f);
        case 11: return new Length(f);
        case 12: return new Surface(f);
	case 13: return new SectionArea(f);
        case 14: return new Volume(f);
        case 15: return new EucDistance(f);
        case 16: return new PathDistance(f);
        case 17: return new Xyz(f);
        case 18: return new Branch_Order(f);
        case 19: return new Terminal_degree(f);
	case 20: return new TerminalSegment(f);
        case 21: return new Taper_1(f);
        case 22: return new Taper_2(f);
        case 23: return new Branch_pathlength(f);
        case 24: return new Contraction(f);
        case 25: return new Fragmentation(f);
        case 26: return new Daughter_Ratio(f);
        case 27: return new Parent_Daughter_Ratio(f);
        case 28: return new Partition_asymmetry(f);
        case 29: return new Rall_Power(f);
        case 30: return new Pk(f);
        case 31: return new Pk_classic(f);
        case 32: return new Pk_2(f);
        case 33: return new Bif_ampl_local(f);
        case 34: return new Bif_ampl_remote(f);
        case 35: return new Bif_tilt_local(f);
        case 36: return new Bif_tilt_remote(f);
        case 37: return new Bif_torque_local(f);
        case 38: return new Bif_torque_remote(f);
        case 39: return new Last_parent_diam(f);
        case 40: return new Diam_threshold(f);
        case 41: return new HillmanThreshold(f);
        case 42: return new Hausdorff(f);
        case 43: return new Helix(f);
        case 44: return new Fractal_Dim(f);
        case 99: return new N_Class(f);

       }

	return new Zero();
}

;

//SG code to print time
void print_Current_Time(){
	  time_t rawtime;
	  struct tm * timeinfo;

	  time(&rawtime);
	  timeinfo = localtime(&rawtime);
	  cerr<< "Current local time and date:" << asctime(timeinfo);

};

