#pragma once
#ifndef  _PREDICTCLASS_
#define  _PREDICTCLASS_
#pragma message("PredictClass program") 
#include"GetInfo.h"
#include"GetModel.h"
#include"GetValue.h"
#include <algorithm>

#ifndef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#else
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#endif


class PredictClass
{
private:
	int Inc;
	char	Delimiter;
	GetInfo *gi;
	GetModel *gm;
	string filename;
	string AttFileName;
	void GetName();
	void GetData();
	bool  ReadName(FILE *f,char *Buffer);
	void  Error(int n, string s1, string s2);
	Description GetDescription(FILE *Df);
	int Which(string Val,string List[],short First,short Last);//determine the attribute value Val's position or class value in AttValName

	/* use to store the attributes and class information of all variables in .names file*/
	string *ClassName;//name of class
	string *AttName;//name of attribute
	string **AttValName;//name of attribute value
	short *MaxAttValNo;//the number of categorical attribute.  It starts at 1 deposit£¬0 position to deal with illegal in attribute values.If it's 0£¬said the property for continuous attributes, need discretization.
	short MaxClassNo;//the number of classes starting from 0, so the total number is MaxClassNo+1 
	short MaxAttNo;//the number of attributes starting from 0, so the total number is MaxAttNo+1
	short MaxDiscrValNo;//the maximum number of all discrete attributes value

    /* use to store all variables in .data file*/
    int MaxItemNo;//number of training set
	Description	*Item;
	void show(const char *resultFileName);
public:

	/*===================================================== new content£ºcalculate classification accuracy ======================================================================*/

	int TruePositive;
	int FalsePositive;
	double accuracy;

	/*===========================================================================================================================================================================*/

	PredictClass(GetInfo *i,GetModel *m,string name,string Attname);
	void Predict(const char *showResultFileName);
};
#endif