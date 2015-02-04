#ifndef DATASETINMETRICSPACE_H
#define DATASETINMETRICSPACE_H

#include"../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include"../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../HeaderFiles/metricdata/DoubleVector.h"

/** 
 *@ file  DatasetInMetricSpace.h
 *@ describe the training dataset in metric space
 *@ author  Ping Li
 *@ version  2014-06-13
 */

/**
 *@ class  CDatasetInMetricSpace
 *@ brief  calculate the distance between samples and pivots 
 *@ author  Ping Li
*/

class CDatasetInMetricSpace
{
public:
	/**@brief none parameter constructor*/
	CDatasetInMetricSpace();

	/**@brief destructor*/
	~CDatasetInMetricSpace();

	/**
	 *@ brief calculate the distance between samples and pivots
	 *@ data: data is the primary dataset
	 *@ pivots: pivots contains the pivot index in the dataset
	 *@ return: return an vector contains the distance between each sample and pivots 
	 */
	static vector<vector<double> > CalculateDistance(vector<shared_ptr<CMetricData> > *data,vector<int> pivots);
};

#endif