#pragma once
#ifndef  _GETMODEL_
#define  _GETMODEL_
#pragma message("GetModel program") 
#include"GetInfo.h"
class GetModel
{
public:
	GetModel(GetInfo *g);
    void TrainModel();
	float ***PostFreq;		/* PostFreq[class][attribute][attribute_value],Used to store trained model*/
	float *SplitPoint;    //used to store continuous attribute discrete points value
private:
	void Cache();//Allocate memory for the model
	GetInfo *gi;
	
};
#endif