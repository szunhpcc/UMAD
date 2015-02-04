
#include "../../HeaderFiles/classifier/Classifier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <memory>

#define  ATTR_NUM  3                   //the number of attribute
#define  MAX_SIZE_OF_TRAINING_SET  21000      //maxsize of training set
#define  MAX_SIZE_OF_TEST_SET      21000       //maxsize of test set
#define  MAX_VALUE  10000.0                  //maxvalue of attribute
#define  K  3
using namespace std;

/**
 *@ file  knn.h
 *@ classification algorithm
 *@ author  from network
 *@ version  2014-08-01
 */

/**
 *@ class cKnn
 *@ classification algorithm
 *@ author from network
 */

class cKnn:public CClassifier
{
private:
struct dataVector 
{
	int ID;                      //ID number
	char classLabel[20];             //class label
	double attributes[ATTR_NUM]; //attribute
};
struct distanceStruct 
{
	int ID;                      //ID number
	double distance;             //distance
	char classLabel[15];             //class label
};

struct dataVector gTrainingSet[MAX_SIZE_OF_TRAINING_SET]; //training set
struct dataVector gTestSet[MAX_SIZE_OF_TEST_SET];         //test set
struct distanceStruct gNearestDistance[K];                //K nearest neighbour
int curTrainingSetSize;                                 //size of training set
int curTestSetSize;                                     //size of test set

public:
	cKnn();
	~cKnn();
/**
 *@ Euclidean distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
	virtual double euclideanDistance(struct dataVector vect1,struct dataVector vect2);

/**
 *@ L infinity distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
	virtual double LDistance(struct dataVector vect1,struct dataVector vect2);

/**
 *@ get the maxdistance in gNearestDistance and return the subscript
 */
	virtual int GetMaxDistance();

/**
 *@ classify unclassification sample
 */
	virtual char* Classify(struct dataVector Sample);

/**
 *@show classification result
 *@infilename: the training set under metric space that after pivot selection and distance calculation
 *@outfilename: outfilename is going to store the classify result
 */
	virtual void showClassifierResult(char *infilename,const char* outfilename);
};

