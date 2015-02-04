/** @file BuildORQueryIndex
* @contains the main interface to bulid or query index.
* @author ***
* @date 2013 5 21
* @version 0.x
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

extern void buildAndSearch(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int buildMode,char *indexName,int searchMode,double MF_maxRadius,double MF_middleProportion,bool putInfo,int cacheHeight,int sThreadNum,bool cacheAll,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot,int numberOfChildrenPathSearchingThreads);
extern void batchBulkLoad(char *&dataFileName,int &numPivot,int setA,int setN,int &singlePivotFanout,int fftscale,char *&pivotSelectionMethod,char *&partitionMethod,int &maxLeafSize,char *&dataType,int initialSize,int finalSize,int stepSize,bool bucket,int &fragmentLength,int dim,double maxR,char *&indexType,int fftOpt,char *buildingInformationFile,int buildMode,char *indexName,double MF_maxRadius,double MF_middleProportion,bool putinfo,int bThreadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);
extern void searchIndex(int dataBasehSize,char* queryFileName,char *dataType,int firstQuery,int lastQuery,int dim,int fragmentLength,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,int buildMode,char *indexName,int searchMode,int cacheHeight,int sThreadNum,bool cacheAll,int numberOfChildrenPathSearchingThreads);

void IndexFun(char *runOption,char *dataFileName,int numPivot,int setA,int setN,int singlePivotFanout,int fftscale,char *pivotSelectionMethod,char *partitionMethod,int maxLeafSize,char *dataType,int initialSize,int finalSize,int stepSize,bool bucket,int fragmentLength,int dim,double maxR,char *indexType,int fftopt,char *queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char resultsFileName[500],int buildMode,char *indexName,int searchMode,double MF_maxRadius,double MF_middleProportion,bool putinfo,int cacheHeight,int tsThreadNum,bool cacheAll,int bThreadNum,double trisectionRadius,char *selectOptimalPivots,char *partitionByOnePivot,int csThreadNum)		
{
	if(strcmp(runOption,"buildandsearch")==0)
	{
		buildAndSearch(dataFileName,numPivot,setA,setN,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,queryFileName,firstQuery,lastQuery,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,MF_maxRadius,MF_middleProportion,putinfo,cacheHeight,tsThreadNum,cacheAll,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot,tsThreadNum);
	}
	else if(strcmp(runOption,"build")==0)
	{
		batchBulkLoad(dataFileName,numPivot,setA,setN,singlePivotFanout,fftscale,pivotSelectionMethod,partitionMethod,maxLeafSize,dataType,initialSize,finalSize,stepSize,bucket,fragmentLength,dim,maxR,indexType,fftopt,resultsFileName,buildMode,indexName,MF_maxRadius,MF_middleProportion,putinfo,bThreadNum,trisectionRadius,selectOptimalPivots,partitionByOnePivot);
	}
	else if(strcmp(runOption,"search")==0)
	{
		searchIndex(initialSize,queryFileName,dataType,firstQuery,lastQuery,dim,fragmentLength,maxRadius,minRadius,step,verify,resultsFileName,buildMode,indexName,searchMode,cacheHeight,tsThreadNum,cacheAll,csThreadNum);
	}
}

