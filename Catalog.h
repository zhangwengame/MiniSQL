///////////////////////////////////////////////////////////////
///---------------------------------------------------------///
///       Module: Catalog                                   ///
///       Produced by: Wen Zhang                            ///
///       Description: Get and set parameters configuration ///
///                    parameters , create and remove files ///
///                    according to orders from other parts.///
///       date: 2014/11/18                                  ///
///-------------------------------------------------------- ///
///////////////////////////////////////////////////////////////
#ifndef _CATALOG_H 
#define _CATALOG_H

#include <fstream>
#include <windows.h>
#include <string.h>

using namespace std;

struct attrInfo{
	string attrName;
	string indexName;
	int pri;
	int type;
	int index;	
};

void createDatabase(string DB_Name);
void createTable(string DB_Name, string Table_Name);
void createIndex(string DB_Name, string Table_Name, string Attr_Name, string Index_Name);
void addAttr(string DB_Name, string Table_Name, string Attr_Name, int Attr_Len, int Attr_Type, int Data_Type);
int attrOrder(string DB_Name, string Table_Name, string Attr_Name);
bool existDatabase(string DB_Name);
bool existTable(string DB_Name, string Table_Name);
void dropIndex(string DB_Name, string Table_Name, string Index_Name);
void dropTable(string DB_Name, string Table_Name);
void dropDatabase(string DB_Name);
int getRecordSum(string DB_Name, string Table_Name);
void setRecordSum(string DB_Name, string Table_Name,int val);
attrInfo *getAttrInfo(string DB_Name, string Table_Name, string Attr_Name);
attrInfo *getAttrInfo(string DB_Name, string Table_Name, int Attr_No);
#endif
