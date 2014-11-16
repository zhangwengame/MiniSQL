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
#include "Record.h"
#include <iostream>
#include <string>
using namespace std;

void  API_Module(string SQL);
//显示帮助信息
//void Help();
//插入索引节点
//void Insert_Index(string DB_Name,string Table_Name,index_info & index);
//删除索引节点
//void Delete_Index(string DB_Name,string Table_Name,index_info & index);
//查找索引节点
//void Find_Index(string DB_Name,string Table_Name,index_info & index);
//获取条件的索引信息
//void Get_Index(string DB_Name,string Table_Name,int & start,int & end,int type,index_info & index);
conditionInfo Str_To_Conds(string str);
