#include "../../HeaderFiles/metricdistance/CountedMetric.h"



CountedMetric::CountedMetric(void)

{

}





CountedMetric::~CountedMetric(void)

{

}

/**

* Creates a <code>CountedMetric</code> with a given base {@link Metric},

* setting the internal counter to zero.

*/

CountedMetric::CountedMetric(CMetricDistance *baseMetric){

	this->baseMetric = baseMetric;

	this->counter = 0;

}

/**

* Returns the value of {@link Metric#getDistance(IndexObject,IndexObject)}

* for the base {@link Metric} and increments the internal counter.

*/

double CountedMetric::getDistance(CMetricData *one, CMetricData *two){

	++counter;

	return (*baseMetric).getDistance(one, two);

}

/**

* Returns the current value of the internal counter.

*/

int CountedMetric::getCounter(){

	return counter;

}

/**

* Sets the internal counter to zero.

*/

void CountedMetric::clear(){

	counter = 0;

}

CMetricDistance* CountedMetric::getBaseMetric()
{
	return baseMetric;
}