#ifndef READTRAINLABEL_H
#define READTRAINLABEL_H

#include "../../HeaderFiles/metricdata/MetricData.h"
#include <fstream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

/** 
 *@ file	readtrainlabel.h
 *@ classes	store data number and the class label of train data
 *@ author	Ping Li
 *@ version	2014-08-01
 */

/**
 *@ class	CReadTrainLabel
 *@ brief	store data number and the class label of train data
 *@ author	Ping Li
 */

class CReadTrainLabel:public CMetricData
{
public:

/**
 *@ store data number and the class label of train data
 *@ param fileName: fileName is the primary data file that store attribute and class label
 *@ param maxDataNum: the total number of the data list
 */
	static map<int,string> loadLabel(string fileName,int maxDataNum);
};

#endif