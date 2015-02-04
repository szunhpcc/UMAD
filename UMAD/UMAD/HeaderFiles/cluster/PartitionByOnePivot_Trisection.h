#ifndef PARTITIONBYONEPIVOT_TRISECTION_H
#define PARTITIONBYONEPIVOT_TRISECTION_H

#include "PartitionByOnePivot.h"
#include "../metricdistance/MetricDistance.h"
#include "util/ClusteringTask.h"

/** @file	partitionByOnePivot_Trisection.h
* @classes	Inherited partitionByOnePivot
* @author	He Zhang
* @version	2014-07-23
*/

/**
* @class	PartitionByOnePivot_Balance
* @brief	Inherited partitionByOnePivot,According to the user's parameters r ,the data points is divided into three parts
* @author	He Zhang
*/

class PartitionByOnePivot_Trisection : public PartitionByOnePivot 
{
public:
	/**@brief Have a parameter constructor
	*@param trisectionRadius	The radius of the designated by the user
	*/
	PartitionByOnePivot_Trisection(double trisectionRadius);
	/**@brief	According to the user's parameters r ,the data points is divided into three parts.meanwhile Repeat points also need to consider
	*@param	task	To divide the task
	*@param	metric	The data types of distance calculation function
	*@param	numpartition	The task is divided into several parts
	*@param	pivot	The pivot is going to be used
	*@param	indictor	Instructions which the pivot is used, is also used as a labeled data class label
	*@param	cluster_Start	Points out that the starting value of class labels
	*@param	cluster_end	Points out that the end value of the class label
	*/
	virtual void partitionByOnePivot(shared_ptr<ClusteringTask>&task,CMetricDistance *metric,int& numpartition,shared_ptr<CMetricData>& pivot,int indictor,int& cluster_Start,int& cluster_end);

private:
	/** The radius of the designated by the user*/
	double trisectionRadius;
};

#endif