//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Record                               ///
///       Produced by: Yuheng Zhou                     ///
///       Description: Deal with each lines of index,  ///
///                    conduct the operations such as  ///
///                    insert, select and delete.      ///
///       date: 2014/11/18                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////
#ifndef _RECORD_H 
#define _RECORD_H

#define BLOCK_LEN			4096	// the size of one block
#define MAX_FILE_ACTIVE 	5		// limit the active files in the buffer
#define MAX_BLOCK			40		// the max number of the blocks
#define LinB                8
#include <time.h>
#include <fstream>
#include <set>
#include <string>
#include "Buffer.h"
using namespace std;
#include "Catalog.h"
#include "Index.h"

struct attr_info{
	int num;
	string name;
};

struct conditionInfo{
    int type,symbol;
    string left;
    int right0;
    char right1[40];
    float right2;
};

void Insert_Item(string DB_Name,string Table_Name,string Attr,int & record_Num,bufferInfo *bufferInfo);
bool Confirm_To_Where(string DB_Name,string Table_Name,char *detail[10],conditionInfo conds[10],int count,char cond);
bool Confirmi(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition);
bool Confirmc(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition);
bool Confirmf(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition);
void Select_No_Where(string DB_Name,string Table_Name,attr_info print[32],int count,int all,bufferInfo *bufferInfo);
void Select_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,char cond,attr_info print[32],
     int Count,int all,bufferInfo *bufferInfo,int use,int &check);
void Delete_No_Where(string DB_Name,string Table_Name,bufferInfo *bufferInfo);
void Delete_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,
     char cond,bufferInfo *bufferInfo);
#endif
