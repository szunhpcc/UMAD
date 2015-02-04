#include"../../HeaderFiles/metricdata/MetricData.h"

#include"../../HeaderFiles/metricdistance/MetricDistance.h"
#include"../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include"../../HeaderFiles/metricdistance/EditDistance.h"
#include "../../HeaderFiles/metricdistance/LInfinityDistance.h"
//#include "../../HeaderFiles/metricdistance/CountedMetric.h"
//#include "../../HeaderFiles/metricdistance/DNAMetric.h"
//#include "../../HeaderFiles/metricdistance/ImageMetric.h"
//#include "../../HeaderFiles/metricdistance/MSMSMetric.h"
//#include "../../HeaderFiles/metricdistance/PeptideMetric.h"
//#include "../../HeaderFiles/metricdistance/RNAMetric.h"

#include"../../HeaderFiles/index/PivotSelectionMethod.h"
#include"../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include"../../HeaderFiles/index/PCAOnFFT.h"
//#include"../../HeaderFiles/index/PCA.h"
#include"../../HeaderFiles/index/IncrementalSelection.h"

#include"../../HeaderFiles/classifier/knn.h"
#include"../../HeaderFiles/classifier/NavieBayes/NavieBayes.h"
#include"../../HeaderFiles/classifier/NavieBayes/GetInfo.h"
#include"../../HeaderFiles/classifier/NavieBayes/GetModel.h"
#include"../../HeaderFiles/classifier/NavieBayes/PredictClass.h"

#include"../../HeaderFiles/classifier/DatasetInMetricSpace.h"
#include"../../HeaderFiles/classifier/ReadTrainLabel.h"


#include<memory>
#include<iostream>
#include<vector>
using namespace std;

extern void joinCharArray(char*&dest,char *sor);

void ClassificationFun(char *dataType,char *dataFileName,int finalSize,int dim,int numPivot,char *pivotSelectionMethod,char *disfun,char *testFileName,char *classifyMethod,char *dataFileNameInMetricSpace,char *showResultFileName,char *testDataSetLabel)
{  

	char *newDataFileName="SourceFiles/util/data/";

	joinCharArray(newDataFileName,dataFileName);

	char *newTestFileName="SourceFiles/util/data/";

	joinCharArray(newTestFileName,testFileName);

	char *newDataFileNameInMetricSpace="SourceFiles/util/data/";

	joinCharArray(newDataFileNameInMetricSpace,dataFileNameInMetricSpace);

	char *newShowResultFileName="SourceFiles/util/result/";

	joinCharArray(newShowResultFileName,showResultFileName);

	char *newTestDataSetLabel="SourceFiles/util/data/";

	joinCharArray(newTestDataSetLabel,testDataSetLabel);

	/*CMetricData *trainclassifier=0;

	if(strcmp(dataType,"vector")==0)
	{
		trainclassifier = new CDoubleVector;
	}*/
	
	CDoubleVector *trainclassifier;

	CMetricDistance *metric=0;

	if(strcmp(disfun,"EuclideanDistance")==0)
	{
		metric = new CEuclideanDistance;
	}
	else if(strcmp(disfun,"EditDistance")==0)
	{
		metric = new CEditDistance;
	}
	else if(strcmp(disfun,"LInfinityDistance")==0)
	{
		metric = new CLInfinityDistance;
	}
	


	CPivotSelectionMethod *pivotselectionmethod=0;

	if(strcmp(pivotSelectionMethod,"fft")==0)
	{
		pivotselectionmethod = new CFFTPivotSelectionMethod;
	}
	else if(strcmp(pivotSelectionMethod,"pcaonfft")==0)
	{
		pivotselectionmethod = new CPCAOnFFT(2);
	}
	else if(strcmp(pivotSelectionMethod,"incremental")==0)
	{
		pivotselectionmethod = new CIncrementalSelection(1,2);
	}
	else if(strcmp(pivotSelectionMethod,"random")==0)
	{

	}
	else if(strcmp(pivotSelectionMethod,"center")==0)
	{

	}
	/*else if(strcmp(pivotSelectionMethod,"pca"))
	{
		pivotselectionmethod = new CPCA;
	}*/

	
	/**
     *tep 1 : loaddata
	 */
	vector<shared_ptr<CMetricData> > *traindata; //store the primary traindata
	CReadTrainLabel readtraindatalabel;
	map<int,string> datalabel; //store the traindata label
	traindata=trainclassifier->loadData(newDataFileName,finalSize,dim);              
	datalabel=readtraindatalabel.loadLabel(newDataFileName,finalSize+1);

	/**
	 *step 2 : select pivot and calculate the distance between samples and pivots
	 */
	vector<int> pivotsindex;  //store the index of the pivots
	pivotsindex=pivotselectionmethod->selectPivots(metric,*traindata,numPivot);

	vector<vector<double> > dataset;  //store the calculate result
	CDatasetInMetricSpace datasetInMetric;
	dataset=datasetInMetric.CalculateDistance(traindata,pivotsindex);

	int pivotnum=pivotsindex.size();

	/**
	 *setp 3 :combine dataset with map(id_classlabel) and write it to hard disk
	 */
	ofstream outfile(newDataFileNameInMetricSpace,ofstream::out);
	for(int i=0; i<dataset.size(); ++i)
	{
		for(map<int,string>::iterator it=datalabel.begin(); it!=datalabel.end(); ++it)
		{
			if(i==it->first)
			{
				for(int k=0; k<pivotnum; ++k)
				{
					outfile<<dataset[i][k]<<",";
				}
				outfile<<it->second<<endl;
				break;
			}
		}
	}
	outfile.close();

	/**
	 *setp 4 : using classification algorithm training classifier and test
	 */
	if(strcmp(classifyMethod,"knn")==0)
	{
		cKnn knn;
		knn.showClassifierResult(newDataFileNameInMetricSpace,newShowResultFileName);
	}
	else if(strcmp(classifyMethod,"naviebayes")==0)
	{
		CNavieBayes NavieBayes;
		NavieBayes.showClassifierResult(newDataFileNameInMetricSpace,newTestFileName,newTestDataSetLabel,newShowResultFileName);
	}
} 