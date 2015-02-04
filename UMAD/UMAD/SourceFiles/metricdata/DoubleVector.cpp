/** @file DoubleVector.cpp

* @describe a kind of object

* @author Fuli Lei

* @version 2012-12-09

*/

#include "../../HeaderFiles/metricdata/DoubleVector.h"





#include <sstream>



/**@brief none parameter constructor*/

CDoubleVector::CDoubleVector()

{

    dim=0;

    dataList=0;



}



/**@brief constructor with two parameters

* @param data a double array represents the liner structure

* @param length length of the liner structure

*/

CDoubleVector::CDoubleVector(double *data,int length)

{

    dim = length;

    dataList = new double[dim];

    for (int i = 0;i<dim;i++)

    {

        dataList[i]=data[i];

    }





}



/**@brief destructure*/

CDoubleVector::~CDoubleVector()

{

    delete[](dataList);        

    /*cout<<"release CDoubleVector"<<endl;*/

}



/**@brief get the data list encapsulated in the objects

* @return return the memory address of the data list

*/

double* CDoubleVector::getData() const

{

    return dataList;

}



/**@brief get the length of the data list

*@return return an value of int represents the length of the data list.

*/

int CDoubleVector::getLen() const

{

    return dim;

}





/**@brief load raw data from hard disk file and package the data into a objects of CDoubleVector,then return the vector with all generated objects

* @param fileName name of the file that contains all the raw data waiting to be load

* @param maxDataNum maximum number of data list to be load from the file

* @param dimension length of each data list

* @return  return a vector contains all the objects generated before.

*/

vector<shared_ptr<CMetricData> >* CDoubleVector::loadData(string fileName,int maxDataNum,int dimension)
{

	ifstream in(fileName.c_str(),std::ios_base::in);


    if(!in)
    {

		cout<<__FILE__<<"¡¡"<<__LINE__<<"open raw data file:"<<fileName<<" failed!"<<endl;

        exit(0);

    }



    int num, dim, i, j;

    vector<shared_ptr<CMetricData> > *a=new vector<shared_ptr<CMetricData> >;

    double* data=NULL;

    shared_ptr<CDoubleVector> temp=shared_ptr<CDoubleVector>();

    string str="";

    in >> dim >> num;	

    getline(in,str);

    dim = dim>dimension ? dimension:dim;
    num = num>maxDataNum ? maxDataNum:num;


    for(i=0;i<num;i++)
    {

        getline(in,str);

        stringstream newStr(str);

        data = new double[dim];



        for(j=0; j<dim; j++)
        {

            newStr>>data[j];

        }



        temp.reset(new CDoubleVector(data, dim));

        a->push_back(temp);

    }


    return a;

}


/**@brief write the instance of this class to hard disk

*@param out out putstream,used to write inforamtion to the hard disk

*@return return the size of information which has been wrote to the hard disk

*/

int CDoubleVector ::writeExternal(ofstream &out)

{

    int size=0;

    out.write((char*) (&dim),sizeof(int));

    size += sizeof(int);

    /*for(int i =0;i<dim;i++)

    {

    out.write((char*)(&dataList[i]),sizeof(double));

    size += sizeof(double);

    }*/

    out.write((char*)dataList,dim*sizeof(double));

    size += dim*sizeof(double);



    return size;



}



/**@load a instance of this class from the file stored in hard disk

*@param in input stream,used to read information from hard disk

*@return return the size of information which has been read from the hard disk

*/

int CDoubleVector ::readExternal(ifstream &in)

{

    int size=0;     



    in.read((char*)(&dim),sizeof(int));    

    size+=sizeof(int);



    dataList = new double[dim];



    in.read((char*)dataList,dim*sizeof(double));    

    size+=dim*sizeof(double);



    return size;



}



/**@brief return the name of a instance of this class

*@return return the name of a instance of this class

*/

CreateFuntion CDoubleVector::getConstructor()

{

    CreateFuntion constructor =& CreateInstance;

    return constructor;

}



void* CDoubleVector:: CreateInstance()

{

    return new CDoubleVector();

}



















