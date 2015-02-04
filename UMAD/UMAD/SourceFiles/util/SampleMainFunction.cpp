#include "../../HeaderFiles/index/MVPIndex.h"
//#include "../HeaderFiles/index/ListIndex.h"
#include "../../HeaderFiles/index/FFTPivotSelectionMethod.h"
#include "../../HeaderFiles/index/BalancePartitionMethod.h"
#include "../../HeaderFiles/metricdata/DoubleVector.h"
#include "../../HeaderFiles/metricdistance/EuclideanDistance.h"
#include "../../HeaderFiles/metricdistance/CountedMetric.h"


#include <string>
#include <iostream>
#include <fstream>
using namespace std;


/**
�˺����򵥵�չʾ��һ�¿�����Ա���ʹ�ñ�ϵͳ�ṩ��API����һ��������Ϊ���ڼ䣬ֻ������MVP�������ṹ��ŷ����þ������������������
������Ա�����Լ�����һ����������������ͣ����ñ�ϵͳ�ṩ��mvp-tree�����ṹ�������������ṹ�����о�
ʹ�ñ�ϵͳ�����Ϸ�Ϊ������
1 ��������
2 �ڹ����������н��в���
*/
//int main()
//{
//	/******************************׼������*****************************************/
//	string rawDatafileName=".\\data\\2000000rawdata.txt";                      //ԭʼ�������ڵ��ļ����ļ���
//	string indexFileName=".\\data\\vector-1000-5.db";						   //�����洢���ļ���
//	CObjectFactory objectF();
//	CObjectFactory::objectRegister("vector",CDoubleVector::getConstructor());  //����vector��Ϊ���ݶ���
//	CPartitionMethod *pm=new CBalancePartitionMethod();				           //����ƽ�⻮�ַ���
//	CPivotSelectionMethod *psm=new CFFTPivotSelectionMethod();                 //����first farthest֧�ŵ�ѡ�񷽷�
//	CountedMetric *m=new CountedMetric(new CEuclideanDistance);			       //����ŷ����þ���
//
//
//
//	/******************************��������*****************************************/
//	//1 ���ļ��ж�ȡԭʼ����,��ԭʼ���ݷ�װ��CDoubleVector���󣬲���������б���
//		vector<shared_ptr<CMetricData> > *dataObjectList=0;					//����������CDoubleVector����
//		dataObjectList=CDoubleVector::loadData(rawDatafileName,1000,5);
//
//	//2 �û�ȡ�Ķ����б�������,����bulkload������ʵ�������Ĺ���, ���������в���д��Ӳ��
//		
//		//CIndex *index=new CListIndex(m);										//�������������ṹ	
//		CIndex *index=new CMVPIndex(*dataObjectList,m,psm,pm,2,3,100);          //����mvp�������ṹ
//		index->setFileName(indexFileName);
//		index->setDataType("vector");
//
//		index->bulkLoad(*dataObjectList,0);
//
//
//
//	/*******************************�������в���*************************************/
//	//1 ������ѯ����Q
//		//��Χ��ѯ��Ŀ�����
//		double data[5]={20,30,40,50,10};                                      //������������
//		shared_ptr<CMetricData> t(new CDoubleVector(data,5));                //���������ݷ�װ�����ݶ���
//		//��Χ��ѯ�ķ�Χֵ
//		double radius=35;                                                     //������Χ���ҵķ�Χֵ
//		//��Χ��ѯ����
//		CRangeQuery *q=new CRangeQuery(radius,t);                             //������ѯ����
//	//2 ���ļ��ж�ȡ������Ϊ��ѯʱ�����ݿ�
//		//CIndex* newIndex=new CListIndex(m);
//		//newIndex->readExternal(indexFileName);
//		CMVPIndex *newIndex=new CMVPIndex();                                  //�����µ�����ָ�룬����Ҫֱ�ߴ��ļ���ȡ������
//		newIndex->setFileName(indexFileName);
//		newIndex->setDataType("vector");
//		newIndex->setMetric(m);
//
//	//3 �ڶ�ȡ�����ݿ��н��в���
//		vector<shared_ptr<CMetricData> > *result=0;
//		result=newIndex->search(q);                                          //����search�������в�ѯ
//		cout<<"result.size:"<<result->size()<<endl;
//
//	return 0;
//}