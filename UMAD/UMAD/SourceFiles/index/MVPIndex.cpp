#include "../../HeaderFiles/index/MVPIndex.h"

#include "../../HeaderFiles/metricdata/DoubleVector.h"



#include<cmath>



/** @file MVPIndex.cpp
* @classes about mvp tree index structure
* @author Fuli Lei,Qiaozhi Li
* @version 2013-6-20
*/



/**
* @class Task
* @brief class about tasks that should be completed during the process of building a mvp-tree
* @author Fuli Lei
*
* In the whole process of building a mvp-tree, some same openration like creating a internal node or leaf node could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees
*/

class Task 
{

public:

	/**give class CMVPIndex a friend right to access the data information of a Task instance in the process of building a mvp tree*/

	friend class CMVPIndex;

	/**@brief constructor with six parameters

	*	@param dataList a vector contains the address of all the obejct over which this mvp tree is built

	*	@param parentIndex address of parent node

	*	@param start the start index of current object partition in the whole data list

	*	@param end the last object index of current object partition

	*	@param myIndex current node index in parent node's children list

	*	@param myHeight height of current node

	*/

	Task(vector< shared_ptr<CMetricData> > &dataList,shared_ptr<CIndexNode> parentIndex,int start,int end,int myIndex,int myHeight):dataList(dataList)
	{

		this->parentIndex=parentIndex;

		this->myIndex=myIndex;



		this->myHeight=myHeight;





		this->start=start;

		this->size=end-start;



		numPivots=0;



		specialN=0;



		this->node=shared_ptr<CIndexNode>();



	};



	~Task()
	{        

		//node.reset();

	}



	friend int standardCreateInternalNodeToWrite(shared_ptr<Task> task,list<shared_ptr<Task> > *taskList,list<shared_ptr<Task> >*newTaskList,CMetricDistance *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot);





	/**@brief move the pivot to the end of a object list partition

	* @param pivotIndex a vector contains all the indices of pivots in the whole objects list

	*/

	void groupPivotsAtEnd(vector<int> &pivotIndex);



	/**@brief get the index of current node in the parent's children list*/

	int getMyIndex();



	/**@brief get the height of current node. the height of root is 0*/

	int getMyHeight();



	void setFlag(int stat){ specialN=stat;}

	int getFlag(){return specialN;}



	/**@brief get the parent node's address*/

	shared_ptr<CIndexNode> getParentIndex();



	/**@brief get all the pivots that belongs to current node

	* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.

	*/

	void getPivots(vector< shared_ptr<CMetricData> >&pivots);



	/**@brief get all the objects that belong to the tree rooted at this node*/

	void getDataPoints(vector< shared_ptr<CMetricData> >&dataList);



	vector<shared_ptr<CMetricData> >& getDataList();

	long getSize();

	int getPivotNumber();

	long getStart();



	void setNode(shared_ptr<CIndexNode> node)
	{

		this->node = node;

	}
	shared_ptr<CIndexNode> getNode()
	{

		return node;

	}

private:

	/**constructor*/

	Task():dataList(*(new vector< shared_ptr<CMetricData> >))
	{



		this->parentIndex=shared_ptr<CIndexNode>();

		this->myIndex=0;

		this->myHeight=0;



		this->size=0;

		this->start=0;



		specialN=0;

		this->numPivots=0;



		this->node=shared_ptr<CIndexNode>();



	};







	int specialN;



	/**a vector contains the address of all the obejct over which this mvp tree is built*/

	vector<shared_ptr<CMetricData> > &dataList;



	/**address of parent node*/

	shared_ptr<CIndexNode> parentIndex;



	shared_ptr<CIndexNode> node;



	long myIndexAdress;



	/**current node index in parent node's children list*/

	int myIndex;



	/**height of current node*/

	int myHeight;



	/**the start index of current object partition in the whole objects list*/

	int start;



	/**number of objects in current object partition*/

	int size;



	/**number of pivots in a node of mvp tree*/

	int numPivots;







};



/**@brief move the pivot to the end of a object list partition
* @param pivotIndex a vector contains all the indices of pivots in the whole objects list
*/

void Task::groupPivotsAtEnd(vector<int> &pivotIndex)
{

	/*initialize numPivots*/

	this->numPivots=pivotIndex.size();





	if(this->numPivots<this->size)
	{

		/*new address of pivots, the first pivots will be moved to the end of current object partition, start+this->size-1 is the end index of current object partition*/

		int newPivotAddressInCompressedData=start+this->size-1;



		/*move every pivot to the end of partiton*/

		for(int i=0;i<this->numPivots;i++)
		{

			shared_ptr<CMetricData> temp;



			/*exchange the address content of pivot element unit and the target element unit*/

			temp=this->dataList[pivotIndex[i]];

			dataList[pivotIndex[i]]=dataList[newPivotAddressInCompressedData];

			dataList[newPivotAddressInCompressedData]=temp;



			/*judge if the content of target elment unit is the address of another pivot whose index is also in the pivot index list, exchange the index the result of judge is true*/

			for(int j=i+1;j<this->numPivots;j++)

				if(pivotIndex[j]==newPivotAddressInCompressedData)

					pivotIndex[j]=pivotIndex[i];



			/*the first pivot move the end of partition, the next pivot is always moved to one element unit before the previous moved pivots*/

			newPivotAddressInCompressedData--;

		}

	}



}



/**@brief get the index of current node in the parent's children list*/

shared_ptr<CIndexNode> Task::getParentIndex()
{

	return this->parentIndex;

}



/**@brief get the index of current node in the parent's children list*/

int Task::getMyIndex()
{

	return this->myIndex;

}



/**@brief get the height of current node. the height of root is 0*/

int Task::getMyHeight()
{

	return this->myHeight;

}



/**@brief get all the pivots that belongs to current node

* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.

*/

void Task::getPivots(vector< shared_ptr<CMetricData> > &pivots)
{

	/*start should be the first pivot index in current partition*/ 

	int start=this->start+this->size-this->numPivots;





	for(int i=0;i<this->numPivots;i++)
	{

		pivots.push_back(this->dataList[start]);



		start++;

	}



}



/**@brief get all the objects that belong to the tree rooted at this node*/

void Task::getDataPoints(vector<shared_ptr<CMetricData> > &objectList)
{

	int mySize=0;



	/**if the number of pivot is less 0,return all the objects in current partitin*/

	if(numPivots==-1)

		mySize=this->size;

	else

		mySize=this->size-this->numPivots;



	for(int i=start;i<start+mySize;i++)
	{

		objectList.push_back(this->dataList[i]);

	}

}



vector<shared_ptr<CMetricData> >& Task::getDataList()
{

	return dataList;

}



long Task::getSize()
{

	return size;

}



int Task::getPivotNumber()
{

	return numPivots;

}



long Task::getStart()
{

	return start;

}



/////////////////////////////////////////////////////////////







////////////////////////////////////////////////////////////







/**@brief None parameter constructor */

CMVPIndex::CMVPIndex()
{

	root=shared_ptr<CIndexNode>();



	metric=0;



	psm=0;

	pm=0;



	singlePivotFanout=0;



	maxLeafSize=0;



	numPivots=0;

	numLeaf=0;

	numInternalNodes=0;

	trisectionRadius = 0.1;
	selectOptimalPivots = "BalanceOne";
	partitionByOnePivot = "Variance";

}



void CMVPIndex::setMetric(CMetricDistance *objMetric)
{

	metric = objMetric;

}



/**@brief constructor with eight parameters.

* @param data the whole object set above which the mvp-tree will be built

* @param metric it is a distance function, we can use it to work out the distance of two objects

* @param psm the name of this parameter means pivot-selection-method, it is a vantage points selection method

* @param pm pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it

* @param numPivot the maxmum number of pivots in a single mvp-tree node

* @param singlePivotFanout the maxmum number of partitions related to a single pivots

* @param maxLeafSize the maxmum number of objects in a leaf node of mvp-tree

* @param maxPathLength the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree

*/

CMVPIndex::CMVPIndex(vector<shared_ptr<CMetricData> > &dataObjects,CMetricDistance *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivots,int singlePivotFanout,int maxLeafSize,int _buildMode,int _threadNum,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{

	this->root=shared_ptr<CIndexNode>();

	this->metric=metric;

	this->numPivots=numPivots;

	this->psm=psm;

	this->pm=pm;

	this->singlePivotFanout=singlePivotFanout;

	this->totalSize=dataObjects.size();

	this->maxLeafSize=maxLeafSize;

	this->buildMode=_buildMode;

	this->threadNum=_threadNum;

	numLeaf=0;

	numInternalNodes=0;

	this->trisectionRadius = trisectionRadius;
	this->selectOptimalPivots = selectOptimalPivots;
	this->partitionByOnePivot = partitionByOnePivot;

}



/**@brief destructor*/

CMVPIndex::~CMVPIndex()
{



}




void CMVPIndex::bulkLoad(vector<std::shared_ptr<CMetricData> > &objectList,int buildMode,int threadNum)
{

	if (buildMode == 0)
		loadLeafToRoot(objectList);
	else if(buildMode == 1)
		loadRootToLeaf(objectList);
	else 
		parallelBuildingMVPtree(objectList,threadNum);

}


/**@brief function to create an internal node

* @param task encapsulated the information which will be used to create a internal node of mvp tree

*/

//void CMVPIndex::createInternalNode(Task *task)
//{
//	this->numInternalNodes ++;
//	//cout<<"internalNode:"<<numInternalNodes<<endl;
//	/*get pivots of current node*/
//	vector<shared_ptr<CMetricData> > pivots;
//	task->getPivots(pivots);
//
//	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
//	CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size-task->numPivots,this->singlePivotFanout,this->maxLeafSize);
//
//	int childrenNumber=pr.partitionSize();
//
//	/*create an internal node and assign its addres to the children list of parent node*/
//    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
//	shared_ptr<CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1));
//
//	if(this->root==0)
//		this->root=node;
//	else
//		(((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=node;
//
//	/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
//	for(int i=childrenNumber-1;i>0;i--)
//	{
//		Task *newTask=0;
//
//		newTask = new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1);
//
//
//		this->taskList.push_back(newTask);
//
//
//
//		this->taskList.insert(this->taskList.begin(),newTask);
//		//this->taskList.push_back(newTask);
//
//	}
//
//}

/**@brief function to create a leaf node
* @param task encapsulated the information which will be used to create a leaf node of mvp tree
*/
//void CMVPIndex::createLeafNode(Task* task)
//{
//	this->numLeaf++;
//	//cout<<"leafNode:"<<numLeaf<<endl;
//	/*get all the objects of current partition*/
//	vector<CMetricData*> children;
//	task->getDataPoints(children);
//
//	/*get all the pivots of current node*/
//	vector<CMetricData*> pivots;
//	task->getPivots(pivots);
//
//	vector<vector<double>> distance;
//
//	/*calcualte the distance from each of the objects of current parition to every pivots*/
//	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
//	{
//		vector<double> ve;
//
//		for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
//		{
//			ve.push_back(this->metric->getDistance(children[j],pivots[i]));
//		
//
//		distance.push_back(ve);
//	}
//
//	/*create a leaf node and assign its memory address to the child list of parent node*/
//	CMVPLeafNode *mvpLeafNode=new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1);
//
//	if(this->root==0)
//		this->root=mvpLeafNode;
//	else
//		(((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=mvpLeafNode;
//
//}

/**@brief this is the function to build a mvp-tree 
*@param dataObjects the whole object set above which the mvp-tree will be built
*/

vector<shared_ptr<CMetricData> > CMVPIndex::travelSearch(CQuery* q)
{

	vector<shared_ptr<CMetricData> > re;



	CRangeQuery *rq=(CRangeQuery*)q;

	root->travelSearch(*rq,*metric,re);

	return re;

}


/**@brief get the number of data objects that make up the index struct
*@return the number of data objects that make up the index struct
*/
int CMVPIndex::size()
{
	return totalSize;
}



int traversingMVPTree(shared_ptr<CIndexNode> node, vector<shared_ptr<CMetricData> > &data)
{

	if(node==NULL)

		return 0;



	shared_ptr<CMVPInternalNode> interNode(dynamic_cast<CMVPInternalNode*>(node.get()));

	if(interNode!=0)
	{

		for(int i=0;i<interNode->getPivots().size();i++)

		{

			data.push_back(interNode->getPivot(i));

		}

		int childNum=interNode->numChildren();

		for(int i=0;i<childNum;i++)

			traversingMVPTree(interNode->getChildAddress(i),data);

		return 0;

	}


	shared_ptr<CMVPLeafNode> leafNode(dynamic_cast<CMVPLeafNode*>(node.get()));

	if(leafNode!=0)
	{

		for(int i=0;i<leafNode->getPivots().size();i++)
		{

			data.push_back(leafNode->getPivot(i));

		}

		for(int i=0;i<leafNode->numChildren();i++)
		{

			data.push_back(leafNode->getObject(i));

		}

	}

}



/**@brief get the object list contains all the objects that make up the index structure

*@return all data objects that make up the index structure

*/

vector<shared_ptr<CMetricData> > CMVPIndex::getAllDataPoints()
{

	vector<shared_ptr<CMetricData> > data;

	traversingMVPTree(this->root,data);

	return data;

}



/**@brief get the root of a mvptree built before

*@return the root of mvp-tree

*/

shared_ptr<CIndexNode> CMVPIndex::getMVPTreeRoot()
{

	return this->root;

}



/**@brief get the metric object of a mvptree

*@return the member varible metric,which is used to calculate the distance of two data objects

*/

CMetricDistance* CMVPIndex::getMetric()
{

	return metric;

}


void CMVPIndex::setDataType(string dataType)
{

	this->dataType = dataType;

}



string CMVPIndex::getDataType()
{

	return this->dataType;

}



void CMVPIndex::setFileName(string fileName)
{

	this->fileName=fileName;

}



string CMVPIndex::getFileName()
{

	return fileName;

}



/*@brief this is a search method, users can use it to find out all the objects in demand

* @param q: this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)

* @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r

*/

list<shared_ptr<CMetricData> >* CMVPIndex::search(CQuery* q)
{

	//CMemMonitor::updateMem();



	list<shared_ptr<CMetricData> > *re=new list<shared_ptr<CMetricData> >;

	CRangeQuery *rq=(CRangeQuery*)q;





	filePointer = 0;



	ifstream in(fileName.c_str(),ios::binary);



	in.seekg(-(long)sizeof(long),ios::end);



	in.read((char*)(&rootAddress),sizeof(long));





	in.seekg(rootAddress,ios::beg);





	shared_ptr<CIndexNode> root;

	char* type = new char[6];

	in.read(type,6*sizeof(char));

	if(!strcmp(type,"INODE"))
	{

		root = dynamic_pointer_cast<CMVPInternalNode> (root);

		root.reset(new CMVPInternalNode());

		/*root = new CMVPInternalNode();*/

	}

	else

	{

		root = dynamic_pointer_cast<CMVPLeafNode> (root);

		root.reset( new CMVPLeafNode());

		//root = new CMVPLeafNode();

	}

	rootAddress += 6*sizeof(char);

	delete(type);	

	root->searchIndex(*rq,rootAddress,in,*metric,re,dataType);



	// CMemMonitor::updateMem();

	in.close();

	// CMemMonitor::updateMem();

	return re;
}



/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE BEGIN***************************************************************/

/*DATA STRUCT FOR BOTH WINDOWS AND C++11 STANDARD VERSION OF MULTI-THREAD CODING*/

struct parallelBlock
{

	/*constuctor*/

	//parallelBlock(){cout<<"building a parallel block"<<endl;}

	/*desturctor*/

	//~parallelBlock(){cout<<"rootTask.useCount:"<<rootTask.use_count()<<" destroying a parallel block"<<endl;}	



	ofstream *fstr;

	int *numPivots;

	CMetricDistance *metric;

	CPivotSelectionMethod *psm;

	CPartitionMethod *pm;



	shared_ptr<Task> task;

	list<shared_ptr<Task> > childTaskList;

	list<shared_ptr<Task> > wTaskList;

	bool isleaf;

	int maxLeafSize;

	int singlePivotFanout;

	int *numInternalNode;

	int *numLeafNode;



	string fileName;

	long *filePointer;

	long *rootAddress;



	shared_ptr<Task> rootTask;


	double trisectionRadius;
	char* selectOptimalPivots;
	char* partitionByOnePivot;

};





mutex stMutexA;

mutex stMutexB;

mutex stMutexO;

mutex stMutexN;



int stdThreadNumA=0;

int stdThreadNumB=0;



int standardCreateInternalNodeToWrite(shared_ptr<Task> task,list<shared_ptr<Task> > *taskList,list<shared_ptr<Task> >*newTaskList,CMetricDistance *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize,double trisectionRadius,char* selectOptimalPivots,char* partitionByOnePivot)
{

	(*numInternalNodes)++;



	/*get pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);





	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/

	CPartitionResults pr=pm->partition(metric,pivots,task->getDataList(),start,size - task->getPivotNumber(),singlePivotFanout,maxLeafSize,trisectionRadius,selectOptimalPivots,partitionByOnePivot);

	int childrenNumber=pr.partitionSize();

	int flag=0;

	if(childrenNumber==2)
	{
		flag=1;
		/*double distance=metric->getDistance(task->dataList[task->start].get(),task->dataList[task->start+1].get());

		for(int i=task->start+1;i<task->start+task->size-task->numPivots;i++)
		{

			if(metric->getDistance(task->dataList[task->start].get(),task->dataList[i].get())==distance)
			{

				flag=1;

			}

			else
			{

				flag=0;

				break;

			}

		}*/

	}

	//pthread_mutex_lock(&stMutexA);

	//if(task->getMyHeight()==6)

	//	cerr<<"task.Height:"<<task->getMyHeight()<<" INTER-NODE task->index:"<<task->getMyIndex()<<" pivotsize:"<<pivots.size()<<" task.start:"<<start<<" size:"<<size<<" childpartitionsize:"<<pr.partitionSize()-1<<endl;

	//pthread_mutex_unlock(&stMutexA);

	/*create an internal node and assign its addres to the children list of parent node*/

	if(flag==0)
	{

		vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

		shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );

		node->setChildSize(childrenNumber-1);





		task->setNode((shared_ptr<CIndexNode>)node);



		newTaskList->push_back(task);



		/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/

		for(int i=childrenNumber-1;i>0;i--)
		{

			int j=0;

			shared_ptr<Task> newTask( new Task(task->getDataList(),node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));       

			taskList->push_back(newTask);

		}

		return 0;

	}
	else
	{

		task->setFlag(1);
		taskList->push_back(task);

	}

}



int standardCreateAndWriteLeafNode(shared_ptr<Task> task,ofstream *out,list<shared_ptr<Task> > *taskList,string fileName,long *filePointer,int *numLeaf,CMetricDistance *metric,long *rootAddress)
{



	(*numLeaf)++;



	/*get all the objects of current partition*/

	vector<shared_ptr<CMetricData> > children;

	task->getDataPoints(children);



	/*get all the pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);



	vector<vector<double> > distance;



	/*calcualte the distance from each of the objects of current parition to every pivots*/

	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
	{

		vector<double> ve;



		for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
		{

			ve.push_back(metric->getDistance(children[j].get(),pivots[i].get()));

		}



		distance.push_back(ve);

	}



	/*create a leaf node and assign its memory address to the child list of parent node*/

	shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));



	if(*rootAddress==-2)
	{        



		task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);

		taskList->push_back(task);

		return 0;

	}
	else
	{

		//pthread_mutex_lock(&stMutexO);

		//out->open(fileName.c_str(),ios::in|ios::out|ios::binary)

		//out->seekp(*filePointer);

		char type[6] = "LNODE";

		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;

		out->seekp(*filePointer);

		out->write(type,6*sizeof(char));

		*filePointer=*filePointer+(long)mvpLeafNode->writeExternal(*out)+6*sizeof(char);

		//out->close();

		//pthread_mutex_unlock(&stMutexO);

		return 0;

	}



}



void* standardThreadOfCreateInternalOrLeafNode(void *vpb)
{

	parallelBlock *pb=(parallelBlock*)vpb;

	if(pb->task->getFlag()==0)
	{

		int pivotNum=((*(pb->numPivots)>pb->task->getSize())?pb->task->getSize():(*(pb->numPivots)));

		vector<int> pivotsIndex;

		pivotsIndex=pb->psm->selectPivots(pb->metric,pb->task->getDataList(),pb->task->getStart(),pb->task->getSize(),pivotNum);

		//pthread_mutex_lock(&stMutexB);

		//if(pb->task->getMyHeight()==1)

		//	cerr<<"index:"<<pb->task->getMyIndex()<<" task.start:"<<pb->task->getStart()<<" task.size:"<<pb->task->getSize()<<" pivotnum:"<<pivotsIndex.size()<<" pivotNum:"<<pivotNum<<endl;

		//pthread_mutex_unlock(&stMutexB);

		pb->task->groupPivotsAtEnd(pivotsIndex);



		int remainDataSize=pb->task->getSize()-pivotsIndex.size();



		if(remainDataSize>pb->maxLeafSize)
		{

			standardCreateInternalNodeToWrite(pb->task,&(pb->childTaskList),&(pb->wTaskList),pb->metric,pb->pm,pb->numInternalNode,pb->task->getStart(),pb->task->getSize(),pb->singlePivotFanout,pb->maxLeafSize,pb->trisectionRadius,pb->selectOptimalPivots,pb->partitionByOnePivot);

		}
		else
		{	

			pb->isleaf=1;

			//standardCreateAndWriteLeafNode(pb->task,pb->fstr,&(pb->wTaskList),pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);

		}

	}

	return 0;

}





void CMVPIndex::parallelBuildingMVPtree(vector<shared_ptr<CMetricData> > &dataObjectList,int maxThreadNum)
{

	ofstream outfile(fileName.c_str(),ios::out|ios::binary);



	filePointer=0;

	int maxTreeHeight=0;



	if(dataObjectList.size()<=maxLeafSize+numPivots)

		rootAddress=-2;

	else

		rootAddress=-1;


	list<parallelBlock*> buildTaskList;

	list<shared_ptr<Task> > tobewritetasklist;

	list<parallelBlock*> leafBuildTaskList;

	shared_ptr<Task> task(new Task(dataObjectList,shared_ptr<CIndexNode>(),0,dataObjectList.size(),0,0));//first task



	numPivots=(numPivots>=task->size)?task->size:numPivots;

	vector<int> pivotsIndex;

	pivotsIndex=psm->selectPivots(metric,task->dataList,task->start,task->size,numPivots);

	task->groupPivotsAtEnd(pivotsIndex);



	list<shared_ptr<Task> > childtasklist;

	standardCreateInternalNodeToWrite(task,&childtasklist,&tobewritetasklist,metric,pm,&numInternalNodes,task->start,task->size,singlePivotFanout,maxLeafSize,trisectionRadius,selectOptimalPivots,partitionByOnePivot);





	while(childtasklist.size()>0)
	{

		parallelBlock *pba=new parallelBlock;



		pba->fileName=fileName;

		pba->filePointer=&filePointer;

		pba->fstr=&outfile;

		pba->maxLeafSize=maxLeafSize; 

		pba->metric=metric; 

		pba->task=childtasklist.back();

		pba->numInternalNode=&numInternalNodes;

		pba->numLeafNode=&numLeaf;

		pba->numPivots=&numPivots;

		pba->pm=pm;

		pba->psm=psm;

		pba->rootAddress=&rootAddress;

		pba->singlePivotFanout=singlePivotFanout;

		pba->isleaf=0;

		pba->trisectionRadius = trisectionRadius;
		pba->selectOptimalPivots = selectOptimalPivots;
		pba->partitionByOnePivot = partitionByOnePivot;

		childtasklist.pop_back();

		buildTaskList.push_back(pba);

	}



	/*compute the maximum number of node in the index will be built,use the number as the maximum of thread created in the future */
	//int dataObjectSize=dataObjectList.size();
	//int numThreadNum=pow(pow(singlePivotFanout,numPivots),ceil(log(dataObjectSize)/log(maxLeafSize)));




	cerr<<"Number of thread:"<<maxThreadNum<<endl;
	while(buildTaskList.size()>0)
	{

		int taskNum=buildTaskList.size();



		thread *threadList=new thread[maxThreadNum];

		list<parallelBlock*> newBuildTaskList;

		int j=0;	
		for(int i=0;i<taskNum;i++)
		{

			if(j<maxThreadNum)
			{

				parallelBlock *pb=buildTaskList.back();



				threadList[j]=thread(standardThreadOfCreateInternalOrLeafNode,pb);



				newBuildTaskList.push_back(pb);

				buildTaskList.pop_back();

				j++;

			}
			else
			{

				for(int k=0;k<j;k++)
				{

					threadList[k].join();

				}

				j=0;

			}

		}



		for(int i=0;i<j;i++)
		{

			void *childthread=0;

			threadList[i].join();

		}

		delete [] threadList;


		while(newBuildTaskList.size()>0)
		{

			parallelBlock *pb=newBuildTaskList.back();

			newBuildTaskList.pop_back();



			tobewritetasklist.insert(tobewritetasklist.end(),pb->wTaskList.begin(),pb->wTaskList.end());



			if(!pb->isleaf&&pb->task->getFlag()==0)
			{

				int newTaskNum=pb->childTaskList.size();

				for(int i=0;i<newTaskNum;i++)
				{

					shared_ptr<Task> task=pb->childTaskList.back();

					pb->childTaskList.pop_back();



					parallelBlock *newpb=new parallelBlock;



					newpb->fileName=fileName;

					newpb->filePointer=&filePointer;

					newpb->fstr=&outfile;

					newpb->maxLeafSize=maxLeafSize;

					newpb->metric=metric;

					newpb->task=task;

					newpb->numInternalNode=&numInternalNodes;

					newpb->numLeafNode=&numLeaf;

					newpb->numPivots=&numPivots;

					newpb->pm=pm;

					newpb->psm=psm;

					newpb->rootAddress=&rootAddress;

					newpb->singlePivotFanout=singlePivotFanout;

					newpb->isleaf=0;

					newpb->trisectionRadius = trisectionRadius;
					newpb->selectOptimalPivots = selectOptimalPivots;
					newpb->partitionByOnePivot = partitionByOnePivot;

					buildTaskList.push_back(newpb);



				}

				maxTreeHeight=(maxTreeHeight>=pb->task->getMyHeight())?maxTreeHeight:pb->task->getMyHeight();

				delete pb;	

			}
			else
			{



				leafBuildTaskList.push_back(pb);

				maxTreeHeight=(maxTreeHeight>=pb->task->getMyHeight())?maxTreeHeight:pb->task->getMyHeight();

			}

			//clock_t endt=times(NULL);

			//cout<<__FILE__<<__LINE__<<" time elapsed:"<<(endt-begt)/100.00<<endl;

		}



	}

	int leafTaskNum=leafBuildTaskList.size();
	for(int i=0;i<leafTaskNum;i++)
	{

		parallelBlock *pb=leafBuildTaskList.back();

		if(pb->task->getFlag()==1&&pb->task->getMyHeight()==0)
		{
			this->rootAddress=-2;
			*pb->rootAddress=-2;
			standardCreateAndWriteLeafNode(pb->task,pb->fstr,&(pb->wTaskList),pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);	
			break;
		}
		else
		{
			leafBuildTaskList.pop_back();

			standardCreateAndWriteLeafNode(pb->task,pb->fstr,&(pb->wTaskList),pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);			

			delete pb;
		}

	}

	while(tobewritetasklist.size()>1)
	{

		shared_ptr<Task> temp=tobewritetasklist.back();



		writeInternalNode(temp,outfile);



		tobewritetasklist.pop_back();

	}

	outfile.close();
	if(leafBuildTaskList.size()>0)
		writeRoot(leafBuildTaskList.back()->task,outfile,maxTreeHeight);
	else if(tobewritetasklist.size()>0)
		writeRoot(tobewritetasklist.back(),outfile,maxTreeHeight);

}



/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE END*************************************************************/


void CMVPIndex::loadLeafToRoot(vector<shared_ptr<CMetricData> > &dataObjects)
{

	/*PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.properties"));

	Logger logger = Logger::getRoot();*/

	ofstream out;
	out.open(fileName.c_str(),ios::binary);
	out.close();

	filePointer = 0;

	int maxTreeHeight=0;

	if(dataObjects.size()<=maxLeafSize+numPivots)
	{

		rootAddress = -2;

	}
	else
	{

		rootAddress = -1;

	}



	vector<shared_ptr<Task> >::iterator taskToGet;



	/*put the task that create the root node to the task queue*/

	shared_ptr<Task> task(new Task(dataObjects,shared_ptr<CIndexNode>(),0,dataObjects.size(),0,0));



	this->taskList.push_back(task);

	int c=0;

	/*use every task in the queue to create a internal node or leaf node until the the queue is null*/

	while(this->taskList.size()>0)
	{

		//clock_t begt=times(NULL);

		taskToGet=this->taskList.end();

		int remainNodeSize;

		taskToGet--;

		task = *taskToGet;

		maxTreeHeight=(maxTreeHeight>=task->getMyHeight())?maxTreeHeight:task->getMyHeight();

		if(task->getNode()==0)
		{



			/*get the number of pivot*/

			int numPivot = (this->numPivots>=task->size) ? task->size : this->numPivots;



			/*selecte several piovt from current partition based on the given parameter*/

			vector<int> pivotsIndex;

			pivotsIndex=this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivot);

			//if(task->getMyHeight()>=6)

			//{

			//	if((task->size-pivotsIndex.size())>maxLeafSize)

			//        cerr<<"task.Height:"<<task->getMyHeight()<<" INTER-NODE index:"<<task->getMyIndex()<<" task.start:"<<task->getStart()<<" task.size:"<<task->getSize()<<" pivotnum:"<<pivotsIndex.size()<<" NumPivots:"<<numPivots<<endl;

			//	//else

			//	//	cerr<<"task.Height:"<<task->getMyHeight()<<" LEAF-NODE index:"<<task->getMyIndex()<<" task.start:"<<task->getStart()<<" task.size:"<<task->getSize()<<" pivotnum:"<<pivotsIndex.size()<<" NumPivots:"<<numPivots<<endl;



			//}



			/*move the pivot to the end of current partition*/

			task->groupPivotsAtEnd(pivotsIndex);



			remainNodeSize=task->size - task->numPivots;//task->size-task->numPivots

			//LOG4CPLUS_TRACE(loggerObj,"remain size:"<<remainNodeSize);

			/*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/

			if(task->getFlag()==0)
			{

				if(remainNodeSize>maxLeafSize)
				{

					createInternalNodeToWrite(task);

					//if(task->getFlag()==1)

					//{

					//	taskList.erase(taskToGet);

					//	task.reset();

					//}

				}
				else
				{

					createAndWriteLeafNode(task,out);



					if(rootAddress!=-2)
					{

						taskList.erase(taskToGet);

						task.reset();

					}

					if(taskList.size()==1)

						break;

				}

			}
			else
			{
				if(maxTreeHeight <=0)
				{

					rootAddress = -2;

				}
				else
				{

					rootAddress = -1;

				}
				createAndWriteLeafNode(task,out);



				if(rootAddress!=-2)
				{

					taskList.erase(taskToGet);

					task.reset();

				}

				if(taskList.size()==1)

					break;

			}

		}
		else
		{

			out.open(fileName.c_str(),ios::in|ios::out|ios::binary);

			writeInternalNode(task,out);

			out.close();

			taskList.erase(taskToGet);

			task.reset();

			if(taskList.size()==1)

				break;

		}

		//clock_t endt=times(NULL);

		//clock_t endt=times(NULL);

		//{

		//	cout<<__FILE__<<__LINE__<<" time elapsed is "<<(endt-begt)/100.00<<endl;

		//	c=c+1;

		//}

	}



	writeRoot(taskList.at(0),out,maxTreeHeight);

	/*   out.close();*/



}



void CMVPIndex::loadRootToLeaf(vector<shared_ptr<CMetricData> > &dataObjects)
{

	ofstream out;



	out.open(fileName.c_str(),ios::binary);

	out.close();



	filePointer = 0;

	int maxTreeHeight=0;

	if(dataObjects.size()<=maxLeafSize)
	{

		rootAddress = -2;

	}
	else
	{

		rootAddress = -1;

	}



	vector<shared_ptr <Task> >::iterator taskToGet;



	/*put the task that create the root node to the task queue*/

	shared_ptr<Task> task(new Task(dataObjects,shared_ptr<CIndexNode>(),0,dataObjects.size(),0,0));

	task->myIndexAdress = -1;



	this->taskList.push_back(task);







	/*use every task in the queue to create a internal node or leaf node until the the queue is null*/

	while(this->taskList.size()>0)
	{

		taskToGet=this->taskList.end();

		int remainNodeSize;

		taskToGet--;

		task = *taskToGet;

		maxTreeHeight=(maxTreeHeight>=task->getMyHeight())?maxTreeHeight:task->getMyHeight();

		/*get the number of pivot*/

		int numPivot = (this->numPivots>=task->size) ? task->size : this->numPivots;

		/*selecte several piovt from current partition based on the given parameter*/

		vector<int> pivotsIndex;

		pivotsIndex=this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivot);



		/*move the pivot to the end of current partition*/

		task->groupPivotsAtEnd(pivotsIndex);



		remainNodeSize=task->size - task->numPivots;//task->size-task->numPivots



		/*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/

		if(remainNodeSize>maxLeafSize&&task->getFlag()==0)
		{

			createAndWriteInternalNode(task,out,taskToGet);

			if(task->getFlag()==0)

				task.reset();

		}
		else
		{
			if(maxTreeHeight <=0)
			{

				rootAddress = -2;

			}
			else
			{

				rootAddress = -1;

			}
			createAndWriteLeafNodeLast(task,out);

			if(rootAddress!=-2)
			{

				taskList.erase(taskToGet);

				task.reset();

			}
			else 
			{

				writeRoot(taskList.at(0),out,maxTreeHeight);

				task.reset();

				break;

			}

		}

	}



	rootAddress = 0;



	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	//out.seekp(-(long)sizeof(long),ios::end);

	//cout<<"filpos:"<<out.tellp()<<endl;

	out.seekp(0,ios::end);

	out.write((char*)&rootAddress,sizeof(long));

	out.close();

	/*   out.close();*/


}


void CMVPIndex::createAndWriteLeafNode(shared_ptr<Task> task,ofstream &out)
{



	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	out.seekp(filePointer);



	this->numLeaf++;

	/*cout<<"leafNode:"<<numLeaf<<endl;*/

	/*get all the objects of current partition*/

	vector<shared_ptr<CMetricData> > children;

	task->getDataPoints(children);



	/*get all the pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);



	vector<vector<double> > distance;



	/*calcualte the distance from each of the objects of current parition to every pivots*/

	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
	{

		vector<double> ve;



		for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
		{

			ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));

		}



		distance.push_back(ve);

	}



	/*create a leaf node and assign its memory address to the child list of parent node*/

	shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));



	if(this->rootAddress==-2)
	{        



		task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);



		out.close();

		return;

	}
	else
	{

		char type[6] = "LNODE";



		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=filePointer;

		out.seekp(filePointer);

		out.write(type,6*sizeof(char));

		filePointer+=(long)mvpLeafNode->writeExternal(out)+6*sizeof(char);

		/*delete(type);*/

	}



	out.close();

}



void CMVPIndex::createInternalNodeToWrite(shared_ptr<Task> task)
{







	this->numInternalNodes ++;

	/*cout<<"internalNode:"<<numInternalNodes<<endl;*/

	/*get pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);





	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/

	CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);

	int childrenNumber=pr.partitionSize();

	int flag=0;

	if(childrenNumber==2)
	{

		//double distance=this->metric->getDistance(task->dataList[task->start].get(),task->dataList[task->start+1].get());

		//for(int i=task->start+1;i<task->start+task->size-task->numPivots;i++)
		//{

		//	if(this->metric->getDistance(task->dataList[task->start].get(),task->dataList[i].get())==distance)
		//	{

		//		flag=1;

		//	}
		//	else
		//	{

		//		flag=0;

		//		break;

		//	}

		//}
		flag=1;

	}

	//	cerr<<"task.Height:"<<task->getMyHeight()<<" pivotsize:"<<pivots.size()<<" task.start:"<<task->start<<" size:"<<task->size<<" task->index:"<<task->getMyIndex()<<" childpartitionsize:"<<childrenNumber-1<<endl;



	//	if(task->getMyHeight()==1)

	//	{

	//		cout<<"task.height:"<<task->getMyHeight()<<" task.index:"<<task->getMyIndex()<<" childpartitionnum:"<<childrenNumber-1<<" flag:"<<flag<<" task-start:"<<task->start<<" task->size:"<<task->size<<" task.numPivots:"<<task->numPivots<<endl;

	//	}

	//if(task->getMyHeight()>=6)

	//	cerr<<"task.Height:"<<task->getMyHeight()<<" pivotsize:"<<pivots.size()<<" task.start:"<<task->start<<" size:"<<task->size<<" task->index:"<<task->getMyIndex()<<" childpartitionsize:"<<childrenNumber-1<<endl;

	/*create an internal node and assign its addres to the children list of parent node*/

	if(flag==0)
	{

		vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

		shared_ptr<CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );

		node->setChildSize(childrenNumber-1);

		//if(task->getMyHeight()==1)

		//{

		//	cout<<"task.height:"<<task->getMyHeight()<<" task.index:"<<task->getMyIndex()<<" childpartitionnum:"<<childrenNumber-1<<" flag:"<<flag<<" task-start:"<<task->start<<" task->size:"<<task->size<<" task.numPivots:"<<task->numPivots<<endl;

		//}



		task->setNode((shared_ptr<CIndexNode>) node );



		/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/

		for(int i=childrenNumber-1;i>0;i--)
		{

			int j=0;

			shared_ptr<Task> newTask( new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1) );       



			this->taskList.push_back(newTask);



		}

	}
	else
	{

		//cout<<"task.height:"<<task->getMyHeight()<<" task.index:"<<task->getMyIndex()<<" childpartitionnum:"<<childrenNumber-1<<" flag:"<<flag<<" task-start:"<<task->start<<" task->size:"<<" task.numPivots:"<<task->numPivots<<endl;

		task->setFlag(1);

		//shared_ptr<Task> newTask( new Task(task->dataList,task->getParentIndex(),pr.getPartition(0),pr.getPartition(0)+task->size,task->getMyIndex(),task->getMyHeight()));

		//newTask->setFlag(1);

		//this->taskList.push_back(newTask);

	}

}



void CMVPIndex::writeInternalNode(shared_ptr<Task>task,ofstream& out)
{

	//    out.open(fileName.c_str(),ios::binary|ios::in|ios::out);



	out.seekp(filePointer);



	CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());

	int num = task->getMyIndex();

	/*cout<<(parentNode->getSubTreeAddress())[0]<<endl;*/

	parentNode->getSubTreeAddress()[task->getMyIndex()-1]=filePointer;



	char *type = "INODE";

	out.write(type,6*sizeof(char));

	filePointer+=(long)task->getNode()->writeExternal(out)+6*sizeof(char);

	/* delete(type);*/



	//    out.close();

}



void CMVPIndex::writeRoot(shared_ptr <Task> task,ofstream& out,int treeHeight)
{ 

	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);



	out.seekp(filePointer);



	char *type1="INODE";

	char *type2="LNODE";



	if(rootAddress == -2)

		out.write(type2,6*sizeof(char));

	else

		out.write(type1,6*sizeof(char));



	task->getNode()->writeExternal(out);

	treeHeight++;

	rootAddress = filePointer;

	out.write((char*)(&treeHeight),sizeof(int));

	out.write((char*)(&rootAddress),sizeof(long));

	out.close();
	cout<<"Root address:"<<rootAddress<<" height of MVP-tree:"<<treeHeight<<endl;
	//delete(type1);
	//delete(type2);

}



void CMVPIndex::createAndWriteInternalNode(shared_ptr <Task> task,ofstream& out,vector<shared_ptr<Task> >::iterator &shouldeBeCreate)
{

	this->numInternalNodes ++;

	/*cout<<"internalNode:"<<numInternalNodes<<endl;*/

	/*get pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);



	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/

	CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize,this->trisectionRadius,this->selectOptimalPivots,this->partitionByOnePivot);

	int childrenNumber=pr.partitionSize();

	int flag=0;

	if(childrenNumber==2)
	{
		flag=1;
		/*double distance=this->metric->getDistance(task->dataList[task->start].get(),task->dataList[task->start+1].get());

		for(int i=task->start+1;i<task->start+task->size-task->numPivots;i++)
		{

			if(this->metric->getDistance(task->dataList[task->start].get(),task->dataList[i].get())==distance)
			{

				flag=1;

			}
			else
			{

				flag=0;

				break;

			}

		}*/

	}



	//if(task->getMyHeight()==1)

	//{

	//	cout<<"task.height:"<<task->getMyHeight()<<" task.index:"<<task->getMyIndex()<<" childpartitionnum:"<<childrenNumber-1<<" flag:"<<flag<<" task-start:"<<task->start<<" task->size:"<<task->size<<" task.numPivots:"<<task->numPivots<<endl;

	//}



	if(flag==0)
	{

		/*create an internal node and assign its addres to the children list of parent node*/

		vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);

		shared_ptr< CMVPInternalNode> node( new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );

		node->setChildSize(childrenNumber-1);



		//if(task->getMyHeight()==1)

		//{

		//	cout<<"task.height:"<<task->getMyHeight()<<" task.index:"<<task->getMyIndex()<<" childpartitionnum:"<<childrenNumber-1<<" flag:"<<flag<<" task-start:"<<task->start<<" task->size:"<<task->size<<" task.numPivots:"<<task->numPivots<<endl;

		//}



		if(task->getParentIndex()!=NULL)
		{

			(((CMVPInternalNode*)(task->getParentIndex().get()))->getSubTreeAddress())[task->myIndex-1]=-1;

		}    





		out.open(fileName.c_str(),ios::binary|ios::in|ios::out);



		out.seekp(filePointer);



		/* CMVPInternalNode* parentNode = (CMVPInternalNode*)task->getParentIndex();*/

		if(task->myIndexAdress!=-1)
		{

			out.seekp(task->myIndexAdress);

			out.write((char*)(&filePointer),sizeof(long));

			out.seekp(filePointer);

		}



		char *type = "INODE";

		out.write(type,6*sizeof(char));

		long tempDelta = node->writeExternal(out)+6*sizeof(char);

		/* delete(type);*/



		out.close();



		taskList.erase(shouldeBeCreate);



		/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/

		for(int i=childrenNumber-1;i>0;i--)
		{

			shared_ptr <Task> newTask ( new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));



			newTask->myIndexAdress = (long) (filePointer+(newTask->myIndex-1)*sizeof(long)+sizeof(int))+6*sizeof(char);



			this->taskList.push_back(newTask);



		}



		filePointer += tempDelta;

	}

	else
	{

		//cout<<"task.height:"<<task->getMyHeight()<<" task.index:"<<task->getMyIndex()<<" childpartitionnum:"<<childrenNumber-1<<" flag:"<<flag<<" task-start:"<<task->start<<" task->size:"<<task->size<<" task.numPivots:"<<task->numPivots<<endl;

		task->setFlag(1);

	}

}



void CMVPIndex::createAndWriteLeafNodeLast(shared_ptr <Task> task,ofstream &out)
{



	out.open(fileName.c_str(),ios::binary|ios::in|ios::out);

	out.seekp(filePointer);



	this->numLeaf++;

	/*cout<<"leafNode:"<<numLeaf<<endl;*/

	/*get all the objects of current partition*/

	vector<shared_ptr<CMetricData> > children;

	task->getDataPoints(children);



	/*get all the pivots of current node*/

	vector<shared_ptr<CMetricData> > pivots;

	task->getPivots(pivots);



	vector<vector<double> > distance;



	/*calcualte the distance from each of the objects of current parition to every pivots*/

	for(vector<CMetricData*>::size_type i=0;i<pivots.size();i++)
	{

		vector<double> ve;



		for(vector<CMetricData*>::size_type j=0;j<children.size();j++)
		{

			ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));

		}



		distance.push_back(ve);

	}



	/*create a leaf node and assign its memory address to the child list of parent node*/

	shared_ptr< CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));



	if(this->rootAddress==-2)
	{        



		task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);



		out.close();

		return;

	}
	else
	{

		char type[6] = "LNODE";





		out.seekp(task->myIndexAdress);

		out.write((char*)(&filePointer),sizeof(long));

		out.seekp(filePointer);



		out.write(type,6*sizeof(char));

		filePointer+=(long)mvpLeafNode->writeExternal(out)+6*sizeof(char);

		/*delete(type);*/

	}



	out.close();

}

