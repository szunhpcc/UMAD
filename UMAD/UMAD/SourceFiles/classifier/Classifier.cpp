/**
 * @file Classifier.cpp

 * @implementation of different classification methods.

 * @author Ping Li

 * @version 2014-08-11

 * this is a common interface of different classification methods.
*/

#include "../../HeaderFiles/classifier/Classifier.h";


/**none parameter constructor*/
CClassifier::CClassifier()
{

}

/**destructor*/
CClassifier::~CClassifier()
{

}

/**
 *@show classification result
 *@infilename: the training set under metric space that after pivot selection and distance calculation
 *@outfilename: outfilename is going to store the classify result
 */
void CClassifier::showClassifierResult(char *infilename,const char* outfilename)
{

}