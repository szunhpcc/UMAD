
#include "../../HeaderFiles/classifier/ReadTrainLabel.h"

/**
 *@ according to thr requirements of our segmentation data
 *@ param &s expression the primary dataset
 *@ param delim: delim expression various punctuation
 */
vector<string> split(string &s,char delim)
{
	stringstream ss(s);
	string str1;
	vector<string> results;
	while (getline(ss,str1,delim))
		results.push_back(str1);
	return results;
}

/**
 *@ store data number and the class label of train data
 *@ param fileName: fileName is the primary data file that store attribute and class label
 *@ param maxDataNum: the total number of the data list
 */
map<int,string> CReadTrainLabel::loadLabel(string fileName,int maxDataNum)
{
	string str;
	ifstream in(fileName);
	vector <string> data;
	map<int,string> id_label;
	if(!in)
	{
		cout<<"can't open the file."<<endl;
	}
	for(int i=0;i<maxDataNum;i++)
	{
		getline(in,str);
		data=split(str,' ');
		id_label.insert(make_pair(i-1,data.back()));
		data.pop_back();
	}
	return id_label;
}

