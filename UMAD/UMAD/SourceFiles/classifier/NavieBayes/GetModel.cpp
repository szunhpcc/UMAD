#include"../../../HeaderFiles/classifier/NavieBayes/GetModel.h"
GetModel::GetModel(GetInfo *g)
{
	gi=g;
}
void GetModel::TrainModel()
{   
	int SplitVal;
	Cache();//allocate memory for model
	for(int i=0;i<=gi->MaxAttNo;i++)
	{
		if(!gi->MaxAttValNo[i])
			SplitPoint[i]=gi->SplitContinuousAtt(i);
	}
	for(int i=0;i<=gi->MaxItemNo;i++)
	{
		for(int j=0;j<=gi->MaxAttNo;j++)
		{   
			if(gi->MaxAttValNo[j])
			    PostFreq[gi->Item[i][gi->MaxAttNo+1].DiscrValue][j][gi->Item[i][j].DiscrValue]++;
			else
			{   
				if(gi->Item[i][j].continuousVal<=SplitPoint[j])
					SplitVal=1;
				else
					SplitVal=2;
                PostFreq[gi->Item[i][gi->MaxAttNo+1].DiscrValue][j][SplitVal]++;
			}
		}
	}
}
void GetModel::Cache()
{   
	PostFreq=(float ***) calloc(gi->MaxClassNo+1,sizeof(float **));
	for(int i=0;i<=gi->MaxClassNo;i++)
	{
        PostFreq[i]=(float **) calloc(gi->MaxAttNo+1,sizeof(float *));
		for(int j=0;j<=gi->MaxAttNo;j++)
		{   
			if(gi->MaxAttValNo[j])
				PostFreq[i][j]=(float *)calloc(gi->MaxAttValNo[j]+1,sizeof(float));
			else
                PostFreq[i][j]=(float *)calloc(3,sizeof(float));
		}
	}
	SplitPoint=(float *)calloc(gi->MaxAttNo+1,sizeof(float));
}