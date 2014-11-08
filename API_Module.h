//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: API_Module                           ///
///       Produced by: XXX                          ///
///       Description: Produced to deal with the       ///
///                    functions between Interpreter,  ///
///                    Record_Manager and Index_Manager///
///       date: 2004/12/29                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

//#include "MiniSQL.h"
#include <string>
using namespace std;

struct conditionInfo{
    int type,symbol;
    string left;
    int right;
};

struct attr_info{
	char name[20],index_name[20];
	char data_type,attr_type;
	int  data_len,num;
};

void  API_Module(string SQL);
//��ʾ������Ϣ
//void Help();
//���������ڵ�
//void Insert_Index(string DB_Name,string Table_Name,index_info & index);
//ɾ�������ڵ�
//void Delete_Index(string DB_Name,string Table_Name,index_info & index);
//���������ڵ�
//void Find_Index(string DB_Name,string Table_Name,index_info & index);
//��ȡ������������Ϣ
//void Get_Index(string DB_Name,string Table_Name,int & start,int & end,int type,index_info & index);
conditionInfo Str_To_Conds(string str);