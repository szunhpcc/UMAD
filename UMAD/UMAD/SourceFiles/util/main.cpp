/**
* @file main
* @Entry of using this database.
* @author ***
* @date 2014 8 04
* @version 0.x
*
* this file contains the main interface to use this database.
*/

#define _CRT_SECURE_NO_WARNINGS

#if defined(_MSC_VER)
#include "../../HeaderFiles/util/getopt_win.h"
#include <tchar.h>
#else _MSC_VER
#include "../../HeaderFiles/util/getopt.h"
#define ARG_NONE no_argument
#define ARG_NULL no_argument
#define ARG_REQ required_argument
#define getopt_long_a getopt_long
#define optarg_a optarg
#define option_a option
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

using namespace std;
extern void resultfilename(int buildMode,char *dataFileName,int numPivot,int setE,int setC,int singlePivotFanout,int fftscale,char *pivotSelectionMethod,char *partitionMethod,int maxLeafSize,char *dataType,int initialSize,int finalSize,	int stepSize,bool bucket,int fragmentLength,int dim,double maxR,char *indexType,int fftopt,char *indexName,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,char *runOption,int searchMode,double MF_maxRadius,double MF_middleProportion);
extern void IndexFun(char *runOption,char *dataFileName,int numPivot,int setE,int setC,int singlePivotFanout,int fftscale,char *pivotSelectionMethod,char *partitionMethod,int maxLeafSize,char *dataType,int initialSize,int finalSize,int stepSize,bool bucket,int fragmentLength,int dim,double maxR,char *indexType,int fftopt,char *queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char resultsFileName[500],int buildMode,char *indexName,int searchMode,double MF_maxRadius,double MF_middleProportion,bool putinfo,int cacheHeight,int tsThreadNum,bool cacheAll,int bThreadNum,double trisectionRadius,char *selectOptimalPivots,char *partitionByOnePivot,int csThreadNum);		
extern void ClassificationFun(char *dataType,char *dataFileName,int finalSize,int dim,int numPivot,char *pivotSelectionMethod,char *disfun,char *testFileName,char *classifyMethod,char *dataFileNameInMetricSpace,char *showResultFileName,char *testDataSetLabel);




template<typename type>
type stringToNumber(const char str[])
{

	istringstream iss(str);
	type data;
	iss>>data;
	return data;
}

/**
* @This is the main entry point for building a MVP Index or do search in the index built before. It can build more than one index structure
* 
* The basic commandline options are:
* @param task the task which you will perform: "index/i","classificaiton/c","outliers/o".
* @param runO [build,search] option that will be done. Input "build" to build a index and input "search" if you want to do search in the index build before.
* @param n raw data file name. 
* @param v number of pivots in an index node.
* @param sete the size of evaluate set in the incremental pivot selection.
* @param setc the size of candidate set in the incremental pivot selection.
* @param f fanout of a pivot.
* @param fftscale use for pcaonfft pivot selection method.
* @param psm the pivot selection method: "random", "fft", "center", "pcaonfft", "pca","incremental".
* @param dpm data partition method: "balanced", "clusteringkmeans", "clusteringboundary","excludedmiddle","cght","ght".
* @param m maximum number of objects in leaf nodes.
* @param t data type, one of "peptide", "dna", "vector", "image", "msms","string".
* @param init size of smallest database.
* @param F size of largest database.
* @param s step size of databases.
* @param b whether bucketing will be used, 1: use.
* @param frag fragment length, only meaningful for sequences.
* @param dim dimension of vector data to load or number of features to be loaded.
* @param r maximum radius for partition.
* @param e [ListIndex,MVPIndex],the type of the index.
* @param fftopt 0:use old version FFT, 1: use new version FFT but don't choose the third point got from FFT as the second pivot, 2:use new version FFT and choose the third point got from FFT as the second pivot. Default 0. 
* @param indexN name of file storing the index structure built before. 
* @param q query file name. 
* @param fq offset of first query to be used in the query file, start from 0, inclusive, default 0. 
* @param l offset of last query to be used in the query file, exclusive, default 1. 
* @param a maximum search radius, default 10. 
* @param i minimum search radius, default 0. 
* @param y step size for search radii, default 1. 
* @param ver 1 if search results are to be verified against a linear scan and 0 otherwise, default 0. 
* @param res name of the file to store the result information of the search operation. 

***classification parameters
* @param W the distance function: "EuclideanDistance" , "EditDistance" , "LInfinityDistance".
* @param w testfilename for classification.
* @param j classifymethod: "knn" , "naviebayes".
* @param K the training data in metric space
* @param k classification result filename
* @param N store the true classlabel of testdata
*
*/

int main(int argc, char** argv)
{
	char *task = "index";

	int c;

	int buildMode = 0;

	char *dataFileName="uniformvector-20dim-1m.txt";

	int numPivot=2;

	int setE = 10000;

	int setC = 50;

	int singlePivotFanout=3;

	int fftscale=100;

	char *pivotSelectionMethod="fft";

	char *partitionMethod="PivotWise";
	char *partitionByOnePivot="Balance";
	char *selectOptimalPivots="Sequence";
	double trisectionRadius=0.25;

	int maxLeafSize=100;

	char *dataType="vector";

	int initialSize = 20000;

	int finalSize = 20000;	

	int stepSize = 20000;	

	bool bucket = false;

	int fragmentLength=6;

	int dim=20;

	double maxR=1;

	char *indexType="MVPIndex";

	int fftopt=0;

	char *indexName="vector_index_PivotWise";

	char* queryFileName="uniformvector-20dim-1m.txt";

	int firstQuery=0;

	int lastQuery=1;

	double maxRadius= 2;

	double minRadius= 0;

	double step=0.2;

	bool verify=false;

	//char *resultsFileName="resultsfile.txt";

	char resultsFileName[500];

	strcpy(resultsFileName,"result/");

	char *runOption="buildandsearch";

	int searchMode=1;

	double MF_maxRadius = 2.0;

	double MF_middleProportion = 0.5; 

	bool putinfo = true;

	int cacheHeight=3;

	bool cacheAll=0;

	int tsThreadNum=10;//number of maximum parallel searching threads --tsThreadNum
	int csThreadNum=7;//number of children path searching threads --csThreadNum

	int bThreadNum=32;//for parallel build --maxThreadNum
	
	//***classification parameters
	char *disfun="EuclideanDistance";

	char *testFileName="diabetes-test.data";
	
	char *classifyMethod="knn";

	char *dataFileNameInMetricSpace="metricDistance.data";

	char *showResultFileName="OutPut.txt";

	char *testDataSetLabel="iristestlabel.data";

	while (1)
	{		

		static struct option_a long_options[] =
		{
			{"task", ARG_REQ, 0 , 'M'},

			{"maxR", ARG_REQ, 0 , 'Y'},

			{"midP", ARG_REQ, 0 , 'Z'},

			{"r",    ARG_REQ, 0, 'a'},

			{"buildMode",    ARG_REQ, 0, 'A'},

			{"b", ARG_REQ,0,'b'},

			{"indexName", ARG_REQ,0,'B'},

			{"searchMode", ARG_REQ,0,'c'},

			{"tR", ARG_REQ,0,'C'},

			{"n",    ARG_REQ, 0 , 'd'},

			{"t",    ARG_REQ, 0 , 'D'},

			{"e",    ARG_REQ, 0 , 'e'},

			{"fftscale", ARG_REQ, 0, 'f'},

			{"F",    ARG_REQ, 0 , 'F'},

			{"frag",    ARG_REQ, 0 , 'g'},

			{"sop",    ARG_REQ, 0 , 'h'},
			{"pbop",    ARG_REQ, 0 , 'H'},

			{"init",    ARG_REQ, 0 , 'i'},

			{"dim",    ARG_REQ, 0 , 'I'},

			{"l",    ARG_REQ, 0 , 'l'},

			{"res",    ARG_REQ, 0 , 'L'},

			{"m",    ARG_REQ, 0 , 'm'},

			{"v",    ARG_REQ, 0 , 'n'},

			{"psm",    ARG_REQ, 0 , 'p'},

			{"dpm",    ARG_REQ, 0 , 'P'},

			{"q",    ARG_REQ, 0 , 'q'},

			{"runOption",ARG_REQ, 0,'r'},

			{"fq",  ARG_REQ, 0 , 'R'},

			{"sete",    ARG_REQ, 0 , 's'},

			{"setc",    ARG_REQ, 0 , 'S'},

			{"s",    ARG_REQ, 0 , 't'},

			{"fftopt",     ARG_REQ, 0, 'T'},

			{"i",    ARG_REQ, 0 , 'U'},

			{"ver",    ARG_REQ, 0 , 'v'},

			{"a",    ARG_REQ, 0 , 'x'},

			{"y",    ARG_REQ, 0 , 'y'},

			{"f",    ARG_REQ, 0 , 'z'},

			{"putinfo",    ARG_REQ, 0 , 'O'},

			{"bThreadNum",   ARG_REQ, 0 , 'G'},

			{"cacheAll",  ARG_REQ, 0 , 'X'},

			{"cacheHeight", ARG_REQ, 0 , 'E'},

			{"tsThreadNum",   ARG_REQ, 0 , 'u'},

			{"csThreadNum",   ARG_REQ, 0 , 'J'},

			{"disfun", ARG_REQ, 0, 'W'},

			{"testFileName", ARG_REQ, 0, 'w'},

			{"classifyMethod", ARG_REQ, 0, 'j'},

			{"dFNIMS", ARG_REQ, 0, 'K'},

			{"sRFN", ARG_REQ, 0, 'k'},

			{"tDSL", ARG_REQ, 0, 'N'}

			/*{ ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }*/

		};



		int option_index = 0;

		c = getopt_long_a(argc, argv, ("M:a:A:b:B:c:C:d:D:e:f:F:g:h:H:i:I:l:L:m:n:p:P:q:r:R:s:S:t:T:U:v:x:y:z:Y:Z:G:O:X:E:u:J:W:w:j:K:k:N"), long_options, &option_index); //o,Q,V no use



		// Check for end of operation or error

		if (c == -1)

			break;



		switch (c)
		{

		case 0:

			/* If this option set a flag, do nothing else now. */

			if (long_options[option_index].flag != 0)

				break;

			printf (("option %s"), long_options[option_index].name);

			if (optarg_a)

				printf ((" with arg %s"), optarg_a);

			printf (("\n"));

			break;

		case('M'):

			task = new char[strlen(optarg_a)+1];

			strcpy(task, optarg_a);

			printf(("option -task with value `%s'\n"), task);

			break;

		case ('a'):	

			maxR = stringToNumber<double>(optarg_a);

			cout<<"option -maxR with value " << maxR << endl;

			break;

		case ('A'):	

			buildMode = stringToNumber<int>(optarg_a);

			cout<<"option -buildMode with value " << buildMode << endl;

			break;

		case ('b'):	

			bucket = stringToNumber<bool>(optarg_a);

			cout<<"option -bucket with value " << bucket << endl;

			break;

		case ('B'):	

			indexName = new char[strlen(optarg_a)+1];

			strcpy(indexName,optarg_a);

			cout<<"option -indexName with value " << indexName << endl;

			break;

		case ('c'):	

			searchMode = stringToNumber<int>(optarg_a);

			cout<<"option -searchMode with value " << searchMode << endl;

			break;

		case ('C'):	

			trisectionRadius = stringToNumber<double>(optarg_a);

			cout<<"option -trisectionRadius with value " << trisectionRadius << endl;

			break;

		case ('d'):	

			dataFileName = new char[strlen(optarg_a)+1];

			strcpy(dataFileName,optarg_a);

			printf (("option -dataFileName with value `%s'\n"), dataFileName);

			break;

		case ('D'):	

			dataType = new char[strlen(optarg_a)+1];

			strcpy(dataType,optarg_a);

			printf (("option -dataType with value `%s'\n"), dataType);

			break;

		case ('e'):	

			indexType = new char[strlen(optarg_a)+1];

			strcpy(indexType,optarg_a);

			printf (("option -listType with value `%s'\n"), indexType);

			break;

		case ('f'):	

			fftscale = stringToNumber<int>(optarg_a);

			printf (("option -fftscale with value `%d'\n"), fftscale);

			break;

		case ('F'):	

			finalSize = stringToNumber<int>(optarg_a);

			printf (("option -finalSize with value `%d'\n"), finalSize);

			break;

		case ('g'):	

			fragmentLength = stringToNumber<int>(optarg_a);

			printf (("option -fragmentLength with value `%d'\n"), fragmentLength);

			break;

		case ('h'):	

			selectOptimalPivots = new char[strlen(optarg_a)+1];

			strcpy(selectOptimalPivots,optarg_a);

			printf (("option -selectOptimalPivots with value `%s'\n"), selectOptimalPivots);

			break;

		case ('H'):	

			partitionByOnePivot = new char[strlen(optarg_a)+1];

			strcpy(partitionByOnePivot,optarg_a);

			printf (("option -partitionByOnePivot with value `%s'\n"), partitionByOnePivot);

			break;

		case ('i'):	

			initialSize = stringToNumber<int>(optarg_a);

			printf (("option -initialSize with value `%d'\n"), initialSize);

			break;

		case ('I'):	

			dim=stringToNumber<int>(optarg_a);

			printf (("option -dim with value `%d'\n"), dim);

			break;

		case ('l'):	

			lastQuery=stringToNumber<int>(optarg_a);

			printf (("option -lastQuery with value `%d'\n"), lastQuery);

			break;

		case ('L'):	

			//resultsFileName=new char[strlen(optarg_a)+1];

			strcpy(resultsFileName,optarg_a);

			printf (("option -resultsFileName with value `%s'\n"), resultsFileName);

			break;

		case ('m'):	

			maxLeafSize=stringToNumber<int>(optarg_a);

			printf (("option -maxLeafSize with value `%d'\n"), maxLeafSize);

			break;

		case ('n'):	

			numPivot=stringToNumber<int>(optarg_a);

			printf (("option -numPivot with value `%d'\n"), numPivot);

			break;

		case ('p'):	

			pivotSelectionMethod = new char[strlen(optarg_a)+1];

			strcpy(pivotSelectionMethod,optarg_a);

			printf (("option -pivotSelectionMethod with value `%s'\n"), pivotSelectionMethod);

			break;

		case ('P'):	

			partitionMethod = new char[strlen(optarg_a)+1];

			strcpy(partitionMethod,optarg_a);

			printf (("option -partitionMethod with value `%s'\n"), partitionMethod);

			break;

		case ('q'):	

			queryFileName = new char[strlen(optarg_a)+1];

			strcpy(queryFileName,optarg_a);

			printf (("option -queryFileName with value `%s'\n"), queryFileName);

			break;

		case ('r'):	

			runOption = new char[strlen(optarg_a)+1];

			strcpy(runOption,optarg_a);

			printf (("option -runOption with value `%s'\n"), optarg_a);

			break;

		case ('R'):	

			firstQuery = stringToNumber<int>(optarg_a);

			printf (("option -firstQuery with value `%d'\n"), firstQuery);

			break;

		case ('s'):	

			setE = stringToNumber<int>(optarg_a);

			printf (("option -setE with value `%d'\n"), setE);

			break;

		case ('S'):	

			setC = stringToNumber<int>(optarg_a);

			printf (("option -setC with value `%d'\n"), setC);

			break;

		case ('t'):	

			stepSize = stringToNumber<int>(optarg_a);

			printf (("option -stepSize with value `%d'\n"), stepSize);

			break;

		case ('T'):	

			fftopt = stringToNumber<int>(optarg_a);

			printf (("option -fftopt with value `%d'\n"), fftopt);

			break;

		case ('U'):	

			minRadius = stringToNumber<double>(optarg_a);

			printf (("option -minRadius with value `%f'\n"), minRadius);

			break;

		case ('v'):	

			verify = stringToNumber<bool>(optarg_a);

			printf (("option -verify with value `%d'\n"), verify);

			break;

		case ('x'):	

			maxRadius = stringToNumber<float>(optarg_a);

			printf (("option -maxRadius with value `%f'\n"), maxRadius);

			break;
		case ('O'):	
			putinfo = stringToNumber<bool>(optarg_a);
			printf (("option -putinfo with value `%d'\n"), putinfo);
			break;

		case ('y'):	

			step=stringToNumber<double>(optarg_a);

			printf (("option -step with value `%f'\n"), step);

			break;

		case ('z'):	

			singlePivotFanout=stringToNumber<int>(optarg_a);

			printf (("option -singlePivotFanout with value `%d'\n"), singlePivotFanout);

			break;

		case ('Y'):

			MF_maxRadius = stringToNumber<double>(optarg_a);

			cout<<"option -MF_maxRadius with value " << MF_maxRadius << endl;

			break;

		case ('Z'):	

			MF_middleProportion = stringToNumber<double>(optarg_a);

			cout<<"option -MF_middleProportion with value " << MF_middleProportion << endl;

			break;
		case ('G'):
			bThreadNum = stringToNumber<int>(optarg_a);
			cout<<"option -maxThreadNum with value "<<bThreadNum<<endl;

			break;
		case ('X'):
			cacheAll=stringToNumber<bool>(optarg_a);
			cout<<"option -cacheAll with value "<<cacheAll <<endl;

			break;
		case ('E'):
			cacheHeight = stringToNumber<int>(optarg_a);
			cout<<"option -cacheHeight with value "<<cacheHeight<<endl;

			break;
		case ('u'):
			tsThreadNum = stringToNumber<int>(optarg_a);
			cout<<"option -tsthreadNum with value "<< tsThreadNum <<endl;

			break;
		case ('J'):
			csThreadNum = stringToNumber<int>(optarg_a);
			cout<<"option -csthreadNum with value "<< csThreadNum <<endl;

			break;

		case ('W'):	

			disfun = new char[strlen(optarg_a)+1];

			strcpy(disfun,optarg_a);

			printf (("option -disfun with value `%s'\n"), disfun);

			break;

		case ('w'):	

			testFileName = new char[strlen(optarg_a)+1];

			strcpy(testFileName,optarg_a);

			printf (("option -testFileName with value `%s'\n"), testFileName);

			break;
			
		case ('j'):	

			classifyMethod = new char[strlen(optarg_a)+1];

			strcpy(classifyMethod,optarg_a);

			printf (("option -classifyMethod with value `%s'\n"), classifyMethod);

			break;
				
		case ('K'):	

			dataFileNameInMetricSpace = new char[strlen(optarg_a)+1];

			strcpy(dataFileNameInMetricSpace,optarg_a);

			printf (("option -dataFileNameInMetricSpace with value `%s'\n"), dataFileNameInMetricSpace);

			break;

		case ('k'):	

			showResultFileName = new char[strlen(optarg_a)+1];

			strcpy(showResultFileName,optarg_a);

			printf (("option -showResultFileName with value `%s'\n"), showResultFileName);

			break;

		case ('N'):	

			testDataSetLabel = new char[strlen(optarg_a)+1];

			strcpy(testDataSetLabel,optarg_a);

			printf (("option -testDataSetLabel with value `%s'\n"), testDataSetLabel);

			break;


		case '?':

			/* getopt_long already printed an error message. */

			break;



		default:

			abort();

		}

	}

	if (optind < argc)
	{

		printf (("non-option ARGV-elements: "));

		while (optind < argc) printf (("%s "), argv[optind++]);

		printf (("\n"));

	}


	if(strcmp(task,"index")==0 || strcmp(task,"i")==0)
	{		

		resultfilename(buildMode,dataFileName,numPivot,setE,setC,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,indexName,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,runOption,searchMode,MF_maxRadius,MF_middleProportion);

		IndexFun(runOption,dataFileName,numPivot,setE,setC,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,MF_maxRadius,MF_middleProportion,putinfo,cacheHeight,tsThreadNum,cacheAll,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot,csThreadNum);
	
	}
	else if(strcmp(task,"classification")==0 || strcmp(task,"c")==0)
	{
		
		ClassificationFun(dataType,dataFileName,finalSize,dim,numPivot,pivotSelectionMethod,disfun,testFileName,classifyMethod,dataFileNameInMetricSpace,showResultFileName,testDataSetLabel);
	
	}
	else if(strcmp(task,"outliers")==0 || strcmp(task,"o")==0)
	{



	}	
	else
	{

		printf(("\n"));

		printf(("you must configure the tese parameter of task : index , classification or outlier."));
		
		printf(("\n"));

		printf(("please return and reset the test parameters."));

	}
	return 0;

}