#include<cstring>
#include<sstream>

using namespace std;


void resultfilename_1(char *resultsFileName,char *dataType,char *method){
	strcpy(resultsFileName,method);
	strcat(resultsFileName,dataType);
	strcat(resultsFileName,"_");
}

void resultfilename_3(char *resultsFileName,int num,char *str){
	char intTemp[32]="";
	stringstream ssTemp;
	string strTemp;

	strcat(resultsFileName,str);
	memset(intTemp,'\0',sizeof(intTemp));
	ssTemp.str("");
	ssTemp.clear();
	ssTemp<<num;
	ssTemp>>strTemp;
	strcat(intTemp,strTemp.c_str());
	strcat(resultsFileName,intTemp);
}

void resultfilename_2(char *resultsFileName,char *dataType,int finalSize,int dim,int numPivot,int singlePivotFanout,int buildMode){
	char intTemp[32]="";
	stringstream ssTemp;
	string strTemp;
	resultfilename_3(resultsFileName,finalSize,"datasize_") ;
	strcat(resultsFileName,"_");
	resultfilename_3(resultsFileName,dim,"dim_") ;
	strcat(resultsFileName,"_");
	resultfilename_3(resultsFileName,numPivot,"numPivot_") ;
	strcat(resultsFileName,"_");
	resultfilename_3(resultsFileName,singlePivotFanout,"singlePivotFanout_") ;
	strcat(resultsFileName,"_");
	resultfilename_3(resultsFileName,buildMode,"buildMode_") ;

}

void resultfilename(int buildMode,char *dataFileName,int numPivot,int setA,int setN,int singlePivotFanout,int fftscale,char *pivotSelectionMethod,char *partitionMethod,int maxLeafSize,char *dataType,int initialSize,int finalSize,	int stepSize,bool bucket,int fragmentLength,int dim,double maxR,char *indexType,int fftopt,char *indexName,char* queryFileName,int firstQuery,int lastQuery,double maxRadius,double minRadius,double step,bool verify,char *resultsFileName,char *runOption,int searchMode,double MF_maxRadius,double MF_middleProportion)
{
	if(strcmp(runOption,"build")==0)
	{
		resultfilename_1(resultsFileName,dataType,"build_") ;
		resultfilename_2(resultsFileName,dataType,finalSize,dim,numPivot,singlePivotFanout,buildMode) ;
	}
	else if(strcmp(runOption,"search")==0)
	{
		resultfilename_1(resultsFileName,dataType,"search_") ;
		resultfilename_3(resultsFileName,searchMode,"searchMode_") ;
	}
	else
	{
		resultfilename_1(resultsFileName,dataType,"buildAndSearch_") ;
		resultfilename_2(resultsFileName,dataType,finalSize,dim,numPivot,singlePivotFanout,buildMode) ;
		strcat(resultsFileName,"_");
		resultfilename_3(resultsFileName,searchMode,"searchMode_") ;
	}
	//________________________________________________________
	strcat(resultsFileName,".txt");
}