#include"../../HeaderFiles/classifier/knn.h"

cKnn::cKnn()
{
	curTrainingSetSize=0;                                 //size of training set
    curTestSetSize=0; 
}

cKnn::~cKnn()
{

}

/**
 *@ Euclidean distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
double  cKnn::euclideanDistance(struct dataVector vector1,struct dataVector vector2)
{
	double dist,sum=0.0;
	for(int i=0;i<ATTR_NUM;i++)
	{
		sum+=(vector1.attributes[i]-vector2.attributes[i])*(vector1.attributes[i]-vector2.attributes[i]);
	}
	dist=sqrt(sum);
	return dist;
}

/**
 *@ L infinity distance between vector1=(x1,x2,...,xn) and vector2=(y1,y2,...,yn)
 */
double cKnn::LDistance(struct dataVector vect1,struct dataVector vect2)
{
	double dist_max=0.0;
	double values[ATTR_NUM];
	for(int i=0;i<ATTR_NUM;++i)
	{
		double temp;
		temp=fabs(vect1.attributes[i]-vect2.attributes[i]);
		values[i]=temp;
	}
	for(int j=0;j<ATTR_NUM;++j)
	{
		if(values[j]>dist_max)
			dist_max=values[j];
	}
	return dist_max;
}

/**
 *@ get the maxdistance in gNearestDistance and return the subscript
 */
int cKnn::GetMaxDistance()
{
	int maxNo=0;
	for(int i=1;i<K;i++)
	{
		if(gNearestDistance[i].distance>gNearestDistance[maxNo].distance)	maxNo = i;
	}
    return maxNo;
}

/**
 *@ classify unclassification sample
 */
char* cKnn::Classify(struct dataVector Sample)
{
	double dist=0;
	int maxid=0,freq[K],i,tmpfreq=1;;
	char *curClassLable=gNearestDistance[0].classLabel;
	memset(freq,1,sizeof(freq));
	//step.1---initalize the distance to MAX_VALUE
	for(i=0;i<K;i++)
	{
		gNearestDistance[i].distance=MAX_VALUE;
	}
	//step.2---calculate K-nearest neighbour distace
	for(i=0;i<curTrainingSetSize;i++)
	{
		//step.2.1---calculate the distance between unclassification sample and each training sample
		dist=euclideanDistance(gTrainingSet[i],Sample);
		//step.2.2---get the max distance in gNearestDistance
		maxid=GetMaxDistance();
		//step.2.3---if the dist less than the maxdistance in gNearestDistance，it will be one of k-nearest neighbour 
		if(dist<gNearestDistance[maxid].distance) 
		{
			gNearestDistance[maxid].ID=gTrainingSet[i].ID;
			gNearestDistance[maxid].distance=dist;
			strcpy(gNearestDistance[maxid].classLabel,gTrainingSet[i].classLabel);
		}
	}
	//step.3---statistics the number of each class appeared
	for(i=0;i<K;i++)  
	{
		for(int j=0;j<K;j++)
		{
			if((i!=j)&&(strcmp(gNearestDistance[i].classLabel,gNearestDistance[j].classLabel)==0))
			{
				freq[i]+=1;
			}
		}
	}
	//step.4---chose the class label with maximum frequency
	for(i=0;i<K;i++)
	{
		if(freq[i]>tmpfreq)  
		{
			tmpfreq=freq[i];
 			curClassLable=gNearestDistance[i].classLabel;
		}
	}
	return curClassLable;
}

/**
 *@show the classification result
 */
//void cKnn::showClassifierResult(char *infilename,const char* outfilename)
//{
//	char c; 
//    char *classLabel="";
//	int i,j, rowNo=0,TruePositive=0,FalsePositive=0;
//	ifstream filein(infilename);
//	FILE *fp;
//	if(filein.fail()){cout<<"Can't open the file"<<endl;}
//
//	//step.1---read file	
//	while(!filein.eof()) 
//	{
//		rowNo++;//first data:rowNo=1
//		if(curTrainingSetSize>=MAX_SIZE_OF_TRAINING_SET) 
//		{
//			cout<<"The training set has "<<MAX_SIZE_OF_TRAINING_SET<<" examples!"<<endl<<endl; 
//			break ;
//		}		
//	
//		if(rowNo%5!=0)
//		{			
//			gTrainingSet[curTrainingSetSize].ID=rowNo;
//			for(int i = 0;i < ATTR_NUM;i++) 
//			{					
//				filein>>gTrainingSet[curTrainingSetSize].attributes[i];
//				filein>>c;
//			}			
//			filein>>gTrainingSet[curTrainingSetSize].classLabel;
//			curTrainingSetSize++;
//			//cout<<"gTrainingSet"<<rowNo<<" : "<<gTrainingSet[curTrainingSetSize].ID<<" ";
//			/*for(int i=0; i< ATTR_NUM; ++i)
//			{
//				cout<<gTrainingSet[curTrainingSetSize].attributes[i]<<" ";
//			}
//			cout<<gTrainingSet[curTrainingSetSize].classLabel<<endl;*/
//			
//		}
//	
//		else if(rowNo%5==0)
//		{
//			gTestSet[curTestSetSize].ID=rowNo;
//			for(int i = 0;i < ATTR_NUM;i++) 
//			{				
//				filein>>gTestSet[curTestSetSize].attributes[i];
//				filein>>c;
//			}		
//			filein>>gTestSet[curTestSetSize].classLabel;
//			curTestSetSize++;
//			/*cout<<"gTestSet"<<rowNo<<" : "<<gTestSet[curTestSetSize].ID<<" ";
//			for(int i=0; i< ATTR_NUM; ++i)
//			{
//				cout<<gTestSet[curTestSetSize].attributes[i]<<" ";
//			}
//			cout<<gTestSet[curTestSetSize].classLabel<<endl;*/
//		}
//	}
//	filein.close();
//
//	//fp=fopen(outfilename,"w+t");
//	//fprintf(fp,"************************************程序说明***************************************\n");
//	//fprintf(fp,"** 采用KNN算法对iris_distance.txt分类。为了操作方便，对各组数据添加rowNo属性,第一组rowNo=1!\n");
//	//fprintf(fp,"** 共有150组数据,选择rowNo模3不等于0的100组作为训练数据集，剩下的50组做测试数据集\n");
//	//fprintf(fp,"***********************************************************************************\n\n");
//	//fprintf(fp,"************************************实验结果***************************************\n\n");
//	//for(i=0;i<curTestSetSize;i++)
//	//{
// //       fprintf(fp,"************************************第%d组数据**************************************\n",i+1);
//	//	classLabel =Classify(gTestSet[i]);
//	//    if(strcmp(classLabel,gTestSet[i].classLabel)==0)
//	//	{
//	//		TruePositive++;
//	//	}
//	//	cout<<"rowNo:	";
//	//	cout<<gTestSet[i].ID<<"    \t";
//	//	cout<<"KNN分类结果:      "; 
//
//	//	cout<<classLabel<<"(正确类标号: ";
//	//	cout<<gTestSet[i].classLabel<<")\n";
//	//	fprintf(fp,"rowNo:  %3d   \t  KNN分类结果:  %s ( 正确类标号:  %s )\n",gTestSet[i].ID,classLabel,gTestSet[i].classLabel);
//	//	if(strcmp(classLabel,gTestSet[i].classLabel)!=0)
//	//	{
//	//		cout<<"   ***分类错误***\n";
//	//		fprintf(fp,"                                                                      ***分类错误***\n");
//	//	}
//	//	fprintf(fp,"%d-最临近数据:\n",K);
//	//	
//	//	for(j=0;j<K;j++)
//	//	{
//	//		cout<<gNearestDistance[j].ID<<"\t"<<gNearestDistance[j].distance<<"\t"<<gNearestDistance[j].classLabel[15]<<endl;
//	//		fprintf(fp,"rowNo:	 %3d   \t   Distance:  %f   \tClassLable:    %s\n",gNearestDistance[j].ID,gNearestDistance[j].distance,gNearestDistance[j].classLabel);
//	//	}
//	//	fprintf(fp,"\n"); 
//	//}
// //   FalsePositive=curTestSetSize-TruePositive;
//	//fprintf(fp,"***********************************结果分析**************************************\n",i);
//	//fprintf(fp,"TP(True positive): %d\nFP(False positive): %d\naccuracy: %f\n",TruePositive,FalsePositive,double(TruePositive)/(curTestSetSize-1));
//	//fclose(fp);  
//
//
//	ofstream outfile(outfilename,ofstream::out);
//	outfile<<"************************************程序说明***************************************"<<endl;
//	outfile<<"** 采用KNN算法对iris_distance.txt分类。为了操作方便，对各组数据添加rowNo属性,第一组rowNo=1!"<<endl;
//	outfile<<"** 共有150组数据,选择rowNo模3不等于0的100组作为训练数据集，剩下的50组做测试数据集"<<endl;
//	outfile<<"***********************************************************************************"<<endl<<endl;
//	outfile<<"************************************实验结果***************************************"<<endl<<endl;
//	for(i=0;i<curTestSetSize;i++)
//	{
//		outfile<<"************************************第"<<i+1<<"组数据**************************************"<<endl;
//		classLabel =Classify(gTestSet[i]);
//	    if(strcmp(classLabel,gTestSet[i].classLabel)==0)
//		{
//			TruePositive++;
//		}
//		
//		outfile<<"rowNo:  "<<gTestSet[i].ID<<"   \t  KNN分类结果:  "<<classLabel<<"  ( 正确类标号:  "<<gTestSet[i].classLabel<<" )"<<endl;
//		if(strcmp(classLabel,gTestSet[i].classLabel)!=0)
//		{
//			outfile<<"                                                                      ***分类错误***"<<endl;
//		}
//		outfile<<K<<"-最临近数据:"<<endl;
//		
//		for(j=0;j<K;j++)
//		{
//			outfile<<gNearestDistance[j].ID<<"\t"<<gNearestDistance[j].distance<<"\t"<<gNearestDistance[j].classLabel[15]<<"\t"<<"rowNo: "<<gNearestDistance[j].ID<<"\t"<<"Distance: "<<gNearestDistance[j].distance<<"\t"<<"ClassLable: "<<gNearestDistance[j].classLabel<<endl;
//		}
//		outfile<<endl;
//	}
//    FalsePositive=curTestSetSize-TruePositive;
//	outfile<<"***********************************结果分析**************************************"<<endl;
//	outfile<<"TP(True positive): "<<TruePositive<<endl<<"FP(False positive): "<<FalsePositive<<endl<<"accuracy: "<<double(TruePositive)/(curTestSetSize-1)<<endl;
//	outfile.close();  
//}


void cKnn::showClassifierResult(char *infilename,const char* outfilename)
{
	char c; 
    char *classLabel="";
	int i,j, rowNo=0,TruePositive=0,FalsePositive=0;
	ifstream filein(infilename);
	FILE *fp;
	if(filein.fail()){cout<<"Can't open the file"<<endl;}

	//step.1---read file	
	while(!filein.eof()) 
	{
		rowNo++;//first data:rowNo=1
		if(curTrainingSetSize>=MAX_SIZE_OF_TRAINING_SET) 
		{
			cout<<"The training set has "<<MAX_SIZE_OF_TRAINING_SET<<" examples!"<<endl<<endl; 
			break ;
		}		
			
		gTrainingSet[curTrainingSetSize].ID=rowNo;
		for(int i = 0;i < ATTR_NUM;i++) 
		{					
			filein>>gTrainingSet[curTrainingSetSize].attributes[i];
			filein>>c;
		}			
		filein>>gTrainingSet[curTrainingSetSize].classLabel;

		if(rowNo%5==0)
		{
			gTestSet[curTestSetSize].ID=gTrainingSet[curTrainingSetSize].ID;
			for(int i = 0;i < ATTR_NUM;i++) 
			{				
				gTestSet[curTestSetSize].attributes[i]=gTrainingSet[curTrainingSetSize].attributes[i];
			}

			for(int j=0; j<20; j++)
			{
				gTestSet[curTestSetSize].classLabel[j]=gTrainingSet[curTrainingSetSize].classLabel[j];
			}
			curTestSetSize++;
		}
			curTrainingSetSize++;	
	}
	filein.close();

	//fp=fopen(outfilename,"w+t");
	//fprintf(fp,"************************************程序说明***************************************\n");
	//fprintf(fp,"** 采用KNN算法对iris_distance.txt分类。为了操作方便，对各组数据添加rowNo属性,第一组rowNo=1!\n");
	//fprintf(fp,"** 共有150组数据,选择rowNo模3不等于0的100组作为训练数据集，剩下的50组做测试数据集\n");
	//fprintf(fp,"***********************************************************************************\n\n");
	//fprintf(fp,"************************************实验结果***************************************\n\n");
	//for(i=0;i<curTestSetSize;i++)
	//{
 //       fprintf(fp,"************************************第%d组数据**************************************\n",i+1);
	//	classLabel =Classify(gTestSet[i]);
	//    if(strcmp(classLabel,gTestSet[i].classLabel)==0)
	//	{
	//		TruePositive++;
	//	}
	//	cout<<"rowNo:	";
	//	cout<<gTestSet[i].ID<<"    \t";
	//	cout<<"KNN分类结果:      "; 

	//	cout<<classLabel<<"(正确类标号: ";
	//	cout<<gTestSet[i].classLabel<<")\n";
	//	fprintf(fp,"rowNo:  %3d   \t  KNN分类结果:  %s ( 正确类标号:  %s )\n",gTestSet[i].ID,classLabel,gTestSet[i].classLabel);
	//	if(strcmp(classLabel,gTestSet[i].classLabel)!=0)
	//	{
	//		cout<<"   ***分类错误***\n";
	//		fprintf(fp,"                                                                      ***分类错误***\n");
	//	}
	//	fprintf(fp,"%d-最临近数据:\n",K);
	//	
	//	for(j=0;j<K;j++)
	//	{
	//		cout<<gNearestDistance[j].ID<<"\t"<<gNearestDistance[j].distance<<"\t"<<gNearestDistance[j].classLabel[15]<<endl;
	//		fprintf(fp,"rowNo:	 %3d   \t   Distance:  %f   \tClassLable:    %s\n",gNearestDistance[j].ID,gNearestDistance[j].distance,gNearestDistance[j].classLabel);
	//	}
	//	fprintf(fp,"\n"); 
	//}
 //   FalsePositive=curTestSetSize-TruePositive;
	//fprintf(fp,"***********************************结果分析**************************************\n",i);
	//fprintf(fp,"TP(True positive): %d\nFP(False positive): %d\naccuracy: %f\n",TruePositive,FalsePositive,double(TruePositive)/(curTestSetSize-1));
	//fclose(fp);  


	ofstream outfile(outfilename,ofstream::out);
	outfile<<"************************************程序说明***************************************"<<endl;
	outfile<<"** 采用KNN算法对iris_distance.txt分类。为了操作方便，对各组数据添加rowNo属性,第一组rowNo=1!"<<endl;
	outfile<<"** 共有150组数据,选择rowNo模3不等于0的100组作为训练数据集，剩下的50组做测试数据集"<<endl;
	outfile<<"***********************************************************************************"<<endl<<endl;
	outfile<<"************************************实验结果***************************************"<<endl<<endl;
	for(i=0;i<curTestSetSize;i++)
	{
		outfile<<"************************************第"<<i+1<<"组数据**************************************"<<endl;
		classLabel =Classify(gTestSet[i]);
	    if(strcmp(classLabel,gTestSet[i].classLabel)==0)
		{
			TruePositive++;
		}
		
		outfile<<"rowNo:  "<<gTestSet[i].ID<<"   \t  KNN分类结果:  "<<classLabel<<"  ( 正确类标号:  "<<gTestSet[i].classLabel<<" )"<<endl;
		if(strcmp(classLabel,gTestSet[i].classLabel)!=0)
		{
			outfile<<"                                                                      ***分类错误***"<<endl;
		}
		outfile<<K<<"-最临近数据:"<<endl;
		
		for(j=0;j<K;j++)
		{
			outfile<<gNearestDistance[j].ID<<"\t"<<gNearestDistance[j].distance<<"\t"<<gNearestDistance[j].classLabel[15]<<"\t"<<"rowNo: "<<gNearestDistance[j].ID<<"\t"<<"Distance: "<<gNearestDistance[j].distance<<"\t"<<"ClassLable: "<<gNearestDistance[j].classLabel<<endl;
		}
		outfile<<endl;
	}
    FalsePositive=curTestSetSize-TruePositive;
	outfile<<"***********************************结果分析**************************************"<<endl;
	outfile<<"TP(True positive): "<<TruePositive<<endl<<"FP(False positive): "<<FalsePositive<<endl<<"accuracy: "<<double(TruePositive)/(curTestSetSize-1)<<endl;
	outfile.close();  
}


