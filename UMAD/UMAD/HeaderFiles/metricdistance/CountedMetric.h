#pragma once

#include "../metricdata/MetricData.h"

#include "MetricDistance.h"



class CountedMetric : public CMetricDistance

{

private:

	CMetricDistance *baseMetric;

	int  counter;

	

public:

	CountedMetric(void);

	~CountedMetric(void);

	CountedMetric(CMetricDistance *baseMetric);

	double getDistance(CMetricData *one, CMetricData *two);

	int getCounter();

	void clear();

	CMetricDistance* getBaseMetric();

};



