#ifndef NAVIEBAYES_H
#define NAVIEBAYES_H

#include "../../../HeaderFiles/classifier/Classifier.h"
#include "GetInfo.h"
#include "GetModel.h"
#include "PredictClass.h"
#include <string>

using namespace std;

/**
* @file NavieBayes.h
* @implementation classification and show classification result
* @author Ping Li
* @version 2014-8-12
*/

/**
* @class CNavieBayes
* @implementation classification and show classification result
* @author Ping Li
* @version 2014-8-12
*/

class CNavieBayes:public CClassifier
{
public:
	/**
     *@show classification result
     *@infilename: the training set under metric space that after pivot selection and distance calculation
	 *@testfilename: unclassified samples
	 *@testlabel: the true class label of unclassified samples
     *@outfilename: outfilename is going to store the classify result
     */
	static void showClassifierResult(char *infilename,char *testfilename,char *testlabel,const char* outfilename);
};

#endif