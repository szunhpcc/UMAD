#pragma once
#ifndef  _GETINFO_
#define  _GETINFO_
#pragma message("GetInfo program") 
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <memory>
#include <sstream>
#include <algorithm>

#ifndef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#else
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0
#endif

using namespace std;
#include<math.h>
#include"GetValue.h"
#define	 Log(x)	 ((x) <= 0 ? 0.0 : log((float)x) / log(2.0))
class GetInfo
{
public:
	void getname();
	void getdata();
	float SplitContinuousAtt(int AttNo);//continuous feature discretization
	GetInfo(string name,string label):filename(name),testlabel(label),MaxClassNo(-1),MaxDiscrValNo(2),Inc(1024),MaxAttNo (-1),Epsilon(1E-3),MINOBJS(2),None(-1),MaxContAttNo(-1){}
	void  Error(int n, string s1, string s2);

	/*use to store the attributes and class information of all variables in .names file */
	string *ClassName;//name of class
	string *AttName;//name of attribute
	string **AttValName;//name of attribute value
	short *MaxAttValNo;//the number of categorical attribute.  It starts at 1 deposit£¬0 position to deal with illegal in attribute values.If it's 0£¬said the property for continuous attributes, need discretization.
	char  *SpecialStatus;
	short MaxClassNo;//the number of classes starting from 0, so the total number is MaxClassNo+1  
	short MaxAttNo;//the number of attributes starting from 0, so the total number is MaxAttNo+1
	short MaxDiscrValNo;//the maximum number of all discrete attributes value

    /* use to store all variables in .data file*/
    int MaxItemNo;//number of training set
	Description	*Item;
	float
	*Weight,	/* Weight[i]  = current fraction of item i */
	**Freq,		/* Freq[x][c] = no. items of class c with outcome x  x is attribute value£¬c is class value*/
	*ValFreq,	/* ValFreq[x]   = no. items with outcome x */
	*ClassFreq;	/* ClassFreq[c] = no. items of class c */
	int MaxContAttNo;//the number of continuous attributes

	//============================================================================ new content=====================================================================

	string testlabel;
	vector<string> testdatalabel;  //the true classlabel of test set

	//=============================================================================================================================================================

private:
	FILE *Nf;
	string filename;
	bool ReadName(FILE *f,char *Buffer);
	char	Delimiter;
	int Inc;
	Description GetDescription(FILE *Df);
	int Which(string Val,string List[],short First,short Last);//determine the attribute value Val's position or class value in AttValName
	void AnalyzeData();//Statistics of attribute value or class in number
	void InitialiseData();//allocate space for the data
	void InitialiseWeights();//initialize each Items weight
	void ResetFreq(float MaxDiscrValNo);//initialize frequency£¨0£©
	void Quicksort(int ItemNoFp,int ItemNoLp ,int AttNo);//quicksort for continuous attributes
	void Swap(int ItemA,int ItemB);
	const float Epsilon ;
	const int MINOBJS;
	const int None;
	float
	*SplitGain,	/* SplitGain[i] = gain with att value of item i as threshold */
	*SplitInfo;	/* SplitInfo[i] = potential info ditto */
	float TotalInfo(float V[],int MinVal,int MaxVal);
	float ComputeGain(float BaseInfo,int MaxValNo,float TotalItems);
	float
	*Gain,		/* Gain[a] = info gain by split on att a */
	//*Info,		/* Info[a] = potential info of split on att a */
	*Bar;		/* Bar[a]  = best threshold for contin att a */
};
#endif