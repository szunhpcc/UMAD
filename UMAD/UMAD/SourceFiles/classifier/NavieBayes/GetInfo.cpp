#include"../../../HeaderFiles/classifier/NavieBayes/GetInfo.h"
#define space(s) (s == ' ' || s == '\n' || s == '\t')
#define  SkipComment	while ( ( c = getc(f) ) != '\n' )
//#define _SCL_SECURE_NO_WARNINGS  
#pragma warning(disable:4996)   //Disable call any one of potentially unsafe in the standard C ++ library compiler warning£¨level 3£©C4996


void GetInfo::getname()
{   
	char Buffer[1000];
	char Fn[100];
	int AttCeiling=100;
	int ClassCeiling=100;
	int	ValCeiling;
	auto i=filename.begin();
	auto j=filename.end();
	copy(i,j,Fn);
	Fn[filename.length()]=NULL;
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
    SpecialStatus = (char *) malloc(AttCeiling);
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
	        SpecialStatus = (char *) realloc(SpecialStatus, AttCeiling);
	    }
	    AttName[MaxAttNo] = string(Buffer);
	    SpecialStatus[MaxAttNo] = 0;
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
				MaxContAttNo++;
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
void   GetInfo::Error(int n, string s1, string s2)
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
bool GetInfo::ReadName(FILE *f,char *Buffer)
{
	register char *Sp=Buffer;
	register int c;
	//Skip the comment Begin with the '|'
	 while ( ( c = getc(f) ) == '|' || space(c) )
    {
	    if ( c == '|' ) 
			SkipComment;
    }
	//Returns false if there is no data in addition to the comments
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
	        if ( ( c = getc(f) ) == '|' || space(c) ) //In '. 'if it is behind the carriage returns annotation quit
				break;
	        *Sp++ = '.';
	    }

	    if ( c == '\\' )//skip if it is '\\'
	    {
	        c = getc(f);
	    }
	    *Sp++ = c;
	    if ( c == ' ' )//skip spaces
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
void GetInfo::getdata()
{
	char Fn[100];
	filename=filename+".data";
	filename.copy(Fn,filename.length());
	Fn[filename.length()]=NULL;
	if ( ! ( Nf = fopen(Fn, "r") ) ) 
		Error(0, Fn, "");
	int ItemNo=0;
	int	ItemSpace=0;

	//================================================== new content£ºstore true class label of test set ======================================================
	testlabel=testlabel+".data";
	ifstream in(testlabel);
	string str;
	while(getline(in,str))
	{
		testdatalabel.push_back(str);
	}
	//=========================================================================================================================================================

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
	InitialiseData();
	InitialiseWeights();
	//SplitContinuousAtt(1);
	AnalyzeData();
}
Description GetInfo::GetDescription(FILE *Df)
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
        if((Dv = Which(name, ClassName, 0, MaxClassNo)) < 0 )
        {
	        Error(5,"",name);
        }
		Dvec[MaxAttNo+1].DiscrValue=Dv;//the position of classlabel in ClassName
	    return Dvec;
    }
    else
    {
	    return 0;
    }
}
/**********************************************************************************************/
/*									                                                          */
/*	Locate value Val in List[First] to List[Last]			                                  */
/**********************************************************************************************/


int GetInfo::Which(string Val,string List[],short First,short Last)
/*  -----  */
{
    short n=First;
    while ( n <= Last && Val!=List[n])
		n++;
    return ( n <= Last ? n : First-1 );
}
void GetInfo::AnalyzeData()
{   
	for(int ClassNo=0;ClassNo<=MaxClassNo;ClassNo++)//initialize class value frequency
    {
	    ClassFreq[ClassNo] = 0;
    }
	for(int ItemNo=0;ItemNo<=MaxItemNo;ItemNo++)//statistics class value frequency
    { 
		ClassFreq[Item[ItemNo][MaxAttNo+1].DiscrValue] += Weight[ItemNo];//MaxAttNo+1 is class attribute£¬0~MaxAttNo is none class attribute
    } 
}
void GetInfo::InitialiseData()
{
	ClassFreq = (float *) calloc(MaxClassNo+1, sizeof(float));
	Weight = (float *) calloc(MaxItemNo+1, sizeof(float));
	Freq  = (float **) calloc(MaxDiscrValNo+1, sizeof(float *));
    for(int DiscrValueNo=0;DiscrValueNo<=MaxDiscrValNo;DiscrValueNo++)
    {
	    Freq[DiscrValueNo]=(float *) calloc(MaxClassNo+1, sizeof(float));
    }
	ValFreq= (float *) calloc(MaxDiscrValNo+1, sizeof(float));
	SplitGain = (float *) calloc(MaxItemNo+1, sizeof(float));
    SplitInfo = (float *) calloc(MaxItemNo+1, sizeof(float));
	Gain	= (float *) calloc(MaxContAttNo+1, sizeof(float));
    //Info	= (float *) calloc(MaxContAttNo+1, sizeof(float));
    Bar		= (float *) calloc(MaxContAttNo+1, sizeof(float));
}

/*************************************************************************/
/*								 	 */
/*		Initialise the weight of each item		 	 */
/*								 	 */
/*************************************************************************/
void GetInfo::InitialiseWeights()
/*  -----------------  */
{
    int ItemNo;
    for(ItemNo=0;ItemNo<=MaxItemNo;ItemNo++)
    {
        Weight[ItemNo] = 1.0;
    }
}
float GetInfo::SplitContinuousAtt(int AttNo)
{   
	float BaseInfo;
	float LowItems = 0;;
	int c;
	float MinSplit;
	float AvGain=0; 
	int Tries=0;
	ResetFreq(2);
	Quicksort(0, MaxItemNo,AttNo);
	for(int i=0;i<=MaxItemNo;i++) //#define  Class(Case)		Case[MaxAtt+1]._discr_val
    {
	    Freq[ 2 ][ Item[i][MaxAttNo+1].DiscrValue ] += Weight[i];
	    SplitGain[i] = -(float)Epsilon;
	    SplitInfo[i] = 0;
     }
     BaseInfo = TotalInfo(Freq[2], 0, MaxClassNo) /(MaxItemNo+1);

	 MinSplit = (float)0.10 * (MaxItemNo+1) / (MaxClassNo + 1);
     if ( MinSplit <= MINOBJS ) 
		MinSplit =(float) MINOBJS;
     else if ( MinSplit > 25 ) 
		MinSplit = 25;     
     for(int i=0;i<=MaxItemNo-1;i++)
     {
		c = Item[i][MaxAttNo+1].DiscrValue;//#define  Class(Case)		Case[MaxAtt+1]._discr_val
	    LowItems   += Weight[i];
	    Freq[1][c] += Weight[i];
	    Freq[2][c] -= Weight[i];
	    if ( LowItems < MinSplit ) 
			continue;
	    else if ( LowItems > MaxItemNo+1 - MinSplit ) 
			break;
		if (Item[i][AttNo].continuousVal<Item[i+1][AttNo].continuousVal- 1E-5 )
	    {
	        ValFreq[1] = LowItems;
	        ValFreq[2] = MaxItemNo+1 - LowItems;
	        SplitGain[i] = ComputeGain(BaseInfo,2,MaxItemNo+1);
	        //SplitInfo[i] = TotalInfo(ValFreq, 0, 2) / Items;
	     }
	 }
	 float BestVal = 0;
     int BestI   = None;
	 float Val;
     for(int i=0;i<=MaxItemNo-1;i++)
     {
	     if((Val = SplitGain[i])> BestVal )
	     {
	         BestI   = i;
	         BestVal = Val;
	     }
     }
	 Bar[AttNo]  = (Item[BestI][AttNo].continuousVal +Item[BestI+1][AttNo].continuousVal) / 2;
	 Gain[AttNo] = BestVal;
	 return Bar[AttNo];
	
}
float GetInfo::ComputeGain(float BaseInfo,int MaxValNo,float TotalItems)
/*    -----------  */
{
    int v;
    float ThisInfo=0.0, ThisGain;
    short ReasonableSubsets=0;

    /*  Check whether all values are unknown or the same  */

    if ( ! TotalItems ) return -(float)Epsilon;

    /*  There must be at least two subsets with MINOBJS items  */

    for(v=1;v<=MaxValNo;v++)
    {
	    if ( ValFreq[v] >= MINOBJS ) 
			ReasonableSubsets++;
    }
    if ( ReasonableSubsets < 2 ) 
		return -(float)Epsilon;

    /*  Compute total info after split, by summing the
	info of each of the subsets formed by the test  */

    for(v=1;v<=MaxValNo;v++)
    {
	    ThisInfo += TotalInfo(Freq[v], 0, MaxClassNo);
    }

    /*  Set the gain in information for all items, adjusted for unknowns  */

    ThisGain=BaseInfo - ThisInfo / TotalItems;
    return ThisGain;
}
/*************************************************************************/
/*									 */
/*  Zero the frequency tables Freq[][] and ValFreq[] up to MaxVal	 */
/*									 */
/*************************************************************************/
void GetInfo::ResetFreq(float MaxDiscrValNo)
{
    int DiscrValue;
    int ClassNo;

    for(DiscrValue=0;DiscrValue<=MaxDiscrValNo;DiscrValue++)
    { 
	    for(ClassNo=0;ClassNo<=MaxClassNo;ClassNo++)
	    {
	        Freq[DiscrValue][ClassNo] = 0;
	    }
	    ValFreq[DiscrValue] = 0;
    } 
}
void  GetInfo::Swap(int ItemA,int ItemB)
/*   ----  */
{
    register Description Hold;
    register float HoldW;
    Hold = Item[ItemA];
    Item[ItemA] = Item[ItemB];
    Item[ItemB] = Hold;
    HoldW = Weight[ItemA];
    Weight[ItemA] = Weight[ItemB];
    Weight[ItemB] = HoldW;
}
void GetInfo::Quicksort(int ItemNoFp,int ItemNoLp ,int AttNo)
{
	register int Lower, Middle;
    register float Thresh;
    register int i;
    if ( ItemNoFp < ItemNoLp )
    {
		Thresh =Item[ItemNoLp][AttNo].continuousVal;		
		/*  Isolate all items with values <= threshold  */		
		Middle = ItemNoFp;		
		for ( i = ItemNoFp ; i < ItemNoLp ; i++ )
		{ 
			if ( Item[i][AttNo].continuousVal<= Thresh )
			{ 
				if ( i != Middle ) 
					Swap(Middle, i);
				Middle++; 
			} 
		} 		
		/*  Extract all values equal to the threshold  */		
		Lower = Middle - 1;		
		for ( i = Lower ; i >= ItemNoFp ; i-- )
		{
			if (Item[i][AttNo].continuousVal == Thresh )
			{ 
				if ( i != Lower ) 
					Swap(Lower, i);
				Lower--;
			} 
		} 
		
		/*  Sort the lower values  */
		
		Quicksort( ItemNoFp, Lower, AttNo);
		
		/*  Position the middle element  */
		
		Swap(Middle,ItemNoLp);
		
		/*  Sort the higher values  */
		
		Quicksort(Middle+1,  ItemNoLp, AttNo);
    }
}
float GetInfo::TotalInfo(float V[],int MinVal,int MaxVal)
/*--------- */
{
    int v;
    float Sum=0.0;
    float N, TotalItems=0;
    for(v=MinVal;v<=MaxVal;v++)
    {
	    N = V[v];
	    Sum += N * (float) Log(N);
	    TotalItems += N;
    }
    return TotalItems *(float)Log(TotalItems) - Sum;
}