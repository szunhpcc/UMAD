#include "../../../HeaderFiles/classifier/NavieBayes/NavieBayes.h"
#include <string.h>


/**
 *@ show classification result
 *@ datafilename: the primary training set
 *@ infilename: the training set under metric space that after pivot selection and distance calculation
 *@ testfilename: unclassified samples
 *@ outfilename: outfilename is going to store the classify result
*/
void CNavieBayes::showClassifierResult(char *infilename,char *testfilename,char *testlabel,const char* outfilename)
{
	/**remove the infilename suffix*/
	int i=0;
	i=strlen(infilename);
	char *newinfilename=infilename+i;
	while(newinfilename > infilename)
	{
		if(*newinfilename=='.')
		{
			*newinfilename=0;
			break;
		}
		newinfilename--;
	}

	GetInfo gi(infilename,testlabel);
	gi.getname();
	gi.getdata();

	GetModel gm(&gi);
	gm.TrainModel();
	
	PredictClass pt(&gi,&gm,testfilename,infilename);
	pt.Predict(outfilename);
}