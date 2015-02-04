#include <iostream>  
#include <string>  
#include <vector>  
#include <map>  
#include <algorithm>  
#include <cmath>  
using namespace std;  
#define MAXLEN 6//����ÿ�е����ݸ���  

/*�������ʵ��   
* @1 �����  
* @2 ��ָ���ʾ�������ھ����Ҹ�����Ӧ��  
* @3 ��Ů����ʾ�������ھ������ӽ���Ӧ��  
* @4 ���ӣ����ֵܱ�ʾ��,ʵ�ֱȽ��鷳  
* @5 ÿ���������к�����vector����  
* @��ѵ:���ݽṹ����ƺ���Ҫ�����㷨����5�ȽϺ��ʣ�ͬʱ  
* @ע��ά��ʣ��������ʣ��������Ϣ������ʱ���������ѭ�����Ե�ֵ������������ݹ����  
*/
  
class CID3
{
public:
	vector <vector <string> > state;//ʵ����  
	vector <string> item(MAXLEN);//��Ӧһ��ʵ����  
	vector <string> attribute_row;//�������м�����������  
	string end("end");//�������  
	string yes("yes");  
	string no("no");  
	string blank(" ");  
	map<string,vector < string > > map_attribute_values;//�洢���Զ�Ӧ�����е�ֵ  
	int tree_size = 0;  
	struct Node//�������ڵ�
	{  
		string attribute;//����ֵ  
		string arrived_value;//���������ֵ  
		vector<Node *> childs;//���еĺ���  
		Node()
		{  
			attribute = blank;  
			arrived_value = blank;  
	    }  
	};  Node * root;  

	virtual void ComputeMapFrom2DVector();//��������ʵ������������ֵ��ɵ�map  
	virtual double ComputeEntropy(vector <vector <string> > remain_state, string attribute, string value,bool ifparent);//���ݾ������Ժ�ֵ�������� 
	virtual double ComputeGain(vector <vector <string> > remain_state, string attribute);//���㰴������attribute���ֵ�ǰʣ��ʵ������Ϣ����
	virtual int FindAttriNumByName(string attri);
	virtual string MostCommonLabel(vector <vector <string> > remain_state);//�ҳ�������ռ��������/����
	virtual bool AllTheSameLabel(vector <vector <string> > remain_state, string label);//�ж������Ƿ������Զ�Ϊlabel

	//������Ϣ���棬DFS����������  
	//current_nodeΪ��ǰ�Ľڵ�  
	//remain_stateΪʣ������������  
	//remian_attributeΪʣ�໹û�п��ǵ�����  
	//���ظ����ָ��  
	virtual Node * BulidDecisionTreeDFS(Node * p, vector <vector <string> > remain_state, vector <string> remain_attribute);
	virtual void Input();
	virtual void PrintTree(Node *p, int depth);
	virtual void FreeTree(Node *p);
};


/*�����ѵ����������
[plain] view plaincopy 
Day Outlook Temperature Humidity Wind PlayTennis  
1 Sunny Hot High Weak no  
2 Sunny Hot High Strong no  
3 Overcast Hot High Weak yes  
4 Rainy Mild High Weak yes  
5 Rainy Cool Normal Weak yes  
6 Rainy Cool Normal Strong no  
7 Overcast Cool Normal Strong yes  
8 Sunny Mild High Weak no  
9 Sunny Cool Normal Weak yes  
10 Rainy Mild Normal Weak yes  
11 Sunny Mild Normal Strong yes  
12 Overcast Mild High Strong yes  
13 Overcast Hot Normal Weak yes  
14 Rainy Mild High Strong no  
end  
*/