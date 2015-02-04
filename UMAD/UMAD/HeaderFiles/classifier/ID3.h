#include <iostream>  
#include <string>  
#include <vector>  
#include <map>  
#include <algorithm>  
#include <cmath>  
using namespace std;  
#define MAXLEN 6//输入每行的数据个数  

/*多叉树的实现   
* @1 广义表  
* @2 父指针表示法，适于经常找父结点的应用  
* @3 子女链表示法，适于经常找子结点的应用  
* @4 左长子，右兄弟表示法,实现比较麻烦  
* @5 每个结点的所有孩子用vector保存  
* @教训:数据结构的设计很重要，本算法采用5比较合适，同时  
* @注意维护剩余样例和剩余属性信息，建树时横向遍历靠循环属性的值，纵向遍历靠递归调用  
*/
  
class CID3
{
public:
	vector <vector <string> > state;//实例集  
	vector <string> item(MAXLEN);//对应一行实例集  
	vector <string> attribute_row;//保存首行即属性行数据  
	string end("end");//输入结束  
	string yes("yes");  
	string no("no");  
	string blank(" ");  
	map<string,vector < string > > map_attribute_values;//存储属性对应的所有的值  
	int tree_size = 0;  
	struct Node//决策树节点
	{  
		string attribute;//属性值  
		string arrived_value;//到达的属性值  
		vector<Node *> childs;//所有的孩子  
		Node()
		{  
			attribute = blank;  
			arrived_value = blank;  
	    }  
	};  Node * root;  

	virtual void ComputeMapFrom2DVector();//根据数据实例计算属性与值组成的map  
	virtual double ComputeEntropy(vector <vector <string> > remain_state, string attribute, string value,bool ifparent);//根据具体属性和值来计算熵 
	virtual double ComputeGain(vector <vector <string> > remain_state, string attribute);//计算按照属性attribute划分当前剩余实例的信息增益
	virtual int FindAttriNumByName(string attri);
	virtual string MostCommonLabel(vector <vector <string> > remain_state);//找出样例中占多数的正/负性
	virtual bool AllTheSameLabel(vector <vector <string> > remain_state, string label);//判断样例是否正负性都为label

	//计算信息增益，DFS构建决策树  
	//current_node为当前的节点  
	//remain_state为剩余待分类的样例  
	//remian_attribute为剩余还没有考虑的属性  
	//返回根结点指针  
	virtual Node * BulidDecisionTreeDFS(Node * p, vector <vector <string> > remain_state, vector <string> remain_attribute);
	virtual void Input();
	virtual void PrintTree(Node *p, int depth);
	virtual void FreeTree(Node *p);
};


/*输入的训练数据如下
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