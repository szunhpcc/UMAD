#include"../../../HeaderFiles/classifier/NavieBayes/PredictClass.h"
#define space(s) (s == ' ' || s == '\n' || s == '\t')
#define  SkipComment	while ( ( c = getc(f) ) != '\n' )
#define p 1/3
//#define _SCL_SECURE_NO_WARNINGS  

#pragma warning(disable:4996)   
PredictClass::PredictClass(GetInfo *i,GetModel *m,string name,string Attname):MaxClassNo(-1),MaxDiscrValNo(2),MaxAttNo (-1)
{
	gi=i;
	gm=m;
	filename=name;
	this->Inc=1024;
	AttFileName=Attname;
	TruePositive=0;
	FalsePositive=0;
	accuracy=0.0;
}
void PredictClass::Predict(const char *showResultFileName)
{   
	float *prob;  //store posteriori probability of each class
	int BestI=-1;
	float MaxProb=0;
	int j;
	GetName();
	GetData();
    prob=(float *)calloc(MaxClassNo+1,sizeof(float));
	for(int i=0;i<=MaxClassNo;++i)
	    prob[i]=1.0;
	for(int i=0;i<=MaxItemNo;++i)
	{
		for(j=0;j<=MaxClassNo;++j)
		{   
			for(int k=0;k<=MaxAttNo;++k)
			{   
				/*
				 ***Using m estimation to calculate the conditional probability：p(x/y)=(nc+mp)/(n+m)
				 */
				if(MaxAttValNo[k])
					prob[j]*=((gm->PostFreq[j][k][Item[i][k].DiscrValue]+gi->ClassFreq[j]*p)/(gi->ClassFreq[j]+gi->ClassFreq[j]));
				else
				{
					if(Item[i][k].continuousVal<=gm->SplitPoint[k])
						prob[j]*=((gm->PostFreq[j][k][1]+gi->ClassFreq[j]*p)/(gi->ClassFreq[j]+gi->ClassFreq[j]));  //prob[j]=p(x/y)=p(x1/y=j)*p(x2/y=j)*p(x3/y=j)*p(x4/y=j)
					else
						prob[j]*=((gm->PostFreq[j][k][2]+gi->ClassFreq[j]*p)/(gi->ClassFreq[j]+gi->ClassFreq[j]));
				}

			}
			prob[j]*=(gi->ClassFreq[j]/(gi->MaxItemNo+1));   
			if(prob[j]>MaxProb)   
		    {
			    MaxProb=prob[j];  //Store the value of maximum a posteriori probability
                BestI=j;   //get the subscript of maximun a posteriori probability
		    }
		}

		cout<<"概率最大="<<MaxProb<<" 类别="<<ClassName[BestI]<<endl;
		Item[i][MaxAttNo+1].DiscrValue=BestI;

		/*====================================================================== new content =================================================================*/
		if(ClassName[BestI]==gi->testdatalabel[i])
			TruePositive++;
		else
			FalsePositive++;
		/*====================================================================================================================================================*/

		/*reset*/
		{
            BestI=-1;
	        MaxProb=0;
			for(int l=0;l<=MaxClassNo;++l)
				prob[l]=1.0;
		}
	}
	show(showResultFileName);
}
void PredictClass::show(const char *resultFileName)
{
	ofstream outfile(resultFileName,ofstream::out);
	int i,j;
	for( i=0;i<=MaxItemNo;i++)
	{   
		outfile<<i+1<<" : ";
		for( j=0;j<=MaxAttNo;j++)
		{
			if(MaxAttValNo[j])
				outfile<<AttValName[j][Item[i][j].DiscrValue]<<" ";
			else
				outfile<<Item[i][j].continuousVal<<" ";
		}
		outfile<<"预测结果为:"<<ClassName[Item[i][j].DiscrValue]<<endl;
	}

	/*===================================================== new content：show classification accuracy =====================================================================*/
	outfile<<"TruePositive:"<<TruePositive<<endl;
	outfile<<"FalsePositive:"<<FalsePositive<<endl;
	accuracy=double(TruePositive)/(MaxItemNo+1);
	outfile<<"Accuracy:"<<accuracy<<endl;
	/*=====================================================================================================================================================================*/
	outfile.close();
}
void PredictClass::GetData()
{   
	FILE *Nf;
	char Fn[50];     
	filename=filename+".data";
	filename.copy(Fn,filename.length());
	Fn[filename.length()]=NULL;
	if ( ! ( Nf = fopen(Fn, "r") ) ) 
		Error(0, Fn, "");
	int ItemNo=0;
	int	ItemSpace=0;
	do
    {
	    MaxItemNo = ItemNo;
	    /*  Make sure there is room for another item  */
	    if ( ItemNo >= ItemSpace )
	    {
	        if ( ItemSpace )
	        {
		        ItemSpace += Inc;
		        Item = (Description *)realloc(Item, ItemSpace*sizeof(Description));
	        }
			else
	        {
		        Item = (Description *)malloc((ItemSpace=Inc)*sizeof(Description));
	         }
	    }
	    Item[ItemNo] = GetDescription(Nf);
    }while ( Item[ItemNo] != 0 && ++ItemNo);
	fclose(Nf);
    MaxItemNo=ItemNo - 1;
}
void   PredictClass::Error(int n, string s1, string s2)
/*  -----  */
{
    cout<<"ERROR:  ";
    switch(n)
    {
	case 0: cout<<"cannot open file "<<s1<<s2<<endl;
		break;

	case 1:	cout<<"colon expected after attribute name "<<s1<<endl;
		break;

	case 2:	cout<<"unexpected EOF while reading attribute "<< s1<<endl;
		break;

	case 3:cout<<"attribute "<<s1<<" has only one value"<<endl;
		break;

	case 4:cout<<"case "<< MaxItemNo+1<<"'s value of '"<<s2<<"' for attribute "<<s1<<"is illegal"<<endl;//Error(4, AttName[AttNo], name);		   
		break;

	case 5:cout<<"case "<<MaxItemNo+1<<"'s class of '"<<s2<<"' is illegal"<<endl;
		break;
    } 
	cout<<"process stop!"<<endl;
    exit(1);
}
Description PredictClass::GetDescription(FILE *Df)
{
	int AttNo;/* attribute number, 0..MaxAttNo */
    char name[50], *endname;
    int Dv;
    float Cv;
    Description Dvec;
    if ( ReadName(Df, name) )
    {
    	Dvec = (Description)calloc(MaxAttNo+2, sizeof(AttValue));
        for(AttNo=0;AttNo<=MaxAttNo;AttNo++)
        {
	        if ( MaxAttValNo[AttNo]) 
	        {
		        /*  Discrete value  */ 
		        Dv = Which(name, AttValName[AttNo], 1, MaxAttValNo[AttNo]);
		        if ( ! Dv )
		        {			       
			        Error(4, AttName[AttNo], name);
		        }
	            Dvec[AttNo].DiscrValue=Dv;
	        }
	        else
	        {
		        /*  Continuous value  */
		        Cv = (float)strtod(name, &endname);
		        if ( endname == name || *endname != '\0' )
			            Error(4, AttName[AttNo], name);
				Dvec[AttNo].continuousVal=Cv;
	        }
	        ReadName(Df,name);
        }
	    return Dvec;
    }
    else
    {
	    return 0;
    }
}
void PredictClass::GetName()
{   
	FILE *Nf;
	char Buffer[1000];
	char Fn[100];
	int AttCeiling=100;
	int ClassCeiling=100;
	int	ValCeiling;
	AttFileName.copy(Fn,AttFileName.length());
	Fn[AttFileName.length()]=NULL;
    strcat(Fn,".names");
	if ( ! ( Nf = fopen(Fn, "r") ) ) 
		Error(0, Fn, "");
	ClassName = (string *) calloc(ClassCeiling, sizeof(string));
	do
    {
	    ReadName(Nf, Buffer);
	    if ( ++MaxClassNo >= ClassCeiling)
	    {
	        ClassCeiling += 100;
	        ClassName = (string *) realloc(ClassName, ClassCeiling*sizeof(string));
	    }
	    ClassName[MaxClassNo]=string(Buffer);
    }
    while ( Delimiter == ',' );
	/*  Get attribute and attribute value names from names file  */
    AttName = (string *) calloc(AttCeiling, sizeof(string));
	MaxAttValNo = (short *) calloc(AttCeiling, sizeof(short));
	AttValName = (string **) calloc(AttCeiling, sizeof(string *));
    //SpecialStatus = (char *) malloc(AttCeiling);
    while ( ReadName(Nf, Buffer) )
    {
	    if ( Delimiter != ':' ) 
			Error(1, Buffer, "");
        if ( ++MaxAttNo >= AttCeiling )//expand space
	    {
	        AttCeiling += 100;
	        AttName = (string *) realloc(AttName, AttCeiling*sizeof(string));
	        MaxAttValNo = (short *) realloc(MaxAttValNo, AttCeiling*sizeof(short));
	        AttValName = (string **) realloc(AttValName, AttCeiling*sizeof(string *));
	        //SpecialStatus = (char *) realloc(SpecialStatus, AttCeiling);
	    }
	    AttName[MaxAttNo] = string(Buffer);
	    //SpecialStatus[MaxAttNo] = 0;
	    MaxAttValNo[MaxAttNo] = 0;
	    ValCeiling = 100;
	    AttValName[MaxAttNo] = (string *) calloc(ValCeiling, sizeof(string));
	    do
	    {
	        if ( ! ( ReadName(Nf, Buffer) ) ) 
				Error(2, AttName[MaxAttNo], "");
	        if ( ++MaxAttValNo[MaxAttNo] >= ValCeiling )
	        {
		        ValCeiling += 100;
		        AttValName[MaxAttNo] =(string *) realloc(AttValName[MaxAttNo], ValCeiling*sizeof(string));
	        }
            AttValName[MaxAttNo][MaxAttValNo[MaxAttNo]] = string(Buffer);
	    }while ( Delimiter == ',' );
	    if ( MaxAttValNo[MaxAttNo] == 1 )
	    {
	        /*  Check for special treatment  */
	        if (!strcmp(Buffer, "continuous") )
	        {	
				//MaxContAttNo++;
			}
	         else
	        {
		     /*  Cannot have only one discrete value for an attribute  */
		         Error(3, AttName[MaxAttNo], "");
	        }
	        MaxAttValNo[MaxAttNo] = 0;
	    }
	    else if ( MaxAttValNo[MaxAttNo] > MaxDiscrValNo ) 
			MaxDiscrValNo = MaxAttValNo[MaxAttNo];
    }
    fclose(Nf);
}
bool PredictClass::ReadName(FILE *f,char *Buffer)
{
	register char *Sp=Buffer;
	register int c;

	 while ( ( c = getc(f) ) == '|' || space(c) )
    {
	    if ( c == '|' ) 
			SkipComment;
    }

    if ( c == EOF )
    {
	    Delimiter = EOF;
	    return false;
    }
	//read data
	 while ( c != ':' && c != ',' && c != '\n' && c != '|' && c != EOF )
    {
	    if ( c == '.' )
	    {
	        if ( ( c = getc(f) ) == '|' || space(c) ) 
				break;
	        *Sp++ = '.';
	    }

	    if ( c == '\\' )
	    {
	        c = getc(f);
	    }
	    *Sp++ = c;
	    if ( c == ' ' )
	    {
	        while ( ( c = getc(f) ) == ' ' );
	    }
	    else
	    {
	        c = getc(f);
	    }
    }

    if ( c == '|' ) 
	    SkipComment;
    Delimiter = c;
    /*  Strip trailing spaces  */
    while ( space(*(Sp-1)) ) Sp--;

    *Sp++ = '\0';
    return true;
}
int PredictClass::Which(string Val,string List[],short First,short Last)
/*  -----  */
{
    short n=First;
    while ( n <= Last && Val!=List[n])
		n++;
    return ( n <= Last ? n : First-1 );
}