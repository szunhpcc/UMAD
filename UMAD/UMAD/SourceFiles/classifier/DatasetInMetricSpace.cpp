#include"../../HeaderFiles/classifier/DatasetInMetricSpace.h"

/**@brief none parameter constructor*/
CDatasetInMetricSpace::CDatasetInMetricSpace()
{
	
}

/**@brief destructor*/
CDatasetInMetricSpace::~CDatasetInMetricSpace()
{
	
}

/**
 *@ brief calculate the distance between samples and pivots
 *@ data: data is the primary dataset
 *@ pivots: pivots contains the pivot index in the dataset
 *@ return: return an vector contains the distance between each sample and pivots 
 */
vector<vector<double> > CDatasetInMetricSpace::CalculateDistance(vector<shared_ptr<CMetricData> > *data,vector<int> pivots)
{
	int pivotnum=pivots.size();
	vector<double> distance;
	CEuclideanDistance euclideandistance;

	vector<vector<double> > results;

	for(decltype(data->size()) i=0;i<data->size();++i)
	{
		shared_ptr<CDoubleVector> temp=dynamic_pointer_cast<CDoubleVector>(data->at(i));
		for(auto j=pivots.begin(); j!=pivots.end(); ++j)
		{
			shared_ptr<CDoubleVector> temp2=dynamic_pointer_cast<CDoubleVector>(data->at(*j));
			distance.push_back(euclideandistance.getDistance(temp->getData(),temp2->getData(),4));
		}
		results.push_back(distance);
		distance.clear();
	}
	return results;
}
