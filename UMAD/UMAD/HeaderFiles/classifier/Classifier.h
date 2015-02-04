#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
using namespace std;
/**
 * @file Classifier.h

 * @implementation of different classification methods.

 * @author Ping Li

 * @version 2014-08-11

 * this is a common interface of different classification methods.
*/

/**
 * @class CClassifier

 * @implementation of different classification methods.

 * @author Ping Li

 * this is a common interface of different classification methods.
*/

class CClassifier
{
public:

	/**none parameter constructor*/
	CClassifier();

	/**destructor*/
	~CClassifier();

	/**
     *@show classification result
     *@infilename: the training set under metric space that after pivot selection and distance calculation
     *@outfilename: outfilename is going to store the classify result
     */
	virtual void showClassifierResult(char *infilename,const char* outfilename);
};

#endif