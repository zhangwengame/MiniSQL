#ifndef _RECORD_H 
#define _RECORD_H

#define BLOCK_LEN			4096	// the size of one block
#define MAX_FILE_ACTIVE 	5		// limit the active files in the buffer
#define MAX_BLOCK			40		// the max number of the blocks
#define LinB                8
#include <time.h>
#include <fstream>
#include <set>
#include "Buffer.h"
using namespace std;
#include "Catalog.h"

struct attr_info{
	int num;
	string name;
};

struct index_info{
};

struct conditionInfo{
    int type,symbol;
    string left;
    int right;
};

//char *lsplit=" ",*esplit=",";

void Close_Database(string DB_Name,bool closetype);
void Close_File(string DB_Name,string filename,int filetype,bool closetype);
void Insert_Item(string DB_Name,string Table_Name,string Attr,int & record_Num);
void Print_Head(attr_info print[32],int count);
void Print_To_Screen(string record,attr_info print[32],int count);
void Select_Without_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,attr_info print[32],int Count,char cond);
void Select_With_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,attr_info print[32],int Count,char cond,int index);
void Select_With_Equal_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index);
void Select_With_Greater_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
bool Confirm_To_Where(string DB_Name,string Table_Name,char *detail[10],conditionInfo conds[10],int count,char cond);
bool Confirm(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition);
void Select_No_Where(string DB_Name,string Table_Name,attr_info print[32],int count,int all);
void Select_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,char cond,attr_info print[32],int Count,int all);
void Delete_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_Without_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_With_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond,int index);
void Delete_With_Equal_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond,index_info Index);
void Delete_With_Greater_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
bool Find_Useful_Cond(conditionInfo conds[10],int count,int & index);
void Insert_Index_All(string DB_Name,string Table_Name,string Index_Name,int length,int offset,int type);
bool Verify_Insertable(string DB_Name,string Table_Name,index_info nodes[32],int count,string Attr);
void Quit(string DB_Name);

#endif
